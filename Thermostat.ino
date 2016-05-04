void  webastoSettings() {

  Serial.print("Submode: ");
  Serial.println(subMode);
  switch (subMode) {
    case 0:
      lcd.setCursor (0, 1);       // go to start of 1st line
      lcd.print("Webasto: Off    ");
      thermostat();
      break;
    case 1:
      lcd.setCursor (0, 1);       // go to start of 1st line
      lcd.print("Webasto: On     ");
      thermostat();
      break;
    case 2:
      lcd.setCursor (0, 1);       // go to start of 1st line
      lcd.print("Webasto: Half  ");
      thermostat();
      break;
    case 3:
      lcd.setCursor (0, 1);       // go to start of 1st line
      lcd.print("auto: - ");
      lcd.print(neededTemp);
      lcd.print(" +");
      // check temp buttons
      buttonState = digitalRead(BUTTON_MIN);
      if (buttonState == LOW) {
        neededTemp = neededTemp - 0.5;
      }
      buttonState = digitalRead(BUTTON_PLUS);
      if (buttonState == LOW) {
        neededTemp = neededTemp + 0.5;
      }
      thermostat();
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
      digitalWrite(RELAY_1, HIGH);
      digitalWrite(RELAY_2, HIGH);
      break;
    default:

      break;

  }
}
