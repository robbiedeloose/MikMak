/*
 *  Collect all sensor data and save them to global variables.
 *  We need all sensor data for logging, regardless of what is displayed on the lcd.
 *  for now only voltage is read. Other sensors are to be installed.
 */

void readAllSensorsDelay() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastRead > readInterval) {
    readAllSensors();
    lastRead = currentMillis;
  }
}

void readAllSensors() {

  Serial.println("read");
  delay (50);

  // Read voltages //////////////////////////////////////////
  voltBat1 = (analogRead(SENSOR_1_BAT_1) * (5.0 / 1023.0) * 3) - 0.13;
  voltBat2 = (analogRead(SENSOR_2_BAT_2) * (5.0 / 1023.0) * 3) - 0.13;

  // read amps /////////////////////////////////////////////
  int16_t results;
  results = ads.readADC_Differential_0_1();
  ampBat1 = ((float)results * 256.0) / 32768.0;//100mv shunt
  ampBat1 = ampBat1 * 1.333; //uncomment for 75mv shunt
  //ampBat1 = ampBat1 * 2; //uncomment for 50mv shunt
  results = ads.readADC_Differential_2_3();
  ampBat2 = ((float)results * 256.0) / 32768.0;//100mv shunt
  ampBat2 = ampBat2 * 1.333; //uncomment for 75mv shunt
  //ampBat2= ampBat2 * 2; //uncomment for 50mv shunt

  // read temp and humidity ////////////////////////////

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  t = dht.readTemperature();
  t = t -2;
  // Check if any reads failed and exit early (to try again).
  /*if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
  }*/


}

void printVariablesToSerial() {

  //// Write to SD card ////////////////////////////////
  /* Open the data.csv file to save our data to.
     If the file already exists it will just tag our new data onto the end of it */

  //// Write to Serial //////////////////////////////////
  int currentMin = HCRTC.GetMinute();
  if (currentMin - lastMin != 0) {
    readAllSensors();
    Serial.print(HCRTC.GetDateString());
    Serial.print(" ");
    Serial.print(HCRTC.GetTimeString());
    Serial.print("; ");
    Serial.print("Bat1:");
    Serial.print(voltBat1);
    Serial.print("v; ");
    Serial.print("Bat1:");
    Serial.print(ampBat1);
    Serial.print("A; ");
    Serial.print("Bat2:");
    Serial.print(voltBat2);
    Serial.print("v; ");
    Serial.print("Bat2:");
    Serial.print(ampBat2);
    Serial.print("A; ");
    Serial.print("Temp: ");
    Serial.print(t);
    Serial.print("c; Humidity: ");
    Serial.print(h);
    Serial.println(";");
    
    delay (80);


    //SD_File = SD.open("data.csv", FILE_WRITE);

    if (SD_File)
    {
      SD_File.print(HCRTC.GetDateString());
      SD_File.print(" ");
      SD_File.print(HCRTC.GetTimeString());
      SD_File.print("; ");
      SD_File.print("Bat1:");
      SD_File.print(voltBat1);
      SD_File.print("v; ");
      SD_File.print("Bat1:");
      SD_File.print(ampBat1);
      SD_File.print("A; ");
      SD_File.print("Bat2:");
      SD_File.print(voltBat2);
      SD_File.print("v; ");
      SD_File.print("Bat2:");
      SD_File.print(ampBat2);
      SD_File.println("A; ");
      SD_File.close();
    }

  }
  lastMin = currentMin;

}


