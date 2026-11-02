#pragma once
#include <pgmspace.h>

// GENERATED from extras/dashboard.html by extras/build_dashboard.py. Edit the .html, not this file.
const char DASHBOARD[] PROGMEM = R"rawliteral(
<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>PulseNet</title>
<style>
:root{
  --bg:#070b14;--card:rgba(20,29,48,.72);--line:rgba(148,163,184,.14);
  --text:#e8eef9;--mute:#8b9bb8;--dim:#5b6b88;
  --ecg:#22d3ee;--heart:#fb7185;--spo2:#60a5fa;--ok:#34d399;--warn:#fbbf24;--bad:#f87171;--violet:#a78bfa;
  --r:16px;
}
*{box-sizing:border-box;margin:0}
html{color-scheme:dark}
body{
  background:radial-gradient(1100px 560px at 8% -10%,rgba(34,211,238,.10),transparent),
             radial-gradient(900px 500px at 100% 0,rgba(167,139,250,.10),transparent),var(--bg);
  color:var(--text);font:15px/1.45 -apple-system,BlinkMacSystemFont,"Segoe UI",Roboto,Inter,sans-serif;
  min-height:100vh;-webkit-font-smoothing:antialiased;
}
.wrap{max-width:1180px;margin:0 auto;padding:0 16px 40px}
.mono{font-variant-numeric:tabular-nums}
header{display:flex;justify-content:space-between;align-items:center;gap:12px;padding:18px 0;flex-wrap:wrap}
.brand{display:flex;align-items:center;gap:12px}
.logo{width:40px;height:40px;border-radius:12px;display:grid;place-items:center;font-size:20px;color:#fff;
  background:linear-gradient(135deg,var(--heart),#c026d3);box-shadow:0 6px 24px rgba(251,113,133,.35)}
h1{font-size:20px;letter-spacing:.2px}
.brand small{color:var(--mute);font-size:12px}
.hdr-r{display:flex;align-items:center;gap:8px;flex-wrap:wrap}
.pill{padding:5px 12px;border-radius:999px;font-size:12px;font-weight:600;border:1px solid var(--line);color:var(--warn);background:rgba(251,191,36,.08)}
.pill.live{color:var(--ok);background:rgba(52,211,153,.10)}
.pill.demo{color:var(--violet);background:rgba(167,139,250,.12)}
.pill.off{color:var(--bad);background:rgba(248,113,113,.10)}
button{font:inherit;font-size:13px;color:var(--text);background:rgba(148,163,184,.10);border:1px solid var(--line);
  padding:6px 12px;border-radius:10px;cursor:pointer}
button:hover{background:rgba(148,163,184,.2)}
.banner{padding:13px 16px;border-radius:14px;margin-bottom:14px;font-weight:600;border:1px solid var(--line);
  background:var(--card);color:var(--mute);display:flex;gap:10px;align-items:center}
.banner.good{color:var(--ok);background:rgba(52,211,153,.09);border-color:rgba(52,211,153,.25)}
.banner.alert{color:#fff;background:linear-gradient(90deg,#b91c1c,#dc2626);border-color:transparent;animation:flash 1.1s infinite}
@keyframes flash{50%{opacity:.72}}
.grid{display:grid;grid-template-columns:repeat(12,1fr);gap:14px;margin-bottom:14px}
.c3{grid-column:span 3}.c4{grid-column:span 4}.c6{grid-column:span 6}.c8{grid-column:span 8}.c12{grid-column:span 12}
.card{background:var(--card);border:1px solid var(--line);border-radius:var(--r);padding:16px;backdrop-filter:blur(8px);min-width:0}
.card h2{font-size:12px;font-weight:600;color:var(--mute);text-transform:uppercase;letter-spacing:.08em;margin-bottom:10px;
  display:flex;justify-content:space-between;align-items:center;gap:8px}
.card h2 .tag{font-size:11px;text-transform:none;letter-spacing:0;color:var(--dim);font-weight:500}
.big{font-size:44px;font-weight:700;line-height:1.05;letter-spacing:-1px}
.unit{font-size:14px;color:var(--mute);font-weight:500;letter-spacing:0}
.sub{color:var(--mute);font-size:12px;margin-top:6px}
.chip{display:inline-block;padding:2px 10px;border-radius:999px;font-size:11px;font-weight:600;background:rgba(148,163,184,.14);color:var(--mute)}
.chip.ok{background:rgba(52,211,153,.14);color:var(--ok)}
.chip.warn{background:rgba(251,191,36,.14);color:var(--warn)}
.chip.bad{background:rgba(248,113,113,.16);color:var(--bad)}
.heart{display:inline-block;color:var(--heart);margin-right:4px}
.heart.pulse{animation:pulse .3s}
@keyframes pulse{50%{transform:scale(1.4)}}
canvas{width:100%;display:block}
#ecgc{height:280px;border-radius:10px;background:#050912}
.spk{height:38px;margin-top:8px}
.ring{position:relative;width:104px;height:104px;flex:none}
.ring svg{width:104px;height:104px;transform:rotate(-90deg)}
.ring .num{position:absolute;inset:0;display:grid;place-items:center;font-size:26px;font-weight:700}
.row{display:flex;align-items:center;gap:16px}
.metrics{display:grid;grid-template-columns:repeat(2,1fr);gap:12px;margin-bottom:12px}
.metric .v{font-size:26px;font-weight:700}
.metric .l{font-size:11px;color:var(--mute);text-transform:uppercase;letter-spacing:.06em}
.bar{height:8px;border-radius:99px;background:rgba(148,163,184,.15);overflow:hidden;margin:8px 0 6px}
.bar>i{display:block;height:100%;width:0;border-radius:99px;background:linear-gradient(90deg,var(--bad),var(--warn),var(--ok));transition:width .6s}
.bar.q>i{background:var(--ok)}
.sq{aspect-ratio:1;height:auto}
.chart{height:150px}
ul.ins{list-style:none;padding:0;display:grid;gap:8px}
ul.ins li{display:flex;gap:10px;padding:10px 12px;border-radius:12px;background:rgba(148,163,184,.07);font-size:14px}
ul.ins li::before{content:"";flex:none;width:8px;height:8px;border-radius:50%;margin-top:7px;background:var(--dim)}
ul.ins li.ok::before{background:var(--ok)}ul.ins li.warn::before{background:var(--warn)}ul.ins li.bad::before{background:var(--bad)}
.log{max-height:230px;overflow:auto;font-size:13px;color:var(--mute)}
.log div{padding:6px 0;border-bottom:1px solid var(--line);display:flex;gap:10px}
.log b{color:var(--text);font-weight:600}
.stat3{display:grid;grid-template-columns:repeat(3,1fr);gap:10px;text-align:center;margin-bottom:12px}
.zones{display:flex;height:10px;border-radius:99px;overflow:hidden;background:rgba(148,163,184,.15);margin:6px 0}
.zones i{display:block;height:100%}
.foot{color:var(--dim);font-size:12px;text-align:center;margin-top:18px}
@media(max-width:980px){.c3,.c4{grid-column:span 6}.c6,.c8{grid-column:span 12}}
@media(max-width:560px){.c4{grid-column:span 12}.big{font-size:34px}#ecgc{height:220px}.ring,.ring svg{width:88px;height:88px}.ring .num{font-size:22px}}
</style>
</head>
<body>
<div class="wrap">
<header>
  <div class="brand"><div class="logo">&#9829;</div><div><h1>PulseNet</h1><small>Live cardio monitor &middot; ECG + PPG + SpO&#8322;</small></div></div>
  <div class="hdr-r">
    <span id="pill" class="pill">connecting</span>
    <span id="timer" class="mono" style="color:var(--mute);font-size:13px">00:00</span>
    <button id="btnDemo">Demo</button><button id="btnCsv">Export CSV</button><button id="btnReset">Reset</button>
  </div>
</header>

<div id="banner" class="banner">Waiting for data...</div>

<div class="grid">
  <div class="card c3">
    <h2><span><span class="heart" id="heart">&#9829;</span>ECG heart rate</span><span id="zone" class="chip">--</span></h2>
    <div class="big mono"><span id="ecg">--</span> <span class="unit">bpm</span></div>
    <canvas id="spk1" class="spk"></canvas>
  </div>
  <div class="card c3">
    <h2><span>Finger pulse</span><span class="tag">MAX30102</span></h2>
    <div class="big mono"><span id="ppg">--</span> <span class="unit">bpm</span></div>
    <canvas id="spk2" class="spk"></canvas>
  </div>
  <div class="card c3">
    <h2><span>SpO&#8322; (estimate)</span><span id="spoChip" class="chip">--</span></h2>
    <div class="row">
      <div class="ring">
        <svg viewBox="0 0 100 100"><circle cx="50" cy="50" r="42" fill="none" stroke="rgba(148,163,184,.15)" stroke-width="9"/>
          <circle id="ringArc" cx="50" cy="50" r="42" fill="none" stroke="var(--spo2)" stroke-width="9" stroke-linecap="round" stroke-dasharray="264" stroke-dashoffset="264" style="transition:stroke-dashoffset .6s,stroke .4s"/></svg>
        <div class="num mono"><span id="spo2">--</span></div>
      </div>
      <div class="sub" id="spoNote">Place a fingertip gently on the sensor.</div>
    </div>
  </div>
  <div class="card c3">
    <h2><span>Signal quality</span><span id="qChip" class="chip">--</span></h2>
    <div class="big mono"><span id="qNum">--</span><span class="unit"> / 100</span></div>
    <div class="bar q"><i id="qBar"></i></div>
    <div class="sub" id="agree">ECG vs finger pulse: --</div>
  </div>
</div>

<div class="card" style="margin-bottom:14px">
  <h2><span>Live ECG &middot; Lead I</span><span class="tag">250 Hz &middot; 4 s window &middot; R-peaks marked, RR in ms</span></h2>
  <canvas id="ecgc"></canvas>
</div>

<div class="grid">
  <div class="card c4">
    <h2><span>Heart-rate variability</span><span class="tag" id="hrvN">--</span></h2>
    <div class="metrics">
      <div class="metric"><div class="l">RMSSD</div><div class="v mono"><span id="rmssd">--</span><span class="unit"> ms</span></div></div>
      <div class="metric"><div class="l">SDNN</div><div class="v mono"><span id="sdnn">--</span><span class="unit"> ms</span></div></div>
      <div class="metric"><div class="l">pNN50</div><div class="v mono"><span id="pnn">--</span><span class="unit"> %</span></div></div>
      <div class="metric"><div class="l">Mean RR</div><div class="v mono"><span id="mrr">--</span><span class="unit"> ms</span></div></div>
    </div>
    <div class="l" style="font-size:11px;color:var(--mute);text-transform:uppercase;letter-spacing:.06em">Recovery / relaxation</div>
    <div class="bar"><i id="balBar"></i></div>
    <div class="sub" id="balTxt">Collecting beats...</div>
  </div>
  <div class="card c4">
    <h2><span>Poincar&eacute; plot</span><span class="tag" id="sd">SD1 -- &middot; SD2 --</span></h2>
    <canvas id="poin" class="sq"></canvas>
  </div>
  <div class="card c4">
    <h2><span>Rhythm analysis</span><span id="rhChip" class="chip">--</span></h2>
    <div id="rhTxt" style="font-size:15px;margin-bottom:12px">Collecting beats...</div>
    <h2 style="margin-top:14px"><span>RR tachogram</span><span class="tag">ms between beats</span></h2>
    <canvas id="tach" class="chart" style="height:110px"></canvas>
  </div>
</div>

<div class="grid">
  <div class="card c4">
    <h2><span>Heart-rate trend</span><span class="tag"><span style="color:var(--ecg)">&#9644;</span> ECG &nbsp;<span style="color:var(--heart)">&#9644;</span> pulse</span></h2>
    <canvas id="hrTrend" class="chart"></canvas>
  </div>
  <div class="card c4">
    <h2><span>SpO&#8322; trend</span><span class="tag">%</span></h2>
    <canvas id="spoTrend" class="chart"></canvas>
  </div>
  <div class="card c4">
    <h2><span>Session summary</span><span class="tag" id="sessN">--</span></h2>
    <div class="stat3">
      <div class="metric"><div class="l">Min</div><div class="v mono" id="hrMin">--</div></div>
      <div class="metric"><div class="l">Avg</div><div class="v mono" id="hrAvg">--</div></div>
      <div class="metric"><div class="l">Max</div><div class="v mono" id="hrMax">--</div></div>
    </div>
    <div class="l" style="font-size:11px;color:var(--mute);text-transform:uppercase;letter-spacing:.06em">Time in zones</div>
    <div class="zones"><i id="zLo" style="background:var(--spo2)"></i><i id="zMid" style="background:var(--ok)"></i><i id="zHi" style="background:var(--bad)"></i></div>
    <div class="sub"><span style="color:var(--spo2)">&lt;60</span> &nbsp;<span style="color:var(--ok)">60&ndash;100</span> &nbsp;<span style="color:var(--bad)">&gt;100</span> bpm</div>
  </div>
</div>

<div class="grid">
  <div class="card c8">
    <h2><span>Live analysis</span><span class="tag">updates every second</span></h2>
    <ul class="ins" id="ins"><li>Waiting for data...</li></ul>
  </div>
  <div class="card c4">
    <h2><span>Event log</span></h2>
    <div class="log" id="log"><div>No events yet</div></div>
  </div>
</div>

<div class="foot">Demo project, not a medical device. SpO&#8322; is an uncalibrated estimate. Do not use for diagnosis.</div>
</div>

<script>
const $=id=>document.getElementById(id);
const avg=a=>a.reduce((x,y)=>x+y,0)/a.length;
const median=a=>{const s=[...a].sort((x,y)=>x-y),m=s.length>>1;return s.length%2?s[m]:(s[m-1]+s[m])/2;};
const clamp=(v,a,b)=>Math.max(a,Math.min(b,v));
const SAMPLE_MS=4;                   // 250 Hz

const S={ecg:0,ppg:0,spo2:0,finger:false,leadsOff:true,
  N:1000,buf:new Float32Array(1000).fill(512),head:0,abs:0,peaks:[],
  rr:[],hist:[],log:[],alerts:{},mode:'connecting',t0:Date.now(),lo:300,hi:700};

/* ---------- canvas helpers ---------- */
function sizeCanvas(c){
  const r=window.devicePixelRatio||1,w=Math.round(c.clientWidth*r),h=Math.round(c.clientHeight*r);
  if(c.width!==w||c.height!==h){c.width=w;c.height=h;}
  return [c.width,c.height,r];
}
function lineChart(c,series,o={}){
  const [W,H,r]=sizeCanvas(c),g=c.getContext('2d');g.clearRect(0,0,W,H);
  const lab=o.labels!==false,pl=(lab?34:6)*r,pr=8*r,pt=8*r,pb=8*r;
  const vals=[];series.forEach(s=>s.data.forEach(v=>{if(v>0)vals.push(v);}));
  let mn=o.min,mx=o.max;
  if(mn==null||mx==null){
    if(vals.length){mn=Math.min(...vals);mx=Math.max(...vals);const p=Math.max(2,(mx-mn)*.15);mn-=p;mx+=p;}
    else{mn=0;mx=1;}
  }
  if(mx-mn<1)mx=mn+1;
  g.font=(10*r)+'px sans-serif';g.textBaseline='middle';
  for(let i=0;i<=3;i++){
    const y=pt+(H-pt-pb)*i/3;
    g.strokeStyle='rgba(148,163,184,.12)';g.lineWidth=r;g.beginPath();g.moveTo(pl,y);g.lineTo(W-pr,y);g.stroke();
    if(lab){g.fillStyle='#5b6b88';g.fillText(Math.round(mx-(mx-mn)*i/3),2*r,y);}
  }
  const n=o.n||Math.max(2,...series.map(s=>s.data.length));
  series.forEach(s=>{
    const off=n-s.data.length;
    g.strokeStyle=s.color;g.fillStyle=s.color;g.lineWidth=2*r;g.lineJoin='round';
    g.beginPath();let pen=false;
    s.data.forEach((v,i)=>{
      const x=pl+((off+i)/(n-1))*(W-pl-pr);
      if(!(v>0)){pen=false;return;}
      const y=pt+(H-pt-pb)*(1-(v-mn)/(mx-mn));
      if(pen)g.lineTo(x,y);else{g.moveTo(x,y);pen=true;}
    });
    g.stroke();
    if(s.dots)s.data.forEach((v,i)=>{
      if(!(v>0))return;
      const x=pl+((off+i)/(n-1))*(W-pl-pr),y=pt+(H-pt-pb)*(1-(v-mn)/(mx-mn));
      g.beginPath();g.arc(x,y,2.5*r,0,7);g.fill();
    });
  });
}

/* ---------- ECG drawing ---------- */
function drawEcg(){
  const c=$('ecgc'),[W,H,r]=sizeCanvas(c),g=c.getContext('2d');
  g.clearRect(0,0,W,H);
  const box=W/100;                                   // one small box = 40 ms
  g.lineWidth=r;
  for(let i=0;i<=100;i++){
    g.strokeStyle=i%5?'rgba(34,211,238,.05)':'rgba(34,211,238,.13)';
    g.beginPath();g.moveTo(i*box,0);g.lineTo(i*box,H);g.stroke();
  }
  for(let y=0;y<=H;y+=box){
    const k=Math.round(y/box);
    g.strokeStyle=k%5?'rgba(34,211,238,.05)':'rgba(34,211,238,.13)';
    g.beginPath();g.moveTo(0,y);g.lineTo(W,y);g.stroke();
  }
  let mn=1e9,mx=-1e9;
  for(let i=0;i<S.N;i++){const v=S.buf[i];if(v<mn)mn=v;if(v>mx)mx=v;}
  if(mx-mn<60){const m=(mx+mn)/2;mn=m-30;mx=m+30;}
  S.lo+=(mn-S.lo)*.08;S.hi+=(mx-S.hi)*.08;
  const Y=v=>H-H*.1-((v-S.lo)/(S.hi-S.lo))*H*.8;
  const X=i=>i*W/(S.N-1);

  const off=S.leadsOff;
  g.strokeStyle=off?'#4b5568':'#22d3ee';g.lineWidth=2*r;g.lineJoin='round';
  if(!off){g.shadowColor='rgba(34,211,238,.7)';g.shadowBlur=8*r;}
  g.beginPath();
  for(let i=0;i<S.N;i++){
    const v=S.buf[(S.head+i)%S.N];
    i?g.lineTo(X(i),Y(v)):g.moveTo(X(i),Y(v));
  }
  g.stroke();g.shadowBlur=0;

  if(!off){
    S.peaks=S.peaks.filter(p=>S.N-1-(S.abs-1-p)>=0);
    const pos=S.peaks.map(p=>{const i=S.N-1-(S.abs-1-p);return{i,v:S.buf[((S.head+i)%S.N)]};});
    g.font=(11*r)+'px sans-serif';g.textAlign='center';
    pos.forEach((p,k)=>{
      g.fillStyle='#fb7185';g.beginPath();g.arc(X(p.i),Y(p.v)-10*r,4*r,0,7);g.fill();
      if(k>0){
        const ms=Math.round((S.peaks[k]-S.peaks[k-1])*SAMPLE_MS);
        g.fillStyle='rgba(232,238,249,.75)';
        g.fillText(ms+' ms',(X(pos[k-1].i)+X(p.i))/2,18*r);
        g.strokeStyle='rgba(232,238,249,.25)';g.lineWidth=r;
        g.beginPath();g.moveTo(X(pos[k-1].i),24*r);g.lineTo(X(p.i),24*r);g.stroke();
      }
    });
    g.textAlign='left';
  }else{
    g.fillStyle='#8b9bb8';g.font=(15*r)+'px sans-serif';g.fillText('Electrodes not connected',14*r,28*r);
  }
  requestAnimationFrame(drawEcg);
}

/* ---------- data in ---------- */
function pushSamples(arr){
  for(const v of arr){S.buf[S.head]=v;S.head=(S.head+1)%S.N;S.abs++;}
}
function markPeak(){
  let best=-1,bv=-1;
  for(let k=1;k<=40&&k<=S.abs;k++){
    const v=S.buf[(S.head-k+S.N)%S.N];
    if(v>bv){bv=v;best=k;}
  }
  if(best>0)S.peaks.push(S.abs-best);
}
function onR(ms){
  // The device sends "R:" the moment it sees the beat, slightly before the "E:" batch that holds
  // the peak itself. Wait a moment so the highest sample in the window is the R-peak, not the P-wave.
  setTimeout(markPeak,100);
  if(ms>0){S.rr.push({t:Date.now(),v:ms});if(S.rr.length>300)S.rr.shift();}
  const h=$('heart');h.classList.remove('pulse');void h.offsetWidth;h.classList.add('pulse');
}
function onVitals(e,p,s,f,lo){
  S.ecg=e;S.ppg=p;S.spo2=s;S.finger=!!f;S.leadsOff=lo===1;
  $('ecg').textContent=e>0?e:'--';
  $('ppg').textContent=p>0?p:'--';
  $('spo2').textContent=s>0?s:'--';
  const arc=$('ringArc');
  if(s>0){
    arc.style.strokeDashoffset=264*(1-clamp((s-80)/20,0,1));
    arc.style.stroke=s>=95?'var(--ok)':s>=92?'var(--warn)':'var(--bad)';
  }else arc.style.strokeDashoffset=264;
  const chip=$('spoChip');
  if(s>0){chip.textContent=s>=95?'Normal':s>=92?'Borderline':'Low';chip.className='chip '+(s>=95?'ok':s>=92?'warn':'bad');}
  else{chip.textContent='--';chip.className='chip';}
  $('spoNote').textContent=s>0?'Estimated from red/IR light absorption.':(S.finger?'Measuring... hold still for a few seconds.':'Place a fingertip gently on the sensor.');
}

/* ---------- analysis ---------- */
function hrv(){
  const w=S.rr.slice(-40).map(x=>x.v);
  if(w.length<8)return null;
  const med=median(w),c=w.filter(v=>Math.abs(v-med)<=.25*med);
  if(c.length<6)return null;
  const mean=avg(c),sdnn=Math.sqrt(avg(c.map(v=>(v-mean)**2)));
  let d2=0,n50=0;
  for(let i=1;i<c.length;i++){const d=c[i]-c[i-1];d2+=d*d;if(Math.abs(d)>50)n50++;}
  const rmssd=Math.sqrt(d2/(c.length-1));
  return{mean,sdnn,rmssd,pnn:100*n50/(c.length-1),n:c.length,
         sd1:rmssd/Math.SQRT2,sd2:Math.sqrt(Math.max(0,2*sdnn*sdnn-.5*rmssd*rmssd))};
}
function rhythm(){
  const w=S.rr.slice(-30).map(x=>x.v);
  if(S.leadsOff||w.length<10)return{s:'wait'};
  const med=median(w);
  let out=0,art=0,n=0;
  for(let i=0;i<w.length;i++){
    const v=w[i];
    if(Math.abs(v-med)<=.2*med){n++;continue;}
    // A missed beat gives about 2x or 3x the usual interval; an extra detection splits one interval in two.
    // Both are detector slips, so they are counted as noise and not as rhythm.
    const k=Math.round(v/med);
    if(k>=2&&Math.abs(v-k*med)<=.2*med){art++;continue;}
    if(v<.8*med&&i+1<w.length&&Math.abs(v+w[i+1]-med)<=.2*med){art+=2;i++;continue;}
    n++;out++;
  }
  if(quality()<45||art>w.length*.25||n<8)return{s:'noisy',out,n,art};
  return{s:out>=Math.max(4,Math.round(n*.25))?'irregular':'regular',out,n,art};
}
function zoneOf(hr){
  if(!(hr>0))return{t:'--',c:'',d:''};
  if(hr<50)return{t:'Low',c:'bad',d:'below 50 bpm (bradycardia range)'};
  if(hr<60)return{t:'Low-normal',c:'warn',d:'below 60 bpm'};
  if(hr<=100)return{t:'Normal',c:'ok',d:'within the normal resting range (60-100 bpm)'};
  if(hr<=120)return{t:'Elevated',c:'warn',d:'above 100 bpm (fine during activity or stress)'};
  return{t:'High',c:'bad',d:'above 120 bpm (tachycardia range)'};
}
function quality(){
  if(S.leadsOff)return 0;
  let mn=1e9,mx=-1e9;
  for(let i=0;i<500;i++){const v=S.buf[(S.head-1-i+S.N)%S.N];if(v<mn)mn=v;if(v>mx)mx=v;}
  const amp=clamp((mx-mn)/150,0,1)*50;
  const w=S.rr.slice(-10).map(x=>x.v);
  let cons=0;
  if(w.length>=5){const m=median(w);cons=(1-w.filter(v=>Math.abs(v-m)>.2*m).length/w.length)*50;}
  return Math.round(amp+cons);
}
function setAlert(key,active,msg){
  if(!!S.alerts[key]===active)return;
  S.alerts[key]=active;
  addLog(active?'bad':'ok',active?msg:msg+' - resolved');
}
function addLog(cls,msg){
  S.log.unshift({t:Date.now(),cls,msg});if(S.log.length>60)S.log.pop();
  $('log').innerHTML=S.log.slice(0,10).map(l=>
    '<div><span class="mono">'+new Date(l.t).toLocaleTimeString([], {hour:'2-digit',minute:'2-digit',second:'2-digit'})+
    '</span><b style="color:'+(l.cls==='bad'?'var(--bad)':'var(--ok)')+'">'+l.msg+'</b></div>').join('');
}

function spark(c,key,color){
  lineChart(c,[{data:S.hist.slice(-60).map(h=>h[key]),color}],{labels:false});
}
function poincare(h){
  const c=$('poin'),[W,H,r]=sizeCanvas(c),g=c.getContext('2d');g.clearRect(0,0,W,H);
  const w=S.rr.slice(-120).map(x=>x.v);
  g.strokeStyle='rgba(148,163,184,.12)';g.lineWidth=r;
  for(let i=1;i<4;i++){g.beginPath();g.moveTo(W*i/4,0);g.lineTo(W*i/4,H);g.stroke();g.beginPath();g.moveTo(0,H*i/4);g.lineTo(W,H*i/4);g.stroke();}
  if(w.length<3){g.fillStyle='#5b6b88';g.font=(12*r)+'px sans-serif';g.fillText('Collecting beats...',12*r,24*r);return;}
  const m=median(w),lo=m-Math.max(120,m*.3),hi=m+Math.max(120,m*.3);
  const P=v=>clamp((v-lo)/(hi-lo),0,1);
  g.strokeStyle='rgba(148,163,184,.3)';g.setLineDash([4*r,4*r]);g.beginPath();g.moveTo(0,H);g.lineTo(W,0);g.stroke();g.setLineDash([]);
  for(let i=0;i<w.length-1;i++){
    const last=i===w.length-2;
    g.fillStyle=last?'#22d3ee':'rgba(167,139,250,.55)';
    g.beginPath();g.arc(P(w[i])*W,H-P(w[i+1])*H,(last?5:3.2)*r,0,7);g.fill();
  }
  g.fillStyle='#5b6b88';g.font=(10*r)+'px sans-serif';
  g.fillText('RR n (ms)',W-62*r,H-6*r);g.fillText('RR n+1',6*r,12*r);
}

function analyse(){
  const sec=Math.round((Date.now()-S.t0)/1000);
  $('timer').textContent=String(Math.floor(sec/60)).padStart(2,'0')+':'+String(sec%60).padStart(2,'0');

  const hr=S.ecg>0?S.ecg:S.ppg;
  S.hist.push({t:Date.now(),e:S.ecg,p:S.ppg,s:S.spo2});
  if(S.hist.length>1800)S.hist.shift();

  const z=zoneOf(hr);$('zone').textContent=z.t;$('zone').className='chip '+z.c;

  spark($('spk1'),'e','#22d3ee');spark($('spk2'),'p','#fb7185');

  const H=hrv();
  if(H){
    $('rmssd').textContent=Math.round(H.rmssd);$('sdnn').textContent=Math.round(H.sdnn);
    $('pnn').textContent=Math.round(H.pnn);$('mrr').textContent=Math.round(H.mean);
    $('hrvN').textContent=H.n+' beats';
    $('sd').innerHTML='SD1 '+H.sd1.toFixed(0)+' &middot; SD2 '+H.sd2.toFixed(0);
    const bal=clamp(H.rmssd/60,0,1)*100;$('balBar').style.width=bal+'%';
    $('balTxt').textContent=H.rmssd>=50?'High variability: relaxed, good recovery.':
      H.rmssd>=20?'Moderate variability: typical resting state.':'Low variability: stress, fatigue or physical activity.';
  }else{
    ['rmssd','sdnn','pnn','mrr'].forEach(k=>$(k).textContent='--');
    $('hrvN').textContent='--';$('sd').textContent='SD1 -- · SD2 --';
    $('balBar').style.width='0';$('balTxt').textContent='Collecting beats...';
  }
  poincare(H);
  lineChart($('tach'),[{data:S.rr.slice(-60).map(x=>x.v),color:'#a78bfa',dots:true}],{});

  const R=rhythm(),rc=$('rhChip');
  if(R.s==='wait'){rc.textContent='--';rc.className='chip';$('rhTxt').textContent='Collecting beats...';}
  else if(R.s==='regular'){rc.textContent='Regular';rc.className='chip ok';$('rhTxt').textContent='Beat-to-beat intervals are consistent. Rhythm looks regular.';}
  else if(R.s==='noisy'){rc.textContent='Unclear';rc.className='chip warn';$('rhTxt').textContent='The signal is too noisy to judge the rhythm (missed or doubled beats). Press the electrodes firmly and stay still.';}
  else{rc.textContent='Irregular';rc.className='chip bad';$('rhTxt').textContent=R.out+' of the last '+R.n+' clean beats deviate more than 20% from the median. Could be natural variation with breathing, or movement and poor electrode contact. Not a diagnosis.';}

  const q=quality();
  $('qNum').textContent=S.leadsOff&&!S.rr.length?'--':q;$('qBar').style.width=q+'%';
  const qc=$('qChip');qc.textContent=S.leadsOff?'No leads':q>=75?'Good':q>=45?'Fair':'Poor';
  qc.className='chip '+(S.leadsOff?'':q>=75?'ok':q>=45?'warn':'bad');
  $('qBar').style.background=q>=75?'var(--ok)':q>=45?'var(--warn)':'var(--bad)';

  let agreeTxt='ECG vs finger pulse: --',agreeOk=null,d=0;
  if(S.ecg>0&&S.ppg>0){d=Math.abs(S.ecg-S.ppg);agreeOk=d<=5;agreeTxt='ECG vs finger pulse: '+(agreeOk?'agree (':'differ (')+d+' bpm)';}
  $('agree').textContent=agreeTxt;

  lineChart($('hrTrend'),[{data:S.hist.slice(-300).map(h=>h.e),color:'#22d3ee'},{data:S.hist.slice(-300).map(h=>h.p),color:'#fb7185'}],{n:300});
  lineChart($('spoTrend'),[{data:S.hist.slice(-300).map(h=>h.s),color:'#60a5fa'}],{n:300,min:85,max:100});

  const hs=S.hist.map(h=>h.e>0?h.e:h.p).filter(v=>v>0);
  if(hs.length){
    $('hrMin').textContent=Math.min(...hs);$('hrAvg').textContent=Math.round(avg(hs));$('hrMax').textContent=Math.max(...hs);
    const lo=hs.filter(v=>v<60).length,hi=hs.filter(v=>v>100).length,mid=hs.length-lo-hi;
    $('zLo').style.width=lo*100/hs.length+'%';$('zMid').style.width=mid*100/hs.length+'%';$('zHi').style.width=hi*100/hs.length+'%';
    $('sessN').textContent=hs.length+' s of data';
  }

  // alerts
  setAlert('high',hr>120,'Heart rate high ('+hr+' bpm)');
  setAlert('low',hr>0&&hr<50,'Heart rate low ('+hr+' bpm)');
  setAlert('spo2',S.spo2>0&&S.spo2<92,'SpO2 low ('+S.spo2+'%)');
  setAlert('irr',R.s==='irregular','Irregular rhythm detected');
  const act=[];
  if(S.alerts.high)act.push('Heart rate high: '+hr+' bpm');
  if(S.alerts.low)act.push('Heart rate low: '+hr+' bpm');
  if(S.alerts.spo2)act.push('SpO2 low: '+S.spo2+'%');
  if(S.alerts.irr)act.push('Irregular rhythm');
  const b=$('banner');
  if(act.length){b.className='banner alert';b.textContent='ALERT: '+act.join(' · ');}
  else if(hr>0||S.spo2>0){b.className='banner good';b.textContent='All readings within normal range';}
  else{b.className='banner';b.textContent=S.leadsOff?'Attach electrodes and place a finger on the sensor':'Waiting for a reading...';}

  // insights
  const ins=[];
  if(hr>0)ins.push([z.c||'ok','Heart rate is '+hr+' bpm, '+z.d+'.']);
  if(H){
    const lvl=H.rmssd>=50?'ok':H.rmssd>=20?'ok':'warn';
    ins.push([lvl,'HRV (RMSSD) is '+Math.round(H.rmssd)+' ms. '+(H.rmssd>=50?'That suggests a relaxed, well-recovered state.':H.rmssd>=20?'That is typical for a resting adult.':'Low values go with stress, tiredness or exertion.')]);
  }
  if(R.s==='regular')ins.push(['ok','Rhythm regularity: consistent beat-to-beat timing.']);
  if(R.s==='noisy')ins.push(['warn','Rhythm cannot be judged yet because the beat detection is noisy.']);
  if(R.s==='irregular')ins.push(['warn','Uneven beat timing in recent clean beats. Stay still and see if it persists; this is not a diagnosis.']);
  if(S.spo2>0)ins.push([S.spo2>=95?'ok':S.spo2>=92?'warn':'bad','SpO2 estimate is '+S.spo2+'%. '+(S.spo2>=95?'Within the normal range.':S.spo2>=92?'Borderline. Re-measure with a steady finger.':'Low. Re-measure; if it stays low, this needs a proper medical device.')]);
  if(agreeOk===true)ins.push(['ok','ECG and finger-pulse readings agree within '+d+' bpm, so both sensors are consistent.']);
  if(agreeOk===false)ins.push(['warn','ECG and finger pulse differ by '+d+' bpm. One sensor probably has poor contact or movement.']);
  if(!S.leadsOff&&q<45&&S.rr.length>3)ins.push(['warn','ECG signal quality is poor. Press the electrodes firmly, keep wires still, and stay relaxed.']);
  if(S.leadsOff)ins.push(['warn','ECG electrodes are not attached or have lost contact.']);
  const hh=S.hist.map(h=>h.e>0?h.e:h.p);
  if(hh.length>=60){
    const a=hh.slice(-30).filter(v=>v>0),bb=hh.slice(-60,-30).filter(v=>v>0);
    if(a.length>10&&bb.length>10){
      const dd=avg(a)-avg(bb);
      if(Math.abs(dd)>=8)ins.push([dd>0?'warn':'ok','Heart rate is '+(dd>0?'rising':'falling')+': average changed by '+Math.round(Math.abs(dd))+' bpm over the last 30 seconds.']);
    }
  }
  $('ins').innerHTML=ins.length?ins.map(i=>'<li class="'+i[0]+'">'+i[1]+'</li>').join(''):'<li>Waiting for data...</li>';
}

/* ---------- connection ---------- */
let ws=null,retry=0,demoTimer=null;
function setMode(m){
  S.mode=m;const p=$('pill');
  p.textContent={live:'Live',demo:'Demo mode',off:'Offline',connecting:'Connecting...'}[m];
  p.className='pill '+({live:'live',demo:'demo',off:'off',connecting:''}[m]);
  $('btnDemo').textContent=m==='demo'?'Stop demo':'Demo';
}
function connect(){
  if(S.mode==='demo')return;
  setMode('connecting');
  try{ws=new WebSocket('ws://'+location.hostname+':81/');}catch(e){setMode('off');return;}
  ws.onopen=()=>{setMode('live');retry=0;addLog('ok','Connected to device');};
  ws.onclose=()=>{if(S.mode==='demo')return;setMode('off');setTimeout(connect,Math.min(5000,500*(++retry)));};
  ws.onmessage=ev=>{
    const t=ev.data;
    if(t.startsWith('E:'))pushSamples(t.slice(2).split(',').map(Number));
    else if(t.startsWith('R:'))onR(+t.slice(2));
    else if(t.startsWith('V:')){const a=t.slice(2).split(',').map(Number);onVitals(a[0],a[1],a[2],a[3],a[4]);}
  };
}
function stopWs(){if(ws){ws.onclose=null;ws.close();ws=null;}}

/* ---------- demo mode: synthetic patient ---------- */
const gauss=(x,s)=>Math.exp(-(x/s)*(x/s));
function tmpl(d){return .10*gauss(d+.16,.025)-.12*gauss(d+.035,.010)+1.0*gauss(d,.011)-.25*gauss(d-.035,.012)+.28*gauss(d-.25,.045);}
function startDemo(){
  stopWs();setMode('demo');resetAll(true);
  const D={t:0,prev:-1,next:.4,last:performance.now(),acc:0,rrs:[],vit:0};
  const nextRR=()=>{const hr=76+14*Math.sin(D.t/45);return 60/hr*(1+.05*Math.sin(2*Math.PI*D.t/4.6))+(Math.random()-.5)*.012;};
  demoTimer=setInterval(()=>{
    const now=performance.now();
    const n=Math.floor((now-D.last)/SAMPLE_MS);if(n<=0)return;D.last+=n*SAMPLE_MS;
    const out=[];
    for(let i=0;i<n;i++){
      D.t+=SAMPLE_MS/1000;
      let beatNow=false;
      if(D.t>=D.next){D.prev=D.next;const rr=nextRR();D.next=D.prev+rr;D.rrs.push(rr);if(D.rrs.length>6)D.rrs.shift();beatNow=true;}
      const v=tmpl(D.t-D.prev)+tmpl(D.t-D.next)+.02*Math.sin(D.t*.9);
      out.push(Math.round(470+300*v+(Math.random()-.5)*7));
      if(beatNow&&D.rrs.length>1)D.pending=Math.round(D.rrs[D.rrs.length-2]*1000);
    }
    pushSamples(out);
    if(D.pending!=null){onR(D.pending);D.pending=null;}
    if(now-D.vit>500){
      D.vit=now;
      const hr=D.rrs.length?Math.round(60/avg(D.rrs.slice(-4))):0;
      onVitals(hr,hr+Math.round((Math.random()-.5)*3),97+Math.round((Math.random()-.5)*2),1,0);
    }
  },40);
}
function stopDemo(){clearInterval(demoTimer);demoTimer=null;}

/* ---------- controls ---------- */
function resetAll(keepMode){
  S.buf.fill(512);S.peaks=[];S.rr=[];S.hist=[];S.log=[];S.alerts={};S.t0=Date.now();S.abs=0;
  S.ecg=S.ppg=S.spo2=0;$('log').innerHTML='<div>No events yet</div>';
  ['ecg','ppg','spo2'].forEach(k=>$(k).textContent='--');
}
$('btnReset').onclick=()=>resetAll();
$('btnDemo').onclick=()=>{
  if(S.mode==='demo'){stopDemo();resetAll();if(location.protocol==='file:'||location.hash==='#demo'){setMode('off');}else{S.mode='off';connect();}}
  else startDemo();
};
$('btnCsv').onclick=()=>{
  let csv='time,ecg_bpm,pulse_bpm,spo2\n'+S.hist.map(h=>new Date(h.t).toISOString()+','+h.e+','+h.p+','+h.s).join('\n');
  csv+='\n\nrr_time,rr_ms\n'+S.rr.map(r=>new Date(r.t).toISOString()+','+r.v).join('\n');
  const a=document.createElement('a');a.href=URL.createObjectURL(new Blob([csv],{type:'text/csv'}));
  a.download='pulsenet-session.csv';a.click();
};

requestAnimationFrame(drawEcg);
setInterval(analyse,1000);
if(location.protocol==='file:'||location.hash==='#demo')startDemo();else connect();
</script>
</body>
</html>
)rawliteral";
