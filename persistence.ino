#define SSID_LENGTH 32
#define PSK_LENGTH 64

#include <EEPROM.h>

char ee_ssid[SSID_LENGTH];
char ee_psk[PSK_LENGTH];
char * eeprom_psk() { return ee_psk; }
char * eeprom_ssid() { return ee_ssid; }
void eeprom_load_wifi() {
  EEPROM.begin(512);
  for (int i = 0; i < SSID_LENGTH; ++i) ee_ssid[i] = char(EEPROM.read(i));
  Serial.print("SSID (EEPROM): ");
  Serial.println(String(ee_ssid));
  for (int i = 0; i < PSK_LENGTH; ++i) ee_psk[i] = char(EEPROM.read(SSID_LENGTH+ i));
  Serial.print("PSK (EEPROM): ");
  Serial.println(String(ee_psk));  
}
void eeprom_save_wifi(String in_ssid, String in_psk) {
  EEPROM.begin(512);
  if (in_ssid.length() > 0 && in_ssid.length() > 0) {
    for (int i = 0; i < (SSID_LENGTH+PSK_LENGTH); ++i) EEPROM.write(i, 0);
    for (int i = 0; i < in_ssid.length(); ++i) EEPROM.write(i, in_ssid[i]);
    for (int i = 0; i < in_psk.length(); ++i) EEPROM.write(SSID_LENGTH+i, in_psk[i]);
    EEPROM.commit();
  }
}

