void  webastoSettings() {

  switch (subMode) {
    case 0:
  
      break;
    case 1:

      break;
    case 2:

      break;
    case 3:
      // check temp buttons
      buttonState = digitalRead(BUTTON_MIN);
      if (buttonState == LOW) {
        neededTemp = neededTemp - 0.5;
      }
      buttonState = digitalRead(BUTTON_PLUS);
      if (buttonState == LOW) {
        neededTemp = neededTemp + 0.5;
      }

      break;
    default:
      // operating mode is bigger than possible options
      subMode = 0;
      break;
  }


}

void  thermostat() {

  Serial.println("Webasto");
  delay(50);
  switch (subMode) {
    case 0:
      digitalWrite(RELAY_1, HIGH);
      digitalWrite(RELAY_2, HIGH);
      break;
    case 1:
      digitalWrite(RELAY_1, LOW);
      digitalWrite(RELAY_2, HIGH);
      break;
    case 2:
      digitalWrite(RELAY_1, LOW);
      digitalWrite(RELAY_2, LOW);
      break;
    case 3:
      // lots of things to do here
      break;
    default:

      break;

  }
}
