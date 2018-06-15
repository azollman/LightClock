#define FLASH_LENGTH 500
#define FLASH_DELAY 1000

void setupWiFiAP() {
  WiFi.softAP(WIFI_AP_SSID);
  Serial.print("AP Mode (");
  Serial.print(String(WIFI_AP_SSID));
  Serial.print(") IP: ");
  Serial.println(WiFi.softAPIP());
  setcolor(MODE_ON);
}
void setupWiFi() {
  WiFi.mode(WIFI_STA);
  eeprom_load_wifi();
  WiFi.begin(eeprom_ssid(), eeprom_psk());
  Serial.print("Connecting to ");
  Serial.print(eeprom_ssid());
  int clicks = 0;
  while (WiFi.status() != WL_CONNECTED && clicks<20) {
    clicks++;
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("\nConnected. IP: ");
    Serial.println(WiFi.localIP());
    FlashLastOctet(WiFi.localIP());
    setupTime();
  } else { //Couldn't connect to WIFI_SSID, instantiating local AP
    Serial.println();
    setupWiFiAP();
  }
}

void FlashLastOctet(IPAddress IP) {
  int lastoctet = IP[3];
  Serial.print("Flashing IP: ");
  Serial.print(lastoctet);
  Serial.print(" = ");
  int hundreds = lastoctet / 100;
  Serial.print(hundreds);
  Serial.print("*100 + ");
  int tens = ( lastoctet - (hundreds * 100))/10;
  Serial.print("*10 + ");
  int ones = lastoctet % 10;
  Serial.println(ones);
  for(int i=0;i<hundreds;i++) {
    setcolor(MODE_RED);
    delay(FLASH_LENGTH);
    setcolor(MODE_OFF);
    delay(FLASH_LENGTH);
  }
  if (hundreds > 0) delay(FLASH_DELAY - FLASH_LENGTH);
  for(int i=0;i<tens;i++) {
    setcolor(MODE_YELLOW);
    delay(FLASH_LENGTH);
    setcolor(MODE_OFF);
    delay(FLASH_LENGTH);
  }
  if (tens >0) delay(FLASH_DELAY - FLASH_LENGTH);
  for(int i=0;i<ones;i++) {
    setcolor(MODE_GREEN);
    delay(FLASH_LENGTH);
    setcolor(MODE_OFF);
    delay(FLASH_LENGTH);
  }
}
