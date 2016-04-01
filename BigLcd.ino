void lcdClearScreen() {
  displayLCD.write((byte)12);
}

void ShowLcdDisplay(int x) {
  if (x != lcdState) {
    byte value = x + 2;
    displayLCD.write((byte)value);
    lcdState = x;
    Serial.println((byte)value);
    Serial.println(lcdState);
    Serial.println(x);
    delay(50);
  }
}

void lcdSetCursor(byte row, byte digit) {
  displayLCD.write((byte)17);
  displayLCD.write((byte)digit);
  displayLCD.write((byte)row);
}

void lcdDisplayBar(int percent) {
  // not working
  displayLCD.write((byte)18);
  displayLCD.write((byte)255);
  displayLCD.write((byte)000);
  displayLCD.write((byte)19);
  displayLCD.write((byte)80);
  displayLCD.write((byte)003);
}

void displayScreen1() {

  displayLCD.write((byte)17);
  displayLCD.write((byte)0);
  displayLCD.write((byte)0);
  displayLCD.print("Battery info");
  displayLCD.write((byte)17);
  displayLCD.write((byte)0);
  displayLCD.write((byte)1);
  displayLCD.write("1: ");
  displayLCD.print(voltBat1);
  displayLCD.write("v 2: ");
  displayLCD.print(voltBat2);
  displayLCD.print("v");
  displayLCD.write((byte)17);
  displayLCD.write((byte)0);
  displayLCD.write((byte)2);
  displayLCD.print("3: 12,63v");
  delay(50);
}

void displayScreen2() {
  displayLCD.write((byte)17);
  displayLCD.write((byte)0);
  displayLCD.write((byte)0);
  displayLCD.write("Webasto:");

  displayLCD.write((byte)17);
  displayLCD.write((byte)0);
  displayLCD.write((byte)3);
  displayLCD.print("In: 21.8c Out 14,2c");
  Serial.write("mode 2:");
  delay(50);
}

//  display webasto modus

void  webastoSettings() {

  switch (subMode) {
    case 0:
      displayLCD.write((byte)17);
      displayLCD.write((byte)0);
      displayLCD.write((byte)1);
      displayLCD.print("Mode: Off           ");

      displayLCD.write((byte)17);
      displayLCD.write((byte)0);
      displayLCD.write((byte)2);
      displayLCD.print("                    ");
      delay(50);
      break;
    case 1:
      displayLCD.write((byte)17);
      displayLCD.write((byte)0);
      displayLCD.write((byte)1);
      displayLCD.print("Mode: On            ");

      displayLCD.write((byte)17);
      displayLCD.write((byte)0);
      displayLCD.write((byte)2);
      displayLCD.print("                    ");
      delay(50);
      break;
    case 2:
      displayLCD.write((byte)17);
      displayLCD.write((byte)0);
      displayLCD.write((byte)1);
      displayLCD.print("Mode: Half            ");

      displayLCD.write((byte)17);
      displayLCD.write((byte)0);
      displayLCD.write((byte)2);
      displayLCD.print("                    ");
      delay(50);
      break;
    case 3:
      displayLCD.write((byte)17);
      displayLCD.write((byte)0);
      displayLCD.write((byte)1);
      displayLCD.write("Mode: Auto          ");

      displayLCD.write((byte)17);
      displayLCD.write((byte)0);
      displayLCD.write((byte)2);
      displayLCD.print("Thermostat - ");
      displayLCD.print(neededTemp);
      displayLCD.print(" +");
      delay(50);
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
