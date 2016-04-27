

//Include libraries
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <LowPower.h>
#include <HCRTC.h> //Hobby Components RTC library 
#include <Adafruit_ADS1015.h>

// pins for buttons
#define BUTTON_1        2   // cycle through screens
#define BUTTON_2        3   // subscreen or switch on webasto -> off/on/half/auto       
#define BUTTON_MIN      4   // set temperature -
#define BUTTON_PLUS     5   // set temperature +

// pin for leds (Digital PWM)
#define LED_DATA        8

// pins for relais (Digital)
#define RELAY_1         9
#define RELAY_2         10
#define RELAY_3         11
#define RELAY_4         12

// pins for sensors
#define SENSOR_1_BAT_1  A0  // Voltage divider 1
#define SENSOR_2_BAT_2  A1  // Voltage divider 2

//// LCD defines ///////////////////////////////////////////////////////////////////////////////////
#define I2C_ADDR    0x27 // <<----- Add your address here.  Find it from I2C Scanner
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

LiquidCrystal_I2C  lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);


/* Ada1115 board */
Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */

/* Define the I2C addresses for the RTC and EEPROM */
#define I2CDS1307Add 0x68
#define I2C24C32Add  0x50

/* Create an instance of HCRTC library */
HCRTC HCRTC;

int lcdState = 1;

// variables to store sensor data
int sensorValue = 0;        // variable to pass on sensor info
int sensorValue2 = 1;        // variable to store the value coming from sensor A2

float voltBat1 = 0;          // convert sensor value to true voltage withe voltage divider formula (x3)
float voltBat2 = 0;          // convert sensor value to true voltage withe voltage divider formula (x3)

float amps = 0;              // Amperes

String lcdLine = "";         // to collect valiables and write to lcd in one go
String lcdLineOld = "";         // to collect valiables and write to lcd in one go

// variables to store operating mode
int operatingMode = 1;    // 1 = Battery monitor, 2: Webasto, more to come as additional sensors are added
int subMode = 0;       // 0 = Off, 1 = On, 2 = Half, 3 = Auto

// part for button interupts
volatile int buttonState = 0;         // variable for reading the pushbutton1 status, can change outside of program code

// temperature for thermostat
float neededTemp = 21.0;

// option variables
int mode = 0;                // operation mode

// timer variable
unsigned long lastRead = 0;     // last time values were checked
unsigned long lastWrite = 0;     // last time values were written to log
unsigned long lastButtonPress = 0;    // last time a button was pressed
const long readInterval = 2000;       // interval before checking again
const long writeInterval = 30000;     // interval before checking again
const long sleepTimer = 10000;        // go to sleep after x seconds afetr last button press

unsigned long thermostatTimer = 0;    // sets a delay between thermostat selection and setting the relay
const long thermostatDelay = 5000;    // this will keep webasto from going into aftercool when going through the options

// RTC variables
int lastMin = 0;
int lastSec = 0;


void setup() {

  //// USB SERIAL init ////////////////////////////////////////////////////////////////////////////////////////
  Serial.begin(9600);

  //// ADA1115 init //////////////////////////////////////////////////////////////////////////////////////
  ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
  ads.begin();

  // LCD init
  lcd.begin (16, 2); //  <<----- My LCD was 16x2
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home (); // go home

  //// SET PINMODES ///////////////////////////////////////////////////////////////////////////////////////////
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  pinMode(BUTTON_MIN, INPUT);
  pinMode(BUTTON_PLUS, INPUT);
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_3, OUTPUT);
  pinMode(RELAY_4, OUTPUT);

  //// ATTACH INTERUPTS ///////////////////////////////////////////////////////////////////////////////////////
  attachInterrupt(digitalPinToInterrupt(BUTTON_1), pin1_ISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_2), pin2_ISR, FALLING);

  // Set relais to off position
  digitalWrite(RELAY_1, HIGH);
  digitalWrite(RELAY_2, HIGH);
  digitalWrite(RELAY_3, HIGH);
  digitalWrite(RELAY_4, HIGH);

  // uncomment line below to reset date on RTC
  // HCRTC.RTCWrite(I2CDS1307Add, 16, 3, 27, 21, 36, 0, 7);
}

void loop() {

  // Read current time
  HCRTC.RTCRead(I2CDS1307Add);
  //TODO: Check how long the arduino has not recieved adny input
  unsigned long currentMillis2 = millis();

  if (currentMillis2 - lastButtonPress >= sleepTimer) {
    // save the last time you blinked the LED
    operatingMode = 0;
  }

  switch (operatingMode) {
    case 0: // sleep mode: do nothing for one minute and read sensors again
      lcd.setBacklight(LOW);
      lcd.off();
      readAllSensors();
      printVariablesToSerial();   // found in SensorLogging.ino
      sleep();
      break;
    case 1: // battery monitor
      readAllSensorsDelay();           // found in SensorLogging.ino
      displayScreen();
      break;
    case 2: // Webasto
      readAllSensorsDelay();           // found in SensorLogging.ino
      displayScreen();
      webastoSettings();
      break;
    default:
      // return to first case
      operatingMode = 1;
      break;
  }
}
