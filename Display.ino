void displayScreen() {

  lcd.on();
  lcd.setBacklight(HIGH);

  switch (operatingMode) {
    case 1:
      lcd.setCursor (0, 0);       // go to start of 1st line
      lcd.print("Battery:       ");
      lcd.home();
      lcd.setCursor (0, 1);       // go to start of 1st line
      lcd.print("b1:        ");
      break;
    case 2:

      lcd.setCursor (0, 0);       // go to start of 1st line
      lcd.print("Temp: ");
      lcd.print(t);
       lcd.home();
      break;
  }
}

