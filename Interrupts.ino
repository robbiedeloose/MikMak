/*
 * Interups for buttons or sensors
 */

// Loop through info screens with button 1
void pin1_ISR() {
  buttonState = digitalRead(buttonPin1);
  if (buttonState == LOW) {
    operatingMode++;
    // max number of screens
    if (operatingMode > 4) {
      operatingMode = 1;
    }
    Serial.print("Operation: ");
    Serial.println(operatingMode);
  }
}

// Loop through webasto settings with button 2
void pin2_ISR() {

  buttonState = digitalRead(buttonPin2);
  if (buttonState == LOW) {
    operatingMode = 3;
    webastoMode++;
    // max number of screens
    if (webastoMode > 3) {
      webastoMode = 0;
    }
    Serial.print("Webasto: ");
    Serial.println(webastoMode);
  }
}
