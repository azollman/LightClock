
#define WIFI_SSID "YOUR_SSID" //SSID for default AP connection
#define WIFI_PSK "YOUR_PSK" //PSK to use for both AP-connected and SoftAP
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

#include <FS.h>

#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>


//Internal state assignment modes. No need to touch.
#define MODE_WIFI_STA 0
#define MODE_WIFI_AP 1
#define MODE_SETUP 255
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
int wifiMode;
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
  if (wifiMode == MODE_WIFI_AP) { 
    s.replace("@@BOOTSTRAP@@","/bootstrap.min.css"); 
  } else {
    s.replace("@@BOOTSTRAP@@","https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css");
  }
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
void manual() {
  int led;
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
  File f = SPIFFS.open("/bootstrap.min.css", "r");
  if (f) {
    server.streamFile(f,"text/css");
    f.close();
  } else {
    Serial.println("file open failed");
  }
}
void setup() {
  pinMode(PIN_SETUP, INPUT_PULLUP);
  pinMode(pins[MODE_RED], OUTPUT);
  pinMode(pins[MODE_YELLOW], OUTPUT);
  pinMode(pins[MODE_GREEN], OUTPUT);

  Serial.begin(115200);

  Serial.print("Starting...\n");
if (SPIFFS.begin()) {
  Serial.println("SPIFFS FS mounted.");
}
  setcolor(MODE_OFF);
  if (digitalRead(PIN_SETUP) == 1) {
    setupWiFiSTA();
    setupTime();
  } else {
    setupWiFiAP();
  }
  server.on("/", renderPage);
  server.on("/manual", manual);
  server.on("/alarm", alarm);
  server.on("/time", timeset);
  server.on("/bootstrap.min.css", renderbootstrap);
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
    /*if (minute() % 3 == 0) {
      digitalWrite(LED_GREEN,0);
      } else {
      digitalWrite(LED_GREEN,1);
      }*/
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

void setupWiFiAP() {
  WiFi.softAP(WIFI_AP_SSID,WIFI_PSK);
  Serial.print("AP Mode. IP: ");
  Serial.println(WiFi.softAPIP());
  wifiMode = MODE_WIFI_AP;
}
void setupWiFiSTA() {
  WiFi.mode(WIFI_STA);
  char * ssid = WIFI_SSID;
  char * psk = WIFI_PSK;
  WiFi.begin(ssid, psk);
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  int clicks = 0;
  while (WiFi.status() != WL_CONNECTED && clicks<20) {
    clicks++;
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("\nConnected. IP: ");
    Serial.println(WiFi.localIP());
    wifiMode = MODE_WIFI_STA;
    FlashLastOctet(WiFi.localIP());
  } else { //Couldn't connect to WIFI_SSID, instantiating local AP
    Serial.println();
    setupWiFiAP();
    
    digitalWrite(pins[MODE_RED], LAMP_ON);
    digitalWrite(pins[MODE_YELLOW], LAMP_ON);
    digitalWrite(pins[MODE_GREEN], LAMP_ON);
  }
}

