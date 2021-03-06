


////INCLUDES ////////////////////////////////////////////////////////////////////////////////////////
//Include libraries
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <LowPower.h>
#include <HCRTC.h> //Hobby Components RTC library 
#include <Adafruit_ADS1015.h>
#include <SPI.h>
#include <SD.h>
#include <DHT.h>

/* DIO pin used to control the SD card CS pin */
#define SD_CS_DIO 10

//// PINS ///////////////////////////////////////////////////////////////////////////////////////////
// pins for buttons
#define BUTTON_1        2   // cycle through screens
#define BUTTON_2        3   // subscreen or switch on webasto -> off/on/half/auto       
#define BUTTON_MIN      8   // set temperature -
#define BUTTON_PLUS     7   // set temperature +

// pin for leds (Digital PWM)
#define LED_DATA        4

// pins for relais (Digital)
#define RELAY_1         9
#define RELAY_2         10
#define RELAY_3         11
#define RELAY_4         12

// pins for sensors
#define SENSOR_1_BAT_1  A7  // Voltage divider 1
#define SENSOR_2_BAT_2  A1  // Voltage divider 2

// pins for Temp sensor
#define DHTPIN 5     // what digital pin we're connected to

// pins for SD card
// #define SD_CS_DIO 10

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


//// ADS1115 init //////////////////////////////////////////////////////////////////////////////////
Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */

//// RTC ///////////////////////////////////////////////////////////////////////////////////////////
/* Define the I2C addresses for the RTC and EEPROM */
#define I2CDS1307Add 0x68
#define I2C24C32Add  0x50
/* Create an instance of HCRTC library */
HCRTC HCRTC;

//// SD CARD init //////////////////////////////////////////////////////////////////////////////////
/* DIO pin used to control the SD card CS pin */
File SD_File;


//// DHT Type /////////////////////////////////////////////////////////////////////////////////////
// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);

//// VARIABLES /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// variables to store sensor data

float voltBat1 = 0;     // convert sensor value to true voltage withe voltage divider formula (x3)
float voltBat2 = 0;     // convert sensor value to true voltage withe voltage divider formula (x3)
float ampBat1 = 0;      // Amperes
float ampBat2 = 0;      // Amperes

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
const long sleepTimer = 20000;        // go to sleep after x seconds afetr last button press

unsigned long thermostatTimer = 0;    // sets a delay between thermostat selection and setting the relay
const long thermostatDelay = 5000;    // this will keep webasto from going into aftercool when going through the options

// RTC variables
int lastMin = 0;
int lastSec = 0;

// HTC variables
float h; // humidity
float t; // temperature


void setup() {

  //// SD CARD init /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  /*
    // Set the SD card CS pin to an output
    pinMode(SD_CS_DIO, OUTPUT);
    // Initialise the SD card
    if (!SD.begin(SD_CS_DIO))
    {
      // If there was an error output this to the serial port and go no further
      Serial.println("ERROR: SD card failed to initialise");
      while (1);
    } else
    {
      Serial.println("SD Card OK");
    }
  */

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

  // DHT init
  dht.begin();

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
    /* Use the RTCWrite library function to set the time and date. 
     Parameters are: I2C address, year, month, date, hour, minute, second, 
     day of week */
   //HCRTC.RTCWrite(I2CDS1307Add, 16, 5, 4, 19, 57, 0, 3);

  Serial.println("Started");
}

void loop() {

  //Serial.println("loop");
  // Read current time
  HCRTC.RTCRead(I2CDS1307Add);
  //xSerial.println(HCRTC.GetTimeString());
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
