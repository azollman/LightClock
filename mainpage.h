//Uses Bootstrap for style, served from CDN or direct onboard
const char MAIN_page[] PROGMEM = R"=====(
<html>
<head>
<link href="@@BOOTSTRAP@@" rel="stylesheet">
<meta name="viewport" content="width=device-width, initial-scale=1">
<script>
function setclock() {
  var d = new Date();
  document.getElementById("year").value = d.getFullYear();
  document.getElementById("month").value = d.getMonth()+1;
  document.getElementById("day").value = d.getDate();
  document.getElementById("hour").value = d.getHours();
  document.getElementById("minute").value = d.getMinutes();
  document.getElementById("second").value = d.getSeconds();
};
function renderTS() {
  var ts = parseInt(document.getElementById("alarm").value);
  if (ts == 999999) {
    document.getElementById("alarm").value = "";
  } else {
    var seconds = ts %60;
    var hours = Math.floor(ts/(60*60));
    var minutes = Math.floor((ts - hours*60*60)/60);
    document.getElementById("alarm").value = hours+":"+minutes+":"+seconds;
  }
  
  var ts = document.getElementById("current").innerHTML;
  if (ts == "") {
    document.getElementById("current").visible = false;
  } else if (ts == "999999") {
    document.getElementById("current").innerHTML = "&nbsp;";
  } else {
    ts = parseInt(ts);
    var seconds = ts %60;
    var hours = Math.floor(ts/(60*60));
    var minutes = Math.floor((ts - hours*60*60)/60);
    document.getElementById("current").innerHTML = hours+":"+minutes+":"+seconds;
  }
};
function dumpTS() {
  try {
    var ts = document.getElementById("alarm").value.split(":");
    if (ts.length==2) {
      document.getElementById("alarm").value = ts[0]*60*60+ts[1]*60;
    } else if (ts.length==3) {
      document.getElementById("alarm").value = ts[0]*60*60+ts[1]*60+ts[2];
    } else {throw true;}
  } catch (e) {
    document.getElementById("alarm").value = "999999";
  }
}
</script>
</head>
<body class="container" onload="renderTS()">
<h1 class="text-center">Sleepy-Time Clock</h1>
<div class="@@CURRENTCOLOR@@" id="current">@@CURRENT@@</div>
<form method="post" action="/manual">
<div class="well text-center">
<div class="btn-group">
<p><input type="submit" name="m" value="OFF" class="btn btn-block btn-lg btn-default"></p>
<p><input type="submit" name="m" value="RED" class="btn btn-block btn-lg  btn-danger"></p>
<p><input type="submit" name="m" value="YELLOW" class="btn btn-block btn-lg  btn-warning"></p>
<p><input type="submit" name="m" value="GREEN" class="btn btn-block btn-lg  btn-success"></p>
</div>
</div>
</form>

<form method="post" action="/alarm" onsubmit="return dumpTS();">
<div class="well text-center">
<h2>Alarm Clock</h2>
<p><input type="text" id="alarm" name="alarm" value="@@ALARM@@"></p>
<p><input type="submit" name="m" value="Set!" class="btn btn-med btn-default"></p>
</div>
</form>

<form method="post" onsubmit="return setclock();" action="/time">
<div class="well text-center">
<h3>Clock Time</h3>
<div id="currentTime">
@@YEAR@@-@@MONTH@@-@@DAY@@ @@HOUR@@:@@MINUTE@@:@@SECOND@@
</div>
<div id="sync">
<input id="year" type="hidden" name="year">
<input id="month" type="hidden" name="month">
<input id="day" type="hidden" name="day">
<input id="hour" type="hidden" name="hour">
<input id="minute" type="hidden" name="minute">
<input id="second" type="hidden" name="second">
<input onclick="setclock();" type="submit" name="set" value="Sync to Device" class="btn btn-default">
</div>
<div id="tzset">
<input id="offset" type="text" size="3" name="offset" value="@@TZOFFSET@@"><input type="submit" name="set" value="Set TZ" class="btn btn-default">
</div>
</div>
</form>

</body>
</html>
)=====";

