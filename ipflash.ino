#define FLASH_LENGTH 500
#define FLASH_DELAY 1000
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
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
