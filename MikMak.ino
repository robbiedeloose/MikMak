
/* LCD EXAMPLE USED TO BUILD THIS CODE
** Example Arduino sketch for SainSmart I2C LCD Screen 16x2
** based on https://bitbucket.org/celem/sainsmart-i2c-lcd/src/3adf8e0d2443/sainlcdtest.ino
** by
** Edward Comer
** LICENSE: GNU General Public License, version 3 (GPL-3.0)

** This example uses F Malpartida's NewLiquidCrystal library. Obtain from:
** https://bitbucket.org/fmalpartida/new-liquidcrystal

** Modified - Ian Brennan ianbren at hotmail.com 23-10-2012 to support Tutorial posted to Arduino.cc

** Written for and tested with Arduino 1.0
**
** NOTE: Tested on Arduino Uno whose I2C pins are A4==SDA, A5==SCL

*/

//Includes

// for lcd usage - To check if all pins are needed /////////////////////////////////////////////////////////
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR    0x27 // <<----- Add your address here.  Find it from I2C Scanner
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

//define LCD
LiquidCrystal_I2C  lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);
// end includes for lcd ////////////////////////////////////////////////////////////////////////////////////


// set  pin variables
const int  buttonPin1 = 2;    // cycle through screens
const int  buttonPin2 = 3;    // switch on webasto -> off/on/half/auto
const int  buttonPin3 = 4;    // set temperature

const int sensorPin1 = A0;    // Voltage divider 1
const int sensorPin2 = A1;    // Voltage divider 2

const int ledPin = 13;        // built in led


// variables to store sensor data
int sensorValue = 0;        // variable to pass on sensor info
int sensorValue2 = 1;        // variable to store the value coming from sensor A2

float voltBat1 = 0;          // convert sensor value to true voltage withe voltage divider formula (x3)
float voltBat2 = 0;          // convert sensor value to true voltage withe voltage divider formula (x3)

String lcdLine = "";         // to collect valiables and write to lcd in one go
String lcdLineOld = "";         // to collect valiables and write to lcd in one go

// button variables
int buttonPushCounter = 0;   // counter for the number of button presses

// part for button interupts
volatile int buttonState = 0;         // variable for reading the pushbutton1 status, can change outside of program code

// option variables
int mode = 0;                // operation mode

void setup() {

  //initialise LCD
  lcd.begin (16, 2); //  <<----- My LCD was 16x2
  // Switch on the backlight
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home (); // go home

  // set pinModes
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  // initialize serial communication:
  Serial.begin(9600);

  // part for interupts
  attachInterrupt(digitalPinToInterrupt(buttonPin1), pin1_ISR, CHANGE);

  lcd.clear();
  lcd.setCursor (0, 0);       // go to start of 1nd line
  lcd.print("MikMak");
}


void loop() {

  // TODO:

  // set option variables

  // read sensor values and populate variables
  readAllSensors();

  // write variables to sd card for logging without Raspberry Pi

  // write variables to serial out for logging on the Raspberry Pi
  printVariablesToSerial();
  
  // actions based on mode selected: battery, alarms, temperature, ...


  delay(500);
  Serial.println(buttonPushCounter);



  switch (buttonPushCounter) {
    case 1:
      //do something when var equals 1 --> Battery info
      lcdLine = "";
      lcd.home();
      lcd.setCursor (0, 0);       // go to start of 1st line
      lcd.print("Battery info");
      //volt
      lcd.home();
      lcd.setCursor (0, 1);       // go to start of 2nd line
      lcdLine = lcdLine + "A " + voltBat1 + "v B " + voltBat2 + "v";
      lcd.print(lcdLine);
      
      break;
    case 2:
      //do something when var equals 2

      break;
    case 3:
      //do something when var equals 3
      
      break;
    case 4:
      //do something when var equals 4
 
      break;
    default:
      // if nothing else matches, do the default
      // default is optional
      break;
  }


  /*
    lcd.setCursor (0, 0);       // go to start of 1nd line
    lcd.print("Volt 1: ");
    //volt
    lcd.setCursor (8, 0);       // go to position 8 of 1st line
    lcd.print(volt, 2);
    //delay(1000);
  */
  // turns on the LED every four button pushes by
  // checking the modulo of the button push counter.
  // the modulo function gives you the remainder of
  // the division of two numbers:
  /*
  if (buttonPushCounter % 4 == 0) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
  */
}

void readAllSensors()
{
  // read the value fromvolatge divider 1

  voltBat1 = (analogRead(sensorPin1) * (5.0 / 1023.0) * 3) - 0.13;
  voltBat2 = (analogRead(sensorPin2) * (5.0 / 1023.0) * 3) - 0.13;
  // read the value fromvolatge divider 2
  // sensorValue = analogRead(sensorPin2);
  //voltBat2 = (sensorValue * (5.0 / 1023.0) * 3) - 0.13;

}

void printVariablesToSerial() {
  String variableList;
  variableList = variableList + voltBat1 +";"+ voltBat2;
  Serial.println(variableList); 
}


// Loop through info screens with button 1
void pin1_ISR() {
  buttonState = digitalRead(buttonPin1);
  if (buttonState == LOW) {
    buttonPushCounter++;
    // max number of screens
    if (buttonPushCounter > 4) {
      buttonPushCounter = 1;
      Serial.println(buttonPushCounter);
    }
  }
}

/*

void checkButtonPress() {

  buttonState = digitalRead(buttonPin);
  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      // if the current state is HIGH then the button
      // wend from off to on:
      if (buttonPushCounter == 4) {
        buttonPushCounter = 1;
      }
      else
      {
        buttonPushCounter++;
      }

      Serial.println("on");
      Serial.print("number of button pushes:  ");
      Serial.println(buttonPushCounter);
      setDisplayMode();
    } else {
      // if the current state is LOW then the button
      // wend from on to off:
      Serial.println("off");
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state,
  //for next time through the loop
  lastButtonState = buttonState;



}

void setDisplayMode() {

}
*/



