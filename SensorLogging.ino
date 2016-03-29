/*
 *  Collect all sensor data and save them to global variables.
 *  We need all sensor data for logging, regardless of what is displayed on the lcd.
 *  for now only voltage is read. Other sensors are to be installed.
 */

void readAllSensorsDelay(){
  unsigned long currentMillis = millis();
  if(currentMillis - lastRead > readInterval) {
    readAllSensors();
    lastRead = currentMillis;   
  }
}

void readAllSensors() {

  int currentSec = HCRTC.GetSecond();
  //  Serial.println(currentSec);
  //  Serial.println(lastSec);
  //  delay(50);
  //  if (currentSec - lastSec > 3) {

  // save the last time you read the values

  // read the value from volatge divider 1 and calvulate real voltage
  voltBat1 = (analogRead(SENSOR_1_BAT_1) * (5.0 / 1023.0) * 3) - 0.13;

  // read the value from volatge divider 2 and calvulate real voltage
  voltBat2 = (analogRead(SENSOR_2_BAT_2) * (5.0 / 1023.0) * 3) - 0.13;

  Serial.println("read");
  delay (50);
  //    lastSec = currentSec;
  //
  //    if (lastSec >= 40) {
  //      lastSec = 0;
  //    }

//}



}

void printVariablesToSerial() {
  int currentMin = HCRTC.GetMinute();
  if (currentMin - lastMin != 0) {
    readAllSensors();
    Serial.print(HCRTC.GetDateString());
    Serial.print(" ");
    Serial.print(HCRTC.GetTimeString());
    Serial.print("; ");
    Serial.print("Bat1;");
    Serial.print(voltBat1);
    Serial.print("v; ");
    Serial.print("Bat2;");
    Serial.print(voltBat2);
    Serial.print("v; ");
    Serial.println();
    delay (50);
  }
  lastMin = currentMin;
}


