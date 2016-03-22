/*
 * Interups for buttons or sensors
 */

// Loop through info screens with button 1
void pin1_ISR() {
  //buttonState = digitalRead(BUTTON_1);
  lcdClearScreen();
  // go to next screen
  operatingMode++;
  // reset subscreen
  subMode = 0;
  
  //Serial.print("Operation: ");
  //Serial.println(operatingMode);
}

// Loop through webasto settings with button 2
void pin2_ISR() {

  // buttonState = digitalRead(BUTTON_2);
  subMode++;
  //Serial.print("Webasto: ");
  //Serial.println(subMode);
}

