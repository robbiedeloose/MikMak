/* 
 *  Collect all sensor data and save them to global variables. 
 *  We need all sensor data for logging, regardless of what is displayed on the lcd.
 *  for now only voltage is read. Other sensors are to be installed.
 */

void readAllSensors(){
  
  // read the value from volatge divider 1 and calvulate real voltage
  voltBat1 = (analogRead(sensorPin1) * (5.0 / 1023.0) * 3) - 0.13;

  // read the value from volatge divider 2 and calvulate real voltage
  voltBat2 = (analogRead(sensorPin2) * (5.0 / 1023.0) * 3) - 0.13;

  // read shunt from battery 1

  // read water sensor 1

  // read water sensor 2

  // read temp sensor interior

  // read temp sensor exterior

}

/* 
 *  Put all sensordata in a single string and send it to usb output
 *  If arduino is connected to raspberry pi data can be collected and displayed on a webserver.
 */

void printVariablesToSerial(){
  String variableList;
  variableList = variableList + voltBat1 + ";" + voltBat2;
  Serial.println(variableList);
}


