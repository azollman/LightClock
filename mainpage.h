//Uses Bootstrap for style, served from CDN or direct onboard
const char MAIN_page[] PROGMEM = R"=====(
<html>
<head>
<link href="/style.css" rel="stylesheet">
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

const char BOOTSTRAP_css[] PROGMEM = R"=====(
/*!
 * Bootstrap v3.3.7 (http://getbootstrap.com)
 * Copyright 2011-2018 Twitter, Inc.
 * Licensed under MIT (https://github.com/twbs/bootstrap/blob/master/LICENSE)
 *//*!
 * Generated using the Bootstrap Customizer (<none>)
 * Config saved to config.json and <none>
 *//*!
 * Bootstrap v3.3.7 (http://getbootstrap.com)
 * Copyright 2011-2016 Twitter, Inc.
 * Licensed under MIT (https://github.com/twbs/bootstrap/blob/master/LICENSE)
 *//*! normalize.css v3.0.3 | MIT License | github.com/necolas/normalize.css */hr,legend{border:0}legend,td,th{padding:0}.btn,.btn-danger.active,.btn-danger:active,.btn-default.active,.btn-default:active,.btn-info.active,.btn-info:active,.btn-primary.active,.btn-primary:active,.btn-success.active,.btn-success:active,.btn-warning.active,.btn-warning:active,.open>.dropdown-toggle.btn-danger,.open>.dropdown-toggle.btn-default,.open>.dropdown-toggle.btn-info,.open>.dropdown-toggle.btn-primary,.open>.dropdown-toggle.btn-success,.open>.dropdown-toggle.btn-warning{background-image:none}html{font-family:sans-serif;-ms-text-size-adjust:100%;-webkit-text-size-adjust:100%}article,aside,details,figcaption,figure,footer,header,hgroup,main,menu,nav,section,summary{display:block}a{background-color:transparent;color:#337ab7;text-decoration:none}a:active,a:hover{outline:0}b,optgroup,strong{font-weight:700}h1{margin:.67em 0}hr{-webkit-box-sizing:content-box;-moz-box-sizing:content-box;box-sizing:content-box;height:0;margin-top:20px;margin-bottom:20px;border-top:1px solid #eee}button,input,optgroup,select,textarea{color:inherit;font:inherit;margin:0}button{overflow:visible}button,select{text-transform:none}button,html input[type=button],input[type=reset],input[type=submit]{-webkit-appearance:button;cursor:pointer}button[disabled],html input[disabled]{cursor:default}.btn,[role=button]{cursor:pointer}button::-moz-focus-inner,input::-moz-focus-inner{border:0;padding:0}textarea{overflow:auto}table{border-collapse:collapse;border-spacing:0}*,:after,:before{-webkit-box-sizing:border-box;-moz-box-sizing:border-box;box-sizing:border-box}html{font-size:10px;-webkit-tap-highlight-color:transparent}body{margin:0;font-family:"Helvetica Neue",Helvetica,Arial,sans-serif;font-size:14px;line-height:1.42857143;color:#333;background-color:#fff}button,input,select,textarea{font-family:inherit;font-size:inherit;line-height:inherit}a:focus,a:hover{color:#23527c;text-decoration:underline}a:focus{outline:-webkit-focus-ring-color auto 5px;outline-offset:-2px}.h1,.h2,.h3,.h4,.h5,.h6,h1,h2,h3,h4,h5,h6{font-family:inherit;font-weight:500;line-height:1.1;color:inherit}.h1,.h2,.h3,h1,h2,h3{margin-top:20px;margin-bottom:10px}.h4,.h5,.h6,h4,h5,h6{margin-top:10px;margin-bottom:10px}.h1,h1{font-size:36px}.h2,h2{font-size:30px}.h3,h3{font-size:24px}.h4,h4{font-size:18px}.h5,h5{font-size:14px}.h6,h6{font-size:12px}p{margin:0 0 10px}.container,.container-fluid{margin-right:auto;margin-left:auto;padding-left:15px;padding-right:15px}.lead{margin-bottom:20px;font-size:16px;font-weight:300;line-height:1.4}@media (min-width:768px){.lead{font-size:21px}.container{width:750px}}.small,small{font-size:85%}.text-center{text-align:center}.text-primary{color:#337ab7}.text-success{color:#3c763d}.text-info{color:#31708f}.text-warning{color:#8a6d3b}.text-danger{color:#a94442}.bg-primary{color:#fff;background-color:#337ab7}.bg-success{background-color:#dff0d8}.bg-info{background-color:#d9edf7}.bg-warning{background-color:#fcf8e3}.bg-danger{background-color:#f2dede}label{display:inline-block;max-width:100%;margin-bottom:5px;font-weight:700}.btn,.btn-link{font-weight:400}select[multiple],select[size]{height:auto}@media screen and (-webkit-min-device-pixel-ratio:0){input[type=date].form-control,input[type=time].form-control,input[type=datetime-local].form-control,input[type=month].form-control{line-height:34px}.input-group-sm input[type=date],.input-group-sm input[type=time],.input-group-sm input[type=datetime-local],.input-group-sm input[type=month],input[type=date].input-sm,input[type=time].input-sm,input[type=datetime-local].input-sm,input[type=month].input-sm{line-height:30px}.input-group-lg input[type=date],.input-group-lg input[type=time],.input-group-lg input[type=datetime-local],.input-group-lg input[type=month],input[type=date].input-lg,input[type=time].input-lg,input[type=datetime-local].input-lg,input[type=month].input-lg{line-height:46px}}@media (min-width:768px){.col-sm-1,.col-sm-10,.col-sm-11,.col-sm-12,.col-sm-2,.col-sm-3,.col-sm-4,.col-sm-5,.col-sm-6,.col-sm-7,.col-sm-8,.col-sm-9{float:left}.col-sm-12{width:100%}.col-sm-11{width:91.66666667%}.col-sm-10{width:83.33333333%}.col-sm-9{width:75%}.col-sm-8{width:66.66666667%}.col-sm-7{width:58.33333333%}.col-sm-6{width:50%}.col-sm-5{width:41.66666667%}.col-sm-4{width:33.33333333%}.col-sm-3{width:25%}.col-sm-2{width:16.66666667%}.col-sm-1{width:8.33333333%}.col-sm-pull-12{right:100%}.col-sm-pull-11{right:91.66666667%}.col-sm-pull-10{right:83.33333333%}.col-sm-pull-9{right:75%}.col-sm-pull-8{right:66.66666667%}.col-sm-pull-7{right:58.33333333%}.col-sm-pull-6{right:50%}.col-sm-pull-5{right:41.66666667%}.col-sm-pull-4{right:33.33333333%}.col-sm-pull-3{right:25%}.col-sm-pull-2{right:16.66666667%}.col-sm-pull-1{right:8.33333333%}.col-sm-pull-0{right:auto}.col-sm-push-12{left:100%}.col-sm-push-11{left:91.66666667%}.col-sm-push-10{left:83.33333333%}.col-sm-push-9{left:75%}.col-sm-push-8{left:66.66666667%}.col-sm-push-7{left:58.33333333%}.col-sm-push-6{left:50%}.col-sm-push-5{left:41.66666667%}.col-sm-push-4{left:33.33333333%}.col-sm-push-3{left:25%}.col-sm-push-2{left:16.66666667%}.col-sm-push-1{left:8.33333333%}.col-sm-push-0{left:auto}.col-sm-offset-12{margin-left:100%}.col-sm-offset-11{margin-left:91.66666667%}.col-sm-offset-10{margin-left:83.33333333%}.col-sm-offset-9{margin-left:75%}.col-sm-offset-8{margin-left:66.66666667%}.col-sm-offset-7{margin-left:58.33333333%}.col-sm-offset-6{margin-left:50%}.col-sm-offset-5{margin-left:41.66666667%}.col-sm-offset-4{margin-left:33.33333333%}.col-sm-offset-3{margin-left:25%}.col-sm-offset-2{margin-left:16.66666667%}.col-sm-offset-1{margin-left:8.33333333%}.col-sm-offset-0{margin-left:0}.form-inline .form-control-static,.form-inline .form-group{display:inline-block}.form-inline .control-label,.form-inline .form-group{margin-bottom:0;vertical-align:middle}.form-inline .form-control{display:inline-block;width:auto;vertical-align:middle}.form-inline .input-group{display:inline-table;vertical-align:middle}.form-inline .input-group .form-control,.form-inline .input-group .input-group-addon,.form-inline .input-group .input-group-btn{width:auto}.form-inline .input-group>.form-control{width:100%}.form-inline .checkbox,.form-inline .radio{display:inline-block;margin-top:0;margin-bottom:0;vertical-align:middle}.form-inline .checkbox label,.form-inline .radio label{padding-left:0}.form-inline .checkbox input[type=checkbox],.form-inline .radio input[type=radio]{position:relative;margin-left:0}.form-inline .has-feedback .form-control-feedback{top:0}.form-horizontal .control-label{text-align:right;margin-bottom:0;padding-top:7px}}.btn-block,input[type=button].btn-block,input[type=reset].btn-block,input[type=submit].btn-block{width:100%}.form-horizontal .has-feedback .form-control-feedback{right:15px}@media (min-width:768px){.form-horizontal .form-group-lg .control-label{padding-top:11px;font-size:18px}.form-horizontal .form-group-sm .control-label{padding-top:6px;font-size:12px}}.btn{display:inline-block;margin-bottom:0;text-align:center;vertical-align:middle;-ms-touch-action:manipulation;touch-action:manipulation;border:1px solid transparent;white-space:nowrap;padding:6px 12px;font-size:14px;line-height:1.42857143;border-radius:4px;-webkit-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none}.btn.active.focus,.btn.active:focus,.btn.focus,.btn:active.focus,.btn:active:focus,.btn:focus{outline:-webkit-focus-ring-color auto 5px;outline-offset:-2px}.btn.focus,.btn:focus,.btn:hover{color:#333;text-decoration:none}.btn.active,.btn:active{outline:0;background-image:none;-webkit-box-shadow:inset 0 3px 5px rgba(0,0,0,.125);box-shadow:inset 0 3px 5px rgba(0,0,0,.125)}.btn.disabled,.btn[disabled],fieldset[disabled] .btn{cursor:not-allowed;opacity:.65;filter:alpha(opacity=65);-webkit-box-shadow:none;box-shadow:none}a.btn.disabled,fieldset[disabled] a.btn{pointer-events:none}.btn-default{color:#333;background-color:#fff;border-color:#ccc}.btn-default.focus,.btn-default:focus{color:#333;background-color:#e6e6e6;border-color:#8c8c8c}.btn-default.active,.btn-default:active,.btn-default:hover,.open>.dropdown-toggle.btn-default{color:#333;background-color:#e6e6e6;border-color:#adadad}.btn-default.active.focus,.btn-default.active:focus,.btn-default.active:hover,.btn-default:active.focus,.btn-default:active:focus,.btn-default:active:hover,.open>.dropdown-toggle.btn-default.focus,.open>.dropdown-toggle.btn-default:focus,.open>.dropdown-toggle.btn-default:hover{color:#333;background-color:#d4d4d4;border-color:#8c8c8c}.btn-default.disabled.focus,.btn-default.disabled:focus,.btn-default.disabled:hover,.btn-default[disabled].focus,.btn-default[disabled]:focus,.btn-default[disabled]:hover,fieldset[disabled] .btn-default.focus,fieldset[disabled] .btn-default:focus,fieldset[disabled] .btn-default:hover{background-color:#fff;border-color:#ccc}.btn-default .badge{color:#fff;background-color:#333}.btn-primary{color:#fff;background-color:#337ab7;border-color:#2e6da4}.btn-primary.focus,.btn-primary:focus{color:#fff;background-color:#286090;border-color:#122b40}.btn-primary.active,.btn-primary:active,.btn-primary:hover,.open>.dropdown-toggle.btn-primary{color:#fff;background-color:#286090;border-color:#204d74}.btn-primary.active.focus,.btn-primary.active:focus,.btn-primary.active:hover,.btn-primary:active.focus,.btn-primary:active:focus,.btn-primary:active:hover,.open>.dropdown-toggle.btn-primary.focus,.open>.dropdown-toggle.btn-primary:focus,.open>.dropdown-toggle.btn-primary:hover{color:#fff;background-color:#204d74;border-color:#122b40}.btn-primary.disabled.focus,.btn-primary.disabled:focus,.btn-primary.disabled:hover,.btn-primary[disabled].focus,.btn-primary[disabled]:focus,.btn-primary[disabled]:hover,fieldset[disabled] .btn-primary.focus,fieldset[disabled] .btn-primary:focus,fieldset[disabled] .btn-primary:hover{background-color:#337ab7;border-color:#2e6da4}.btn-primary .badge{color:#337ab7;background-color:#fff}.btn-success{color:#fff;background-color:#5cb85c;border-color:#4cae4c}.btn-success.focus,.btn-success:focus{color:#fff;background-color:#449d44;border-color:#255625}.btn-success.active,.btn-success:active,.btn-success:hover,.open>.dropdown-toggle.btn-success{color:#fff;background-color:#449d44;border-color:#398439}.btn-success.active.focus,.btn-success.active:focus,.btn-success.active:hover,.btn-success:active.focus,.btn-success:active:focus,.btn-success:active:hover,.open>.dropdown-toggle.btn-success.focus,.open>.dropdown-toggle.btn-success:focus,.open>.dropdown-toggle.btn-success:hover{color:#fff;background-color:#398439;border-color:#255625}.btn-success.disabled.focus,.btn-success.disabled:focus,.btn-success.disabled:hover,.btn-success[disabled].focus,.btn-success[disabled]:focus,.btn-success[disabled]:hover,fieldset[disabled] .btn-success.focus,fieldset[disabled] .btn-success:focus,fieldset[disabled] .btn-success:hover{background-color:#5cb85c;border-color:#4cae4c}.btn-success .badge{color:#5cb85c;background-color:#fff}.btn-info{color:#fff;background-color:#5bc0de;border-color:#46b8da}.btn-info.focus,.btn-info:focus{color:#fff;background-color:#31b0d5;border-color:#1b6d85}.btn-info.active,.btn-info:active,.btn-info:hover,.open>.dropdown-toggle.btn-info{color:#fff;background-color:#31b0d5;border-color:#269abc}.btn-info.active.focus,.btn-info.active:focus,.btn-info.active:hover,.btn-info:active.focus,.btn-info:active:focus,.btn-info:active:hover,.open>.dropdown-toggle.btn-info.focus,.open>.dropdown-toggle.btn-info:focus,.open>.dropdown-toggle.btn-info:hover{color:#fff;background-color:#269abc;border-color:#1b6d85}.btn-info.disabled.focus,.btn-info.disabled:focus,.btn-info.disabled:hover,.btn-info[disabled].focus,.btn-info[disabled]:focus,.btn-info[disabled]:hover,fieldset[disabled] .btn-info.focus,fieldset[disabled] .btn-info:focus,fieldset[disabled] .btn-info:hover{background-color:#5bc0de;border-color:#46b8da}.btn-info .badge{color:#5bc0de;background-color:#fff}.btn-warning{color:#fff;background-color:#f0ad4e;border-color:#eea236}.btn-warning.focus,.btn-warning:focus{color:#fff;background-color:#ec971f;border-color:#985f0d}.btn-warning.active,.btn-warning:active,.btn-warning:hover,.open>.dropdown-toggle.btn-warning{color:#fff;background-color:#ec971f;border-color:#d58512}.btn-warning.active.focus,.btn-warning.active:focus,.btn-warning.active:hover,.btn-warning:active.focus,.btn-warning:active:focus,.btn-warning:active:hover,.open>.dropdown-toggle.btn-warning.focus,.open>.dropdown-toggle.btn-warning:focus,.open>.dropdown-toggle.btn-warning:hover{color:#fff;background-color:#d58512;border-color:#985f0d}.btn-warning.disabled.focus,.btn-warning.disabled:focus,.btn-warning.disabled:hover,.btn-warning[disabled].focus,.btn-warning[disabled]:focus,.btn-warning[disabled]:hover,fieldset[disabled] .btn-warning.focus,fieldset[disabled] .btn-warning:focus,fieldset[disabled] .btn-warning:hover{background-color:#f0ad4e;border-color:#eea236}.btn-warning .badge{color:#f0ad4e;background-color:#fff}.btn-danger{color:#fff;background-color:#d9534f;border-color:#d43f3a}.btn-danger.focus,.btn-danger:focus{color:#fff;background-color:#c9302c;border-color:#761c19}.btn-danger.active,.btn-danger:active,.btn-danger:hover,.open>.dropdown-toggle.btn-danger{color:#fff;background-color:#c9302c;border-color:#ac2925}.btn-danger.active.focus,.btn-danger.active:focus,.btn-danger.active:hover,.btn-danger:active.focus,.btn-danger:active:focus,.btn-danger:active:hover,.open>.dropdown-toggle.btn-danger.focus,.open>.dropdown-toggle.btn-danger:focus,.open>.dropdown-toggle.btn-danger:hover{color:#fff;background-color:#ac2925;border-color:#761c19}.btn-danger.disabled.focus,.btn-danger.disabled:focus,.btn-danger.disabled:hover,.btn-danger[disabled].focus,.btn-danger[disabled]:focus,.btn-danger[disabled]:hover,fieldset[disabled] .btn-danger.focus,fieldset[disabled] .btn-danger:focus,fieldset[disabled] .btn-danger:hover{background-color:#d9534f;border-color:#d43f3a}.btn-danger .badge{color:#d9534f;background-color:#fff}.btn-link{color:#337ab7;border-radius:0}.btn-link,.btn-link.active,.btn-link:active,.btn-link[disabled],fieldset[disabled] .btn-link{background-color:transparent;-webkit-box-shadow:none;box-shadow:none}.btn-link,.btn-link:active,.btn-link:focus,.btn-link:hover{border-color:transparent}.btn-link:focus,.btn-link:hover{color:#23527c;text-decoration:underline;background-color:transparent}.btn-link[disabled]:focus,.btn-link[disabled]:hover,fieldset[disabled] .btn-link:focus,fieldset[disabled] .btn-link:hover{color:#777;text-decoration:none}.btn-group-lg>.btn,.btn-lg{padding:10px 16px;font-size:18px;line-height:1.3333333;border-radius:6px}.btn-group-sm>.btn,.btn-sm{padding:5px 10px;font-size:12px;line-height:1.5;border-radius:3px}.btn-group-xs>.btn,.btn-xs{padding:1px 5px;font-size:12px;line-height:1.5;border-radius:3px}.btn-block{display:block}.btn-block+.btn-block{margin-top:5px}[data-toggle=buttons]>.btn input[type=radio],[data-toggle=buttons]>.btn input[type=checkbox],[data-toggle=buttons]>.btn-group>.btn input[type=radio],[data-toggle=buttons]>.btn-group>.btn input[type=checkbox]{position:absolute;clip:rect(0,0,0,0);pointer-events:none}.alert{padding:15px;margin-bottom:20px;border:1px solid transparent;border-radius:4px}.alert h4{margin-top:0;color:inherit}.alert .alert-link{font-weight:700}.alert>p,.alert>ul{margin-bottom:0}.alert>p+p{margin-top:5px}.alert-dismissable,.alert-dismissible{padding-right:35px}.alert-dismissable .close,.alert-dismissible .close{position:relative;top:-2px;right:-21px;color:inherit}.alert-success{background-color:#dff0d8;border-color:#d6e9c6;color:#3c763d}.alert-success hr{border-top-color:#c9e2b3}.alert-success .alert-link{color:#2b542c}.alert-info{background-color:#d9edf7;border-color:#bce8f1;color:#31708f}.alert-info hr{border-top-color:#a6e1ec}.alert-info .alert-link{color:#245269}.alert-warning{background-color:#fcf8e3;border-color:#faebcc;color:#8a6d3b}.alert-warning hr{border-top-color:#f7e1b5}.alert-warning .alert-link{color:#66512c}.alert-danger{background-color:#f2dede;border-color:#ebccd1;color:#a94442}.alert-danger hr{border-top-color:#e4b9c0}.alert-danger .alert-link{color:#843534}.well{min-height:20px;padding:19px;margin-bottom:20px;background-color:#f5f5f5;border:1px solid #e3e3e3;border-radius:4px;-webkit-box-shadow:inset 0 1px 1px rgba(0,0,0,.05);box-shadow:inset 0 1px 1px rgba(0,0,0,.05)}.well blockquote{border-color:#ddd;border-color:rgba(0,0,0,.15)}
)=====";
