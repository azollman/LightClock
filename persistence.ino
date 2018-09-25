#define SSID_LENGTH 32
#define PSK_LENGTH 64
#define HOST_LENGTH 32

#include <EEPROM.h>

char ee_ssid[SSID_LENGTH];
char ee_psk[PSK_LENGTH];
short ee_tzOffset;

char * eeprom_psk() { return ee_psk; }
char * eeprom_ssid() { return ee_ssid; }
short eeprom_tzOffset() { return ee_tzOffset; }
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
void eeprom_load_tz() {
  EEPROM.begin(512);
  Serial.print("TZ (EEPROM): ");
  ee_tzOffset = short(EEPROM.read(SSID_LENGTH + PSK_LENGTH)) - 100;
  Serial.println(String(ee_tzOffset));  
}

void eeprom_save_tz(short in_tz) {
  EEPROM.begin(512);
  ee_tzOffset = in_tz;
  EEPROM.write(SSID_LENGTH + PSK_LENGTH,100+ee_tzOffset);
  EEPROM.commit();
}

