/*
  State change detection (edge detection)

 Often, you don't need to know the state of a digital input all the time,
 but you just need to know when the input changes from one state to another.
 For example, you want to know when a button goes from OFF to ON.  This is called
 state change detection, or edge detection.

 This example shows how to detect when a button or button changes from off to on
 and on to off.

 The circuit:
 * pushbutton attached to pin 2 from +5V
 * 10K resistor attached to pin 2 from ground
 * LED attached from pin 13 to ground (or use the built-in LED on
   most Arduino boards)

 created  27 Sep 2005
 modified 30 Aug 2011
 by Tom Igoe

This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/ButtonStateChange

 */

/*
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
// for lcd usage 
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
// end includes for lcd

// this constant won't change:
// set button 1 pin - mode select
const int  buttonPin = 2;    // the pin that the pushbutton is attached to
// Sample code 
const int ledPin = 13;       // the pin that the LED is attached to

// LCD stuff
int n = 1;
int sensorPin = A0;    // select the input pin for the potentiometer


//define LCD
LiquidCrystal_I2C  lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);

// Variables will change:
// To do with Button 1
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button
// Sensor 12v 1 
int sensorValue = 0;  // variable to store the value coming from the sensor
float volt = 0;


void setup() {

  //initialise LCD
  lcd.begin (16, 2); //  <<----- My LCD was 16x2
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);

  // Switch on the backlight
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home (); // go home
  
  // initialize the button pin as a input:
  pinMode(buttonPin, INPUT);
  // initialize the LED as an output:
  pinMode(ledPin, OUTPUT);
  // initialize serial communication:
  Serial.begin(9600);
}



void loop() {
  // read the pushbutton input pin:

checkButtonPress();



  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
  volt = (sensorValue * (5.0 / 1023.0) * 3) - 0.13;
  


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
  if (buttonPushCounter % 4 == 0) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

}



void checkButtonPress(){

  buttonState = digitalRead(buttonPin);
  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      // if the current state is HIGH then the button
      // wend from off to on:
      if(buttonPushCounter == 4){
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

void setDisplayMode(){
  switch (buttonPushCounter) {
    case 1:
      //do something when var equals 1
      lcd.clear();
      lcd.setCursor (0, 0);       // go to start of 1nd line
      lcd.print("Battery");
      lcd.setCursor (0, 1);       // go to start of 1nd line
      lcd.print("A 12,3v  B 11,3v");
      break;
    case 2:
      //do something when var equals 2
      lcd.clear();
      lcd.setCursor (0, 0);       // go to start of 1nd line
      lcd.print("Temp in: 21");
      lcd.setCursor (0, 1);       // go to start of 1nd line
      lcd.print("Temp out: 15");
      break;
    case 3:
      //do something when var equals 2
      lcd.clear();
      lcd.setCursor (0, 0);       // go to start of 1nd line
      lcd.print("Webasto: On");
      lcd.setCursor (0, 1);       // go to start of 1nd line
      lcd.print("Therm: 21,0 ");
      break;
    case 4:
      //do something when var equals 2
      lcd.clear();
      lcd.setCursor (0, 0);       // go to start of 1nd line
      lcd.print("");
      break;
    default: 
      // if nothing else matches, do the default
      // default is optional
    break;
  }
  }
  
