/*
 *  Collect all sensor data and save them to global variables.
 *  We need all sensor data for logging, regardless of what is displayed on the lcd.
 *  for now only voltage is read. Other sensors are to be installed.
 */

void readAllSensors() {

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // read the value from volatge divider 1 and calvulate real voltage
    voltBat1 = (analogRead(SENSOR_1_BAT_1) * (5.0 / 1023.0) * 3) - 0.13;

    // read the value from volatge divider 2 and calvulate real voltage
    voltBat2 = (analogRead(SENSOR_2_BAT_2) * (5.0 / 1023.0) * 3) - 0.13;

    // TODO: read shunt from battery 1

    // TODO: read water sensor 1

    // TODO: read water sensor 2

    // TODO: read temp sensor interior

    // TODO: read temp sensor exterior

  }
}

/*
 *  Put all sensordata in a single string and send it to usb output
 *  If arduino is connected to raspberry pi data can be collected and displayed on a webserver.
 */

void printVariablesToSerial() {
  String variableList;
  variableList = variableList + voltBat1 + ";" + voltBat2;
  Serial.println(variableList);
}


