void displayScreen() {

  lcd.on();
  lcd.setBacklight(HIGH);

  switch (operatingMode) {
    case 1:
      lcd.setCursor (0, 0);       // go to start of 1st line
      lcd.print("Battery1:");
      lcd.print(voltBat1);
      lcd.print("v ");
      lcd.home();
      lcd.setCursor (0, 1);       // go to start of 1st line
      lcd.print("A1:");
      lcd.print(ampBat1);
      lcd.print("A2:");
      lcd.print(ampBat2);
      break;
    case 2:

      lcd.setCursor (0, 0);       // go to start of 1st line
      lcd.print("Temp: ");
      lcd.print(t);
      lcd.print("c    ");
      lcd.home();
      break;
  }
}

