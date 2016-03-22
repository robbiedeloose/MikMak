

//Include libraries
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>


// pins for Serial LCD
#define LCD_DISPLAY_TX_PIN           7
#define LCD_DISPLAY_BACKLIGHT_PIN    6

// pins for buttons
#define BUTTON_1        2   // cycle through screens
#define BUTTON_2        3   // subscreen or switch on webasto -> off/on/half/auto       
#define BUTTON_MIN      4   // set temperature -
#define BUTTON_PLUS     5   // set temperature +

// pins for sensors

#define SENSOR_1_BAT_1  A0  // Voltage divider 1
#define SENSOR_2_BAT_2  A1  // Voltage divider 2

// LCD instance
SoftwareSerial displayLCD(0, LCD_DISPLAY_TX_PIN);

int buttonPlusState = 0;
int buttonPlusOldState = 0;
int buttonMinState = 0;
int buttonMinOldState = 0;


// variables to store sensor data
int sensorValue = 0;        // variable to pass on sensor info
int sensorValue2 = 1;        // variable to store the value coming from sensor A2

float voltBat1 = 0;          // convert sensor value to true voltage withe voltage divider formula (x3)
float voltBat2 = 0;          // convert sensor value to true voltage withe voltage divider formula (x3)

String lcdLine = "";         // to collect valiables and write to lcd in one go
String lcdLineOld = "";         // to collect valiables and write to lcd in one go


int operatingMode = 1;    // 1 = Battery monitor, 2: temp, humidity, liquid level  3: Webasto thermostat, 4: duno :) options or something
int subMode = 0;       // 0 = Off, 1 = On, 2 = Half, 3 = Auto

// part for button interupts
volatile int buttonState = 0;         // variable for reading the pushbutton1 status, can change outside of program code

// temperature for thermostat
float neededTemp = 21.0;

// option variables
int mode = 0;                // operation mode

// timer variable
unsigned long previousMillis = 0;   // last time values were checked
const long interval = 5000;         // interval before checking again


void setup() {

  // start display
  displayLCD.begin(9600);
  displayLCD.write((byte)4);
  // initialize serial communication to usb:
  Serial.begin(9600);

  // set pinModes
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  pinMode(BUTTON_MIN, INPUT);
  pinMode(BUTTON_PLUS, INPUT);
  //pinMode(ledPin, OUTPUT);


  // part for interupts
  attachInterrupt(digitalPinToInterrupt(BUTTON_1), pin1_ISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_2), pin2_ISR, FALLING);
}


void loop() {

  // TODO: set option variables -> not needed yet. possible to save these to sd card?

  // read sensor values and populate variables
  readAllSensors();           // found in SensorLogging.ino

  // TODO: write variables to sd card for logging without Raspberry Pi

  // write variables to serial out for logging on the Raspberry Pi
  printVariablesToSerial();   // found in SensorLogging.ino

  // TODO: actions based on mode selected: battery, alarms, temperature, ...

  switch (operatingMode) {
    case 0:
      // sleep mode: do nothing for one minute and read sensors again
      delay(60000);
    case 1:
      //do something when var equals 1 --> Battery info
      displayScreen1();
      break;
    case 2:
      displayScreen2();
      webastoSettings();
      break;
    default:
      // operating mode is bigger than possible options
      operatingMode = 1;
      break;
  }
  delay(200);
}
