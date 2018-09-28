String getChunk(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

String renderTime(int ts) {
  if (ts == 999999) return "";
  int hours= int(ts/60/60);
  int minutes = int((ts-hours*60*60)/60);
  int seconds = int(ts-hours*60*60-minutes*60);
  String pretty = String(hours);
  if (minutes <10 )  { pretty = pretty+":0"+String(minutes); } else { pretty = pretty+":"+String(minutes); }
  if (seconds == 0) {} else if (seconds <10) {pretty=pretty+":0"+String(seconds);} else {pretty = pretty+":"+String(seconds);}
  return pretty;
}

