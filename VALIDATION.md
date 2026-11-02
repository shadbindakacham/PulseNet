# PulseNet validation plan

Goal: show how close PulseNet's numbers are to a reference device, and how reliable the IoT system is. A table of measured errors is what turns "it displays a heart rate" into an engineering result.

**What this can and cannot show.** The reference is a consumer fingertip oximeter or smartwatch, not a clinical instrument. So you are measuring *agreement with a consumer device*, not absolute accuracy. Say that in your report.

## 1. What you need
- PulseNet running (local dashboard open on a laptop or phone).
- One reference: a fingertip pulse oximeter (best, gives both heart rate and SpO2) and/or a smartwatch (heart rate).
- 3 to 5 volunteers who agree to take part. Keep their data anonymous (S1, S2, ...).
- A timer, and `extras/validation_template.csv` for the log.

## 2. Reference vs PulseNet: heart rate and SpO2

### Conditions (per subject: 6 trials)
| Condition | Trials | How |
|---|---|---|
| **Rest** | 3 | Sit quietly for 5 minutes. Take readings 1 minute apart. |
| **After light exercise** | 3 | 1 minute of jumping jacks or fast stair climbing, then sit and read immediately, then at +1 and +2 minutes. |

Do not do breath-holding or anything meant to lower oxygen on purpose. SpO2 only needs to be checked in the normal range.

### Procedure for each trial
1. Put the electrodes on (RA right chest, LA left chest, RL lower right ribs). **Unplug the Mac charger first**, and power the D1 from a power bank.
2. Put the MAX30102 on one fingertip and the reference oximeter on a finger of the **other** hand. Keep both hands still on a table.
3. Wait until both devices settle (about 20 to 30 seconds). Do not start if signal quality on the dashboard is below about 60.
4. Read all values **at the same moment**: PulseNet ECG heart rate, PulseNet finger pulse, PulseNet SpO2, reference heart rate, reference SpO2. A second person reading and typing helps, or take a phone photo of both screens at once.
5. Write one row in the CSV. Note anything unusual in `notes` (movement, loose electrode, cold hands).

Aim for at least 30 rows in total, including some in the elevated range (above 100 bpm), because that is where the errors show up.

### Metrics (computed by `extras/analyze_validation.py`)
| Metric | Meaning |
|---|---|
| **MAE** | Average size of the error, in bpm or SpO2 points. The headline number. |
| **Bias** | Average signed difference. Shows if PulseNet reads systematically high or low. |
| **Limits of agreement** | Bias ± 1.96 × SD of the differences (Bland-Altman). About 95% of readings should fall in this range. |
| **% within ±5 bpm** | Share of readings close enough to be useful. |
| **r** | Correlation between the two devices. |

Compared pairs: ECG vs reference, finger pulse vs reference, ECG vs finger pulse (the two PulseNet sensors against each other), SpO2 vs reference.

### Suggested targets (state them in advance, before measuring)
- Heart rate at rest: MAE ≤ 3 bpm.
- Heart rate after exercise: MAE ≤ 5 bpm.
- SpO2: MAE ≤ 3 points. The SpO2 formula is a rough approximation, so a larger error here is a finding to report honestly, not a failure.

If the result misses a target, report the number and the likely cause (electrode contact, motion, uncalibrated SpO2 formula). Do not adjust the data.

## 3. System tests (IoT reliability)

| Test | Method | Record |
|---|---|---|
| **Lead-off response time** | Film the dashboard and your hand at 60 fps. Pull an electrode off. Count frames until "Electrodes not connected" appears. | Seconds (frames ÷ 60). |
| **Remote latency** | Open the local dashboard and `remote.html` side by side, film both, and change something visible on both, such as lifting your finger off the sensor. Compare when each one reacts. | Seconds for local and for remote. |
| **WiFi drop recovery** | Run normally, turn the router or phone hotspot off for 60 seconds, then back on. | Time until the dashboard reconnects, and until MQTT shows `M` on the OLED again. |
| **Long run** | Leave it running 30 minutes with MQTT on and the remote viewer open. | Any disconnects (see the remote viewer's alert log and status), and the lowest free heap printed in Serial Monitor. |
| **Alert delivery** | With ntfy set up, trigger a high-heart-rate alert (exercise), or temporarily lower `HR_HIGH` in the code to 70 at rest. | Seconds from the condition starting to the phone notification. Expect about 10 s of hold plus network time. |

## 4. Report outline
1. Architecture diagram (use the one in `README.md`).
2. Method: devices, subjects, conditions, number of trials.
3. Results: the table from `validation_results.md` and a Bland-Altman plot if you make one.
4. System-test results table.
5. Limitations: consumer reference, small sample, uncalibrated SpO2, not a medical device.
6. Future work: calibrated SpO2, ESP32 with 12-bit ADC, medical-grade validation.

## 5. Ethics and safety
Volunteers must agree first, and their data stays anonymous. Anyone with a heart condition, a pacemaker, or skin sensitivity to adhesive pads should skip the ECG part. Stop if anyone feels unwell.
