#define WIFI_AP_SSID "LightClock" //SSID to use when Clock is its own AP

#define TZOFFSET -5 //Default Offset. Can be adjusted through UI.

#define ALARM_RED 0 //0 = Alarm clock turns green at time, off otherwise. 1 = Alarm is red until turns green.

#define PIN_RED D5
#define PIN_YELLOW D6
#define PIN_GREEN D7
#define PIN_SETUP D1

//Configure for active low (LAMP_ON = 1, LAMP_OFF = 0) or active high (LAMP_ON = 1, LAMP_OFF = 0)
#define LAMP_ON 0
#define LAMP_OFF 1
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

int tzOffset = TZOFFSET;

int alarmTime = 999999;
int clockMode;
int currentLED;
int pins[4] = {0, PIN_RED, PIN_YELLOW, PIN_GREEN};

ESP8266WebServer server (80);

void setcolor(int led) {
  if (led == MODE_RED) {
    Serial.print("RED\n");
    currentLED = MODE_RED;
    digitalWrite(pins[MODE_RED], LAMP_ON);
    digitalWrite(pins[MODE_YELLOW], LAMP_OFF);
    digitalWrite(pins[MODE_GREEN], LAMP_OFF);
  }
  if (led == MODE_GREEN) {
    Serial.print("GREEN\n");
    currentLED = MODE_GREEN;
    digitalWrite(pins[MODE_RED], LAMP_OFF);
    digitalWrite(pins[MODE_YELLOW], LAMP_OFF);
    digitalWrite(pins[MODE_GREEN], LAMP_ON);
  }
  if (led == MODE_YELLOW) {
    Serial.print("YELLOW\n");
    currentLED = MODE_YELLOW;
    digitalWrite(pins[MODE_RED], LAMP_OFF);
    digitalWrite(pins[MODE_YELLOW], LAMP_ON);
    digitalWrite(pins[MODE_GREEN], LAMP_OFF);
  }
  if (led == MODE_OFF) {
    Serial.print("OFF\n");
    currentLED = MODE_OFF;
    digitalWrite(pins[MODE_RED], LAMP_OFF);
    digitalWrite(pins[MODE_YELLOW], LAMP_OFF);
    digitalWrite(pins[MODE_GREEN], LAMP_OFF);
  }
  if (led == MODE_ON) {
    Serial.print("ON\n");
    digitalWrite(pins[MODE_RED], LAMP_ON);
    digitalWrite(pins[MODE_YELLOW], LAMP_ON);
    digitalWrite(pins[MODE_GREEN], LAMP_ON);
  }

}
void renderPage() {
  String s = MAIN_page;
  switch (currentLED) {
    case MODE_OFF:
      s.replace("@@CURRENTCOLOR@@","");
      break;
    case MODE_RED:  
      s.replace("@@CURRENTCOLOR@@","alert alert-danger");
      break;
    case MODE_YELLOW:  
      s.replace("@@CURRENTCOLOR@@","alert alert-warning");
      break;
    case MODE_GREEN:  
      s.replace("@@CURRENTCOLOR@@","alert alert-success");
      break;
  }
  switch (clockMode) {
    case MODE_OFF:
      s.replace("@@CURRENT@@","");
      break;
    case MODE_TIMER:
      s.replace("@@CURRENT@@",String(alarmTime));
      break;
    default:
      s.replace("@@CURRENT@@","999999");
  }
  
  s.replace("@@YEAR@@", String(year()));
  s.replace("@@MONTH@@", String(month()));
  s.replace("@@DAY@@", String(day()));
  s.replace("@@HOUR@@", String(hour()));
  s.replace("@@MINUTE@@", String(minute()));
  s.replace("@@SECOND@@", String(second()));
  s.replace("@@ALARM@@", String(alarmTime));
  s.replace("@@RESPONSE@@", String(clockMode));
  s.replace("@@TZOFFSET@@", String(tzOffset));
  s.replace("@@SSID@@", String(eeprom_ssid()));
  server.send(200, "text/html", s);
}
void timeset() {
  if (server.arg("set") == "Set TZ") {
  tzOffset = server.arg("offset").toInt();
  setupTime();
  Serial.print("Set TZ offset to: ");
  Serial.println(String(tzOffset));
  } else{
  Serial.println("Manually set time from client.");
  setTime(server.arg("hour").toInt(), server.arg("minute").toInt(), server.arg("second").toInt(), server.arg("day").toInt(), server.arg("month").toInt(), server.arg("year").toInt());
  setSyncInterval(99999999);
  }
  renderPage();
}
void savewifi() {
  eeprom_save_wifi(server.arg("ssid"),server.arg("psk"));
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
  renderPage();
}
void alarm() {
  clockMode = MODE_TIMER;
  if (ALARM_RED) {
    setcolor(MODE_RED);
  } else {
   setcolor(MODE_OFF);
}
  alarmTime = server.arg("alarm").toInt();
  Serial.print("Alarm set for: ");
  Serial.println(alarmTime);
  renderPage();
}
void renderbootstrap() {
server.sendHeader("Cache-Control","max-age=86400");
server.send_P(200,"text/css",BOOTSTRAP_css);
}
void setup() {
  pinMode(PIN_SETUP, INPUT_PULLUP);
  pinMode(pins[MODE_RED], OUTPUT);
  pinMode(pins[MODE_YELLOW], OUTPUT);
  pinMode(pins[MODE_GREEN], OUTPUT);

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
    if (ts == alarmTime) {
      setcolor(MODE_GREEN);
    }
  }
}

