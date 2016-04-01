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
