// PulseNet: ECG + finger pulse + SpO2 monitor with a live WiFi dashboard,
// MQTT telemetry and phone push alerts.
// Board: LOLIN(WEMOS) D1 R2 & mini (ESP8266)
//
// Wiring (all 3.3V):
//   OLED + MAX30102 : SDA -> D14 (GPIO4), SCL -> D15 (GPIO5)
//   AD8232          : OUTPUT -> A0, LO+ -> D12 (GPIO12), LO- -> D13 (GPIO14)
//
// First boot: the D1 opens a hotspot "PulseNet-Setup" (password pulsenet123).
// Join it from a phone and a setup page appears where you enter your WiFi plus
// optional MQTT broker and ntfy.sh topic. Settings are stored in flash.

#include <Wire.h>
#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266mDNS.h>
#include <WiFiClientSecure.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <WebSocketsServer.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "MAX30105.h"
#include "heartRate.h"
#include "dashboard.h"

// ---- Fixed settings ----
const char* SETUP_AP_SSID = "PulseNet-Setup";
const char* SETUP_AP_PASS = "pulsenet123";
const char* FALLBACK_AP_SSID = "PulseNet";
const char* FALLBACK_AP_PASS = "pulsenet123";

// Alert thresholds (a condition must hold for ALERT_HOLD_MS before it fires)
const int HR_HIGH = 120;
const int HR_LOW = 50;
const int SPO2_LOW = 92;
const unsigned long ALERT_HOLD_MS = 10000;
const unsigned long PUSH_COOLDOWN_MS = 300000;  // same alert type at most every 5 min

// ---- Pins ----
const int PIN_LO_PLUS = 12;
const int PIN_LO_MINUS = 14;

// ---- Saved configuration (entered in the setup portal) ----
struct Config {
  char mqttHost[64];
  char mqttPort[6];
  char mqttUser[32];
  char mqttPass[48];
  char devId[24];
  char ntfy[40];
} cfg;

// ---- Objects ----
Adafruit_SSD1306 oled(128, 64, &Wire, -1);
MAX30105 ppg;
ESP8266WebServer server(80);
WebSocketsServer ws(81);
WiFiClient plainClient;
WiFiClientSecure tlsClient;
PubSubClient mqtt;

// ---- ECG state ----
const unsigned long ECG_INTERVAL_US = 4000;  // 250 samples per second
unsigned long lastEcgUs = 0;
int ecgBatch[10];
byte ecgCount = 0;
bool leadsOff = true;
int ecgBpm = 0;

// ECG filter: 5-tap moving average has exact zeros at 50 Hz and 100 Hz when
// sampling at 250 Hz (mains hum), then a ~0.5 Hz high-pass removes baseline drift.
float maBuf[5];
float maSum = 0;
byte maIdx = 0;
float baseline = 0;
bool filtReady = false;

int winMin = 1023, winMax = 0, lastMin = 0, lastMax = 0;
unsigned long winStart = 0;
int prevEcg = 0;
unsigned long lastR = 0;
unsigned long rr[5];
byte rrCount = 0, rrIdx = 0;

// ---- PPG (finger) state ----
const int WIN = 200;  // 2 seconds at 100 samples per second
uint32_t redBuf[WIN], irBuf[WIN];
int winIdx = 0;
bool finger = false;
int ppgBpm = 0;
int spo2 = 0;
float spo2Smooth = 0;
const byte RATE_SIZE = 4;
byte rates[RATE_SIZE];
byte rateSpot = 0;
long lastBeat = 0;

// ---- Cloud state ----
bool mqttEnabled = false;
unsigned long mqttNextTry = 0, mqttBackoff = 10000;
String tBase;

struct AlertState {
  unsigned long since = 0;
  unsigned long lastSent = 0;
  bool active = false;
};
AlertState aHigh, aLow, aSpo2;

// ---- Timers ----
unsigned long lastVitals = 0, lastSecond = 0, lastHeap = 0;
String ipText = "";
bool staMode = false;

// ---------------------------------------------------------------
// Config storage
void setDefaults() {
  memset(&cfg, 0, sizeof(cfg));
  strlcpy(cfg.mqttPort, "1883", sizeof(cfg.mqttPort));
  snprintf(cfg.devId, sizeof(cfg.devId), "pulsenet-%06x", ESP.getChipId());
}

void setKey(const String& k, const String& v) {
  if (k == "mqtt_host") strlcpy(cfg.mqttHost, v.c_str(), sizeof(cfg.mqttHost));
  else if (k == "mqtt_port") strlcpy(cfg.mqttPort, v.c_str(), sizeof(cfg.mqttPort));
  else if (k == "mqtt_user") strlcpy(cfg.mqttUser, v.c_str(), sizeof(cfg.mqttUser));
  else if (k == "mqtt_pass") strlcpy(cfg.mqttPass, v.c_str(), sizeof(cfg.mqttPass));
  else if (k == "dev_id") strlcpy(cfg.devId, v.c_str(), sizeof(cfg.devId));
  else if (k == "ntfy") strlcpy(cfg.ntfy, v.c_str(), sizeof(cfg.ntfy));
}

void loadConfig() {
  setDefaults();
  if (!LittleFS.begin()) return;
  File f = LittleFS.open("/cfg.txt", "r");
  if (!f) return;
  while (f.available()) {
    String line = f.readStringUntil('\n');
    line.trim();
    int eq = line.indexOf('=');
    if (eq > 0) setKey(line.substring(0, eq), line.substring(eq + 1));
  }
  f.close();
}

void saveConfig() {
  File f = LittleFS.open("/cfg.txt", "w");
  if (!f) return;
  f.printf("mqtt_host=%s\nmqtt_port=%s\nmqtt_user=%s\nmqtt_pass=%s\ndev_id=%s\nntfy=%s\n",
           cfg.mqttHost, cfg.mqttPort, cfg.mqttUser, cfg.mqttPass, cfg.devId, cfg.ntfy);
  f.close();
}

// ---------------------------------------------------------------
void oledMessage(const char* a, const char* b = "", const char* c = "") {
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setCursor(0, 0);
  oled.println("PulseNet");
  oled.setCursor(0, 20);
  oled.println(a);
  oled.println(b);
  oled.println(c);
  oled.display();
}

void startNetwork() {
  WiFi.mode(WIFI_STA);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);

  {  // WiFiManager and its parameters only live during setup, to save RAM
    WiFiManager wm;
    WiFiManagerParameter pTitle("<p><b>Optional cloud settings</b> (leave blank to skip)</p>");
    WiFiManagerParameter pHost("mqtt_host", "MQTT broker host", cfg.mqttHost, 63);
    WiFiManagerParameter pPort("mqtt_port", "MQTT port (1883, or 8883 for TLS)", cfg.mqttPort, 5);
    WiFiManagerParameter pUser("mqtt_user", "MQTT username", cfg.mqttUser, 31);
    WiFiManagerParameter pPass("mqtt_pass", "MQTT password", cfg.mqttPass, 47);
    WiFiManagerParameter pId("dev_id", "Device name (used in MQTT topics)", cfg.devId, 23);
    WiFiManagerParameter pNtfy("ntfy", "ntfy.sh topic for phone alerts", cfg.ntfy, 39);
    wm.addParameter(&pTitle);
    wm.addParameter(&pHost);
    wm.addParameter(&pPort);
    wm.addParameter(&pUser);
    wm.addParameter(&pPass);
    wm.addParameter(&pId);
    wm.addParameter(&pNtfy);
    wm.setConnectTimeout(20);
    wm.setConfigPortalTimeout(180);
    wm.setTitle("PulseNet setup");
    wm.setAPCallback([](WiFiManager*) {
      oledMessage("Setup mode", "Join WiFi: PulseNet-Setup", "Open 192.168.4.1");
    });

    oledMessage("Joining WiFi...");
    bool ok = wm.autoConnect(SETUP_AP_SSID, SETUP_AP_PASS);

    if (ok) {  // pick up any values the user just saved in the portal
      setKey("mqtt_host", pHost.getValue());
      setKey("mqtt_port", pPort.getValue());
      setKey("mqtt_user", pUser.getValue());
      setKey("mqtt_pass", pPass.getValue());
      setKey("dev_id", pId.getValue());
      setKey("ntfy", pNtfy.getValue());
      saveConfig();
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    staMode = true;
    ipText = WiFi.localIP().toString();
  } else {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(FALLBACK_AP_SSID, FALLBACK_AP_PASS);
    ipText = WiFi.softAPIP().toString();
    Serial.println("WiFi not configured, started fallback hotspot");
  }
  Serial.print("Dashboard: http://");
  Serial.println(ipText);

  if (MDNS.begin("pulsenet")) MDNS.addService("http", "tcp", 80);

  server.on("/", []() { server.send_P(200, "text/html", DASHBOARD); });
  server.on("/setup", []() {  // /setup?confirm=1 wipes WiFi and reopens the setup portal
    if (!server.hasArg("confirm")) {
      server.send(200, "text/plain", "Add ?confirm=1 to erase the saved WiFi and reopen the setup hotspot.");
      return;
    }
    server.send(200, "text/plain", "Erasing WiFi. Join PulseNet-Setup to reconfigure.");
    delay(500);
    WiFiManager wm;
    wm.resetSettings();
    ESP.restart();
  });
  server.begin();
  ws.begin();
}

// ---------------------------------------------------------------
// MQTT
void setupMqtt() {
  mqttEnabled = staMode && cfg.mqttHost[0];
  if (!mqttEnabled) return;
  int port = atoi(cfg.mqttPort);
  if (port == 8883) {
    tlsClient.setInsecure();  // encrypted, but the broker's certificate is not verified
    tlsClient.setBufferSizes(512, 512);
    mqtt.setClient(tlsClient);
  } else {
    mqtt.setClient(plainClient);
  }
  mqtt.setServer(cfg.mqttHost, port);
  mqtt.setBufferSize(384);
  mqtt.setSocketTimeout(3);
  mqtt.setKeepAlive(30);
  tBase = String("pulsenet/") + cfg.devId;
}

void mqttService() {
  if (!mqttEnabled) return;
  if (mqtt.connected()) {
    mqtt.loop();
    return;
  }
  if (millis() < mqttNextTry) return;

  String clientId = String(cfg.devId) + "-" + String(ESP.getChipId(), HEX);
  String will = tBase + "/status";
  bool ok = mqtt.connect(clientId.c_str(), cfg.mqttUser[0] ? cfg.mqttUser : nullptr,
                         cfg.mqttUser[0] ? cfg.mqttPass : nullptr, will.c_str(), 1, true, "offline");
  if (ok) {
    mqtt.publish(will.c_str(), "online", true);
    mqttBackoff = 10000;
    Serial.println("MQTT connected");
  } else {
    // A failed (especially TLS) connect blocks for a moment, so back off.
    mqttNextTry = millis() + mqttBackoff;
    mqttBackoff = min(mqttBackoff * 2, 60000UL);
    Serial.printf("MQTT connect failed (rc=%d), retry in %lus\n", mqtt.state(), mqttBackoff / 1000);
  }
}

void publishVitals() {
  if (!mqttEnabled || !mqtt.connected()) return;
  char buf[200];
  snprintf(buf, sizeof(buf),
           "{\"ecg\":%d,\"ppg\":%d,\"spo2\":%d,\"finger\":%d,\"leadsOff\":%d,\"rssi\":%d,\"heap\":%u,\"up\":%lu}",
           ecgBpm, ppgBpm, spo2, finger ? 1 : 0, leadsOff ? 1 : 0, WiFi.RSSI(), ESP.getFreeHeap(), millis() / 1000);
  mqtt.publish((tBase + "/vitals").c_str(), buf);
}

void publishAlert(const char* type, const char* state, int value) {
  if (!mqttEnabled || !mqtt.connected()) return;
  char buf[96];
  snprintf(buf, sizeof(buf), "{\"type\":\"%s\",\"state\":\"%s\",\"value\":%d}", type, state, value);
  mqtt.publish((tBase + "/alert").c_str(), buf);
}

// ---------------------------------------------------------------
// Phone push via ntfy.sh (HTTPS). Blocks for a second or two, which is why
// each alert type has a long cooldown.
void sendPush(const String& title, const String& msg) {
  if (!cfg.ntfy[0] || WiFi.status() != WL_CONNECTED) return;
  WiFiClientSecure c;
  c.setInsecure();
  c.setBufferSizes(512, 512);
  HTTPClient http;
  http.setTimeout(4000);
  if (!http.begin(c, String("https://ntfy.sh/") + cfg.ntfy)) return;
  http.addHeader("Title", title);
  http.addHeader("Priority", "high");
  http.addHeader("Tags", "warning");
  int code = http.POST(msg);
  Serial.printf("ntfy push -> %d\n", code);
  http.end();
}

void evalAlert(AlertState& a, bool cond, const char* type, int value, const String& msg) {
  unsigned long now = millis();
  if (!cond) {
    a.since = 0;
    if (a.active) {
      a.active = false;
      publishAlert(type, "resolved", value);
    }
    return;
  }
  if (!a.since) a.since = now;
  if (!a.active && now - a.since >= ALERT_HOLD_MS) {
    a.active = true;
    publishAlert(type, "active", value);
    if (a.lastSent == 0 || now - a.lastSent > PUSH_COOLDOWN_MS) {
      a.lastSent = now;
      sendPush("PulseNet alert", msg);
    }
  }
}

void checkAlerts() {
  int hr = ecgBpm > 0 ? ecgBpm : ppgBpm;
  evalAlert(aHigh, hr > HR_HIGH, "high_hr", hr, "Heart rate high: " + String(hr) + " bpm");
  evalAlert(aLow, hr > 0 && hr < HR_LOW, "low_hr", hr, "Heart rate low: " + String(hr) + " bpm");
  evalAlert(aSpo2, spo2 > 0 && spo2 < SPO2_LOW, "low_spo2", spo2, "SpO2 low: " + String(spo2) + "%");
}

// ---------------------------------------------------------------
// ECG
int filterEcg(int raw) {
  if (!filtReady) {  // start from the first sample so there is no startup spike
    for (byte i = 0; i < 5; i++) maBuf[i] = raw;
    maSum = raw * 5.0f;
    baseline = raw;
    filtReady = true;
  }
  maSum += raw - maBuf[maIdx];
  maBuf[maIdx] = raw;
  maIdx = (maIdx + 1) % 5;
  float smooth = maSum / 5.0f;
  baseline += 0.0126f * (smooth - baseline);  // ~0.5 Hz corner at 250 Hz
  int y = (int)(512 + (smooth - baseline));
  if (y < 1) y = 1;
  if (y > 1023) y = 1023;
  return y;
}

void detectRPeak(int v) {
  unsigned long now = millis();

  if (now - winStart > 2000) {  // refresh amplitude window every 2 s
    lastMin = winMin;
    lastMax = winMax;
    winMin = 1023;
    winMax = 0;
    winStart = now;
  }
  if (v < winMin) winMin = v;
  if (v > winMax) winMax = v;

  int range = lastMax - lastMin;
  if (range > 40) {
    // The range includes the deep S-wave, so the baseline sits near mid-range.
    // 75% keeps P- and T-waves below the threshold and only lets the R-peak through.
    int thr = lastMin + (range * 75) / 100;
    if (prevEcg < thr && v >= thr && now - lastR > 300) {
      unsigned long reported = 0;  // RR interval in ms, 0 if not a valid interval
      if (lastR != 0) {
        unsigned long interval = now - lastR;
        if (interval >= 300 && interval <= 2000) {
          reported = interval;
          rr[rrIdx] = interval;
          rrIdx = (rrIdx + 1) % 5;
          if (rrCount < 5) rrCount++;
          unsigned long sum = 0;
          for (byte i = 0; i < rrCount; i++) sum += rr[i];
          ecgBpm = (int)(60000UL * rrCount / sum);
        }
      }
      lastR = now;
      if (ws.connectedClients() > 0) {  // dashboard uses this for peak markers and HRV
        String s = "R:";
        s += reported;
        ws.broadcastTXT(s);
      }
    }
  }
  prevEcg = v;

  if (lastR != 0 && now - lastR > 3000) {  // no beat for 3 s
    ecgBpm = 0;
    rrCount = 0;
  }
}

void sampleEcg() {
  unsigned long now = micros();
  if (now - lastEcgUs < ECG_INTERVAL_US) return;
  lastEcgUs = now;

  leadsOff = digitalRead(PIN_LO_PLUS) == 1 || digitalRead(PIN_LO_MINUS) == 1;
  int v = 0;

  if (leadsOff) {
    filtReady = false;
    ecgBpm = 0;
    rrCount = 0;
    lastR = 0;
  } else {
    v = filterEcg(analogRead(A0));
    detectRPeak(v);
  }

  ecgBatch[ecgCount++] = v;
  if (ecgCount >= 10) {
    ecgCount = 0;
    if (ws.connectedClients() > 0) {
      String s = "E:";
      for (byte i = 0; i < 10; i++) {
        if (i) s += ',';
        s += ecgBatch[i];
      }
      ws.broadcastTXT(s);
    }
  }
}

// ---------------------------------------------------------------
// Finger sensor: pulse + SpO2 estimate
void resetPpg() {
  finger = false;
  ppgBpm = 0;
  spo2 = 0;
  spo2Smooth = 0;
  winIdx = 0;
  for (byte i = 0; i < RATE_SIZE; i++) rates[i] = 0;
}

void computeSpo2() {
  uint64_t sumR = 0, sumI = 0;
  uint32_t minR = 0xFFFFFFFF, maxR = 0, minI = 0xFFFFFFFF, maxI = 0;
  for (int i = 0; i < WIN; i++) {
    sumR += redBuf[i];
    sumI += irBuf[i];
    if (redBuf[i] < minR) minR = redBuf[i];
    if (redBuf[i] > maxR) maxR = redBuf[i];
    if (irBuf[i] < minI) minI = irBuf[i];
    if (irBuf[i] > maxI) maxI = irBuf[i];
  }
  float dcR = sumR / (float)WIN, dcI = sumI / (float)WIN;
  float acR = maxR - minR, acI = maxI - minI;
  if (dcR < 1 || dcI < 1 || acI < 1) return;

  float ratio = (acR / dcR) / (acI / dcI);
  float est = 110.0f - 25.0f * ratio;  // rough textbook approximation
  if (est > 100) est = 100;
  if (est < 70) return;  // reject nonsense windows

  spo2Smooth = spo2Smooth == 0 ? est : 0.7f * spo2Smooth + 0.3f * est;
  spo2 = (int)(spo2Smooth + 0.5f);
}

void processPpgSample(uint32_t red, uint32_t ir) {
  if (ir < 50000) {
    if (finger) resetPpg();
    return;
  }
  finger = true;

  if (checkForBeat(ir)) {
    long delta = millis() - lastBeat;
    lastBeat = millis();
    float bpm = 60.0f / (delta / 1000.0f);
    if (bpm > 30 && bpm < 220) {
      rates[rateSpot++ % RATE_SIZE] = (byte)bpm;
      int sum = 0, n = 0;
      for (byte i = 0; i < RATE_SIZE; i++)
        if (rates[i]) { sum += rates[i]; n++; }
      if (n) ppgBpm = sum / n;
    }
  }

  redBuf[winIdx] = red;
  irBuf[winIdx] = ir;
  if (++winIdx >= WIN) {
    winIdx = 0;
    computeSpo2();
  }
}

void readPpg() {
  ppg.check();
  while (ppg.available()) {
    uint32_t red = ppg.getFIFORed();
    uint32_t ir = ppg.getFIFOIR();
    ppg.nextSample();
    processPpgSample(red, ir);
  }
}

// ---------------------------------------------------------------
void sendVitals() {
  if (ws.connectedClients() > 0) {
    String s = "V:";
    s += ecgBpm;  s += ',';
    s += ppgBpm;  s += ',';
    s += spo2;    s += ',';
    s += finger ? 1 : 0;  s += ',';
    s += leadsOff ? 1 : 0;
    ws.broadcastTXT(s);
  }
}

void showVal(int v, const char* unit) {
  if (v > 0) {
    oled.print(v);
    oled.print(unit);
  } else {
    oled.print("--");
  }
}

void updateOled() {
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setCursor(0, 0);
  oled.print(ipText);
  if (mqttEnabled) {  // small cloud marker: M = connected to the MQTT broker
    oled.setCursor(122, 0);
    oled.print(mqtt.connected() ? "M" : "x");
  }

  oled.setTextSize(2);
  oled.setCursor(0, 12);
  oled.print("PPG ");
  showVal(ppgBpm, "");
  oled.setCursor(0, 29);
  oled.print("SpO2 ");
  showVal(spo2, "%");
  oled.setCursor(0, 46);
  oled.print("ECG ");
  if (leadsOff) oled.print("off");
  else showVal(ecgBpm, "");
  oled.display();
}

// ---------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  pinMode(PIN_LO_PLUS, INPUT);
  pinMode(PIN_LO_MINUS, INPUT);

  Wire.begin(4, 5);
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oled.setTextColor(SSD1306_WHITE);
  oledMessage("Starting...");

  if (!ppg.begin(Wire, I2C_SPEED_FAST)) {
    oledMessage("MAX30102 not found", "check wiring");
    Serial.println("MAX30102 not found");
    while (true) delay(1000);
  }
  // brightness, sample avg, LED mode (2 = red + IR), rate, pulse width, ADC range
  ppg.setup(0x1F, 4, 2, 400, 411, 4096);

  loadConfig();
  startNetwork();
  setupMqtt();
  winStart = millis();
  lastEcgUs = micros();
}

void loop() {
  server.handleClient();
  ws.loop();
  MDNS.update();

  readPpg();
  sampleEcg();

  unsigned long now = millis();
  if (now - lastVitals >= 500) {
    lastVitals = now;
    sendVitals();
    updateOled();
  }
  if (now - lastSecond >= 1000) {
    lastSecond = now;
    mqttService();
    publishVitals();
    checkAlerts();
  }
  if (now - lastHeap >= 10000) {
    lastHeap = now;
    Serial.printf("heap=%u rssi=%d mqtt=%d\n", ESP.getFreeHeap(), WiFi.RSSI(), mqttEnabled && mqtt.connected());
  }
}
