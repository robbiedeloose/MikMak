/*
 * Interups for buttons or sensors
 */

// Loop through info screens with button 1
void pin1_ISR() {
  lcdClearScreen();
  // reset sleep timer
  lastButtonPress  = millis();
  // go to next screen
  operatingMode++;
  // reset subscreen
  subMode = 0;
  
  //Serial.print("Operation: ");
  //Serial.println(operatingMode);
}

// Loop through sub settings with button 2
void pin2_ISR() {
  // reset sleep timer
  lastButtonPress = millis();
  // change to subscreen of change option
  subMode++;

}

