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
