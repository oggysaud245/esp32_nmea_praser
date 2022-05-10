/* ESP32 ARDUINO NMEA SENTENCES PHRASER
 * This file is capable of reading GPS sensor/module interfaced with esp32, well tested for arduino platform. 
 * The RX pin of GPS is connected with GPIO 4 and TX of GPS with GPIO 2 of esp32.
 * For serial communication the hardware serial1 is used.
 * Author: https://github.com/oggysaud245
 * Version: 0.1.0
 */
 
#define gpsSerial Serial1

#define GPSTAG1 "$GNRMC"
#define GPSTAG2 "$GNGGA"
#define GPSTAG3 "$GNGLL"

#define RX 2
#define TX 4

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial)
    delay(10);
  gpsSerial.begin(115200, SERIAL_8N1, RX, TX);
}

void loop() {
  String b = readGPS();           // read NMEA Sentences

  if (b.substring(0, 6) == GPSTAG1 || b.substring(0, 6) == GPSTAG2 || b.substring(0, 6) == GPSTAG3) { // print only $GNRMC, $GNGGA and $GNGLL nmea sentences
    Serial.println(b);
  }
  //delay(10);
}
String readGPS() {        // read raw data from serial
  static String _rawData;
  while (gpsSerial.available()) {
    clearGPS();         //clear serial buffer and ignore garbage data
    _rawData = gpsSerial.readStringUntil('\r');     // read new data until a carriage return occurs "almost all nmea sentences had CR at end"
  }
  return correctNMEA(_rawData);                  //corrent data sometime multiple nmea identifier may occur in same line for example $GNGGA$GNRMC,XX,XX,, ignore first and extract latest
}
void clearGPS() {  //function to clear serial buffer
  while (gpsSerial.available()) {
    gpsSerial.readStringUntil('\n');
  }
  while (gpsSerial.available()) {
    char c = (char)gpsSerial.read();
    if (c == '\r') {
      break;
    }
  }
}
String correctNMEA(String nmea) { // function to correct nmea sentences
  int len = nmea.length();
  static String validData;
  for ( int i = 0; i < len; i++) {
    if (nmea[i] == '$')
      validData = "";
    validData += nmea[i];     // new data will start form $, if multiple $ occurs only append from latest $
  }
  return validData;
}
