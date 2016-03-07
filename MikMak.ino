
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
const int  buttonPinMin = 4;    // set temperature -
const int  buttonPinPlus = 5;    // set temperature +

int buttonPlusState = 0;
int buttonPlusOldState = 0;
int buttonMinState = 0;
int buttonMinOldState = 0;

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


int operatingMode = 1;    // 1 = Battery monitor, 2: temp, humidity, liquid level  3: Webasto thermostat, 4: duno :) options or something
int webastoMode = 0;       // 0 = Off, 1 = On, 2 = Half, 3 = Auto

// part for button interupts
volatile int buttonState = 0;         // variable for reading the pushbutton1 status, can change outside of program code

// temperature for thermostat
float neededTemp = 21.0;

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
  pinMode(buttonPinMin, INPUT_PULLUP);
  pinMode(buttonPinPlus, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  // initialize serial communication:
  Serial.begin(9600);

  // part for interupts
  attachInterrupt(digitalPinToInterrupt(buttonPin1), pin1_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(buttonPin2), pin2_ISR, CHANGE);

  lcd.clear();
  lcd.setCursor (0, 0);       // go to start of 1nd line
  lcd.print("MikMak");
}


void loop() {

  // TODO:

  // set option variables -> not needed yet. possible to save these to sd card?

  // read sensor values and populate variables
  readAllSensors();           // found in SensorLogging.ino

  // write variables to sd card for logging without Raspberry Pi

  // write variables to serial out for logging on the Raspberry Pi
  printVariablesToSerial();   // found in SensorLogging.ino

  // actions based on mode selected: battery, alarms, temperature, ...


  delay(250);

  // for troubleshooting only.
  // Serial.println(operatingMode);


  //
  switch (operatingMode) {
    case 1:
      //do something when var equals 1 --> Battery info
      lcdLine = "";
      lcd.home();
      lcd.setCursor (0, 0);       // go to start of 1st line
      lcd.print("Battery info    ");
      //volt
      lcd.home();
      lcd.setCursor (0, 1);       // go to start of 2nd line
      lcdLine = lcdLine + "A " + voltBat1 + "v B " + voltBat2 + "v";
      lcd.print(lcdLine);

      break;
    case 2:
      //do something when var equals 2
      lcd.clear();
      lcd.setCursor (0, 0);       // go to start of 1st line
      lcd.print("Mode 2");
      break;
    case 3:
      //change webasto thermostat settings
      lcd.home();
      lcd.setCursor (0, 0);       // go to start of 1st line
      lcd.print("Webasto:        ");

      //  display webasto modus
      switch (webastoMode) {
        case 0:
          lcd.setCursor (10, 0);       // go to start of 1st line
          lcd.print("Off   ");
          lcd.setCursor (0, 1);       // go to start of 1st line
          lcd.print("20,2 C          ");
          break;
        case 1:
          lcd.setCursor (10, 0);       // go to start of 1st line
          lcd.print("On    ");
          lcd.setCursor (0, 1);       // go to start of 1st line
          lcd.print("20,2 C          ");
          break;
        case 2:
          lcd.setCursor (10, 0);       // go to start of 1st line
          lcd.print("Half  ");
          lcd.setCursor (0, 1);       // go to start of 1st line
          lcd.print("20,2 C          ");
          break;
        case 3:
          lcd.setCursor (10, 0);       // go to start of 1st line
          lcd.print("Auto  ");
          lcd.setCursor (0, 1);       // go to start of 1st line
          lcd.print("20,2 C  - ");
          lcd.print(neededTemp, 1);
          lcd.print(" +");

          // check temp buttons
          buttonState = digitalRead(buttonPinMin);
          if (buttonState == LOW) {
            neededTemp = neededTemp - 0.5;
          }
          buttonState = digitalRead(buttonPinPlus);
          if (buttonState == LOW) {
            neededTemp = neededTemp + 0.5;
          }

          break;
      }

      break;
    case 4:
      //do something when var equals 4
      lcd.clear();
      lcd.setCursor (0, 0);       // go to start of 1st line
      lcd.print("Mode 4");
      break;
    default:
      // if nothing else matches, do the default
      // default is optional
      break;
  }
}
