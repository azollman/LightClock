#define WIFI_AP_SSID "LightClock" //SSID to use when Clock is its own AP

#define PIN_RED D5
#define PIN_YELLOW D6
#define PIN_GREEN D7
#define PIN_SETUP D1

//PWM Configurations to control brightness (depending on LEDs and resistors) as well as configure active-low vs active-high
#define PWM_RANGE 100
#define PWM_OFF 100
#define PWM_RED 98
#define PWM_YELLOW 90
#define PWM_GREEN 0

//-------------END OF CONFIGURABLES-------------------------------------------------
#include "mainpage.h"
#include <Time.h>
#include <TimeLib.h>

#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>


//Internal state assignment modes. No need to touch.
#define MODE_OFF 0
#define MODE_RED 1
#define MODE_YELLOW 2
#define MODE_GREEN 3
#define MODE_TIMER 100
#define MODE_ON 254

int alarmTime = 999999;
int clockMode;
int currentLED;

ESP8266WebServer server (80);

void setcolor(int led) {
  currentLED = led;
  analogWrite(PIN_RED, PWM_OFF);
  analogWrite(PIN_YELLOW, PWM_OFF);
  analogWrite(PIN_GREEN, PWM_OFF);
  if (led == MODE_RED) {
    Serial.print("RED\n");
    analogWrite(PIN_RED, PWM_RED);
  } else if (led == MODE_GREEN) {
    Serial.print("GREEN\n");
    analogWrite(PIN_GREEN, PWM_GREEN);
  } else if (led == MODE_YELLOW) {
    Serial.print("YELLOW\n");
    analogWrite(PIN_YELLOW, PWM_YELLOW);
  } else if (led == MODE_OFF) {
    Serial.print("OFF\n");
  } else if (led == MODE_ON) {
    currentLED = MODE_OFF;
    Serial.print("ON\n");
    analogWrite(PIN_RED, PWM_RED);
    analogWrite(PIN_YELLOW, PWM_YELLOW);
    analogWrite(PIN_GREEN, PWM_GREEN);
  }
}
void redirect() {
  server.sendHeader("Location", String("/"), true);
  server.send(302, "text/plain", "");
}
void renderPage() {
  String s = MAIN_page;
  if (currentLED == MODE_OFF && clockMode == MODE_TIMER) {
    s.replace("@@CURRENTCOLOR@@", "alert alert-dark text-center");
  } else if (currentLED == MODE_RED) {
    s.replace("@@CURRENTCOLOR@@", "alert alert-danger text-center");
  } else if (currentLED == MODE_YELLOW) {
    s.replace("@@CURRENTCOLOR@@", "alert alert-warning text-center");
  } else if (currentLED == MODE_GREEN) {
    s.replace("@@CURRENTCOLOR@@", "alert alert-success text-center");
  } else {
    s.replace("@@CURRENTCOLOR@@", "");
  }

  switch (clockMode) {
    case MODE_OFF:
      s.replace("@@CURRENT@@", "");
      break;
    case MODE_TIMER:
      s.replace("@@CURRENT@@", String(alarmTime));
      break;
    default:
      s.replace("@@CURRENT@@", "999999");
  }

  s.replace("@@YEAR@@", String(year()));
  s.replace("@@MONTH@@", String(month()));
  s.replace("@@DAY@@", String(day()));
  s.replace("@@HOUR@@", String(hour()));
  s.replace("@@MINUTE@@", String(minute()));
  s.replace("@@SECOND@@", String(second()));
  s.replace("@@ALARM@@", String(alarmTime));
  s.replace("@@RESPONSE@@", String(clockMode));
  s.replace("@@TZOFFSET@@", String(eeprom_tzOffset()));
  s.replace("@@SSID@@", String(eeprom_ssid()));
  server.send(200, "text/html", s);
}
void timeset() {
  if (server.arg("set") == "Set TZ") {
    eeprom_save_tz(short(server.arg("offset").toInt()));
    setupTime();
    Serial.print("Set TZ offset to: ");
    Serial.println(String(eeprom_tzOffset()));
  } else {
    Serial.println("Manually set time from client.");
    setTime(server.arg("hour").toInt(), server.arg("minute").toInt(), server.arg("second").toInt(), server.arg("day").toInt(), server.arg("month").toInt(), server.arg("year").toInt());
    setSyncInterval(99999999);
  }
  redirect();
}
void savewifi() {
  eeprom_save_wifi(server.arg("ssid"), server.arg("psk"));
  setupWiFi();
}
void manual() {
  if (server.arg("m") == "RED") {
    clockMode = MODE_RED;
    setcolor(clockMode);
  }
  if (server.arg("m") == "YELLOW") {
    clockMode = MODE_YELLOW;
    setcolor(clockMode);
  }
  if (server.arg("m") == "GREEN") {
    clockMode = MODE_GREEN;
    setcolor(clockMode);
  }
  if (server.arg("m") == "OFF") {
    clockMode = MODE_OFF;
    setcolor(clockMode);
  }
  if (server.arg("m") == "ON") {
    clockMode = MODE_ON;
    setcolor(MODE_ON);
  }
  redirect();
}
void alarm() {
  clockMode = MODE_TIMER;
  if (server.arg("redlight") == "on") {
    setcolor(MODE_RED);
  } else {
    setcolor(MODE_OFF);
  }
  alarmTime = server.arg("alarm").toInt();
  Serial.print("Alarm set for: ");
  Serial.println(alarmTime);
  redirect();
}
void renderbootstrap() {
  server.sendHeader("Cache-Control", "max-age=86400");
  server.send_P(200, "text/css", BOOTSTRAP_css);
}
void setup() {
  pinMode(PIN_SETUP, INPUT_PULLUP);
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_YELLOW, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  analogWriteRange(PWM_RANGE);
  Serial.begin(115200);

  Serial.print("Starting...\n");
  setcolor(MODE_OFF);
  if (digitalRead(PIN_SETUP) == 1) {
    setupWiFi();
  } else {
    setupWiFiAP();
  }
  server.on("/", renderPage);
  server.on("/manual", manual);
  server.on("/wifi", savewifi);
  server.on("/alarm", alarm);
  server.on("/time", timeset);
  server.on("/style.css", renderbootstrap);
  server.begin();
}
int lastts = 0;
void loop() {
  int ts = hour() * 60 * 60 + minute() * 60 + second();
  server.handleClient();

  if (lastts != ts) {
    lastts = ts;
    Serial.print("Now: ");
    Serial.print(hour());
    Serial.print(":");
    Serial.print(minute());
    Serial.print(":");
    Serial.print(second());
    Serial.print(" (");
    Serial.print(ts);
    Serial.print(")");
    Serial.print("\n");
    if (ts == alarmTime && clockMode == MODE_TIMER) {
      setcolor(MODE_GREEN);
    }
  }
}

