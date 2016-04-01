//Include libraries
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <LowPower.h>
/* Include the Hobby Components RTC library */
#include <HCRTC.h>

// pins for Serial LCD
#define LCD_DISPLAY_TX_PIN           7
#define LCD_DISPLAY_BACKLIGHT_PIN    6

// pins for buttons
#define BUTTON_1        2   // cycle through screens
#define BUTTON_2        3   // subscreen or switch on webasto -> off/on/half/auto       
#define BUTTON_MIN      4   // set temperature -
#define BUTTON_PLUS     5   // set temperature +

// pins for relais
#define RELAY_1         22
#define RELAY_2         24
#define RELAY_3         26
#define RELAY_4         28

// pins for sensors

#define SENSOR_1_BAT_1  A0  // Voltage divider 1
#define SENSOR_2_BAT_2  A1  // Voltage divider 2

/* Define the I2C addresses for the RTC and EEPROM */
#define I2CDS1307Add 0x68
#define I2C24C32Add  0x50

/* Create an instance of HCRTC library */
HCRTC HCRTC;

// LCD instance
SoftwareSerial displayLCD(0, LCD_DISPLAY_TX_PIN);
int lcdState = 1;

// Button state vriables
// int buttonPlusState = 0;
// int buttonPlusOldState = 0;
// int buttonMinState = 0;
// int buttonMinOldState = 0;

// variables to store sensor data
int sensorValue = 0;        // variable to pass on sensor info
int sensorValue2 = 1;        // variable to store the value coming from sensor A2

float voltBat1 = 0;          // convert sensor value to true voltage withe voltage divider formula (x3)
float voltBat2 = 0;          // convert sensor value to true voltage withe voltage divider formula (x3)

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

  // start display
  displayLCD.begin(9600);
  displayLCD.write((byte)4);

  // initialize serial communication to usb:
  Serial.begin(9600);

  /* Use the RTCWrite library function to set the time and date.
   Parameters are: I2C address, year, month, date, hour, minute, second,
   day of week */
  // uncomment line below to reset date on RTC
  // HCRTC.RTCWrite(I2CDS1307Add, 16, 3, 27, 21, 36, 0, 7);

  // set button pinModes
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  pinMode(BUTTON_MIN, INPUT);
  pinMode(BUTTON_PLUS, INPUT);

  // set relay pinModes
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_3, OUTPUT);
  pinMode(RELAY_4, OUTPUT);

  digitalWrite(RELAY_1, HIGH);
  digitalWrite(RELAY_2, HIGH);
  digitalWrite(RELAY_3, HIGH);
  digitalWrite(RELAY_4, HIGH);

  // define interrupts for the 2 mode buttons
  attachInterrupt(digitalPinToInterrupt(BUTTON_1), pin1_ISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_2), pin2_ISR, FALLING);
}

void loop() {

  // Read current time
  /* Read the current time from the RTC module */
  HCRTC.RTCRead(I2CDS1307Add);

  //TODO: Check how long the arduino has not recieved adny input
  unsigned long currentMillis2 = millis();

  if (currentMillis2 - lastButtonPress >= sleepTimer) {
    // save the last time you blinked the LED
    operatingMode = 0;
  }

  // do webast stuff


  // TODO: actions based on mode selected: battery, alarms, temperature, ...

  switch (operatingMode) {
    case 0:
      // sleep mode: do nothing for one minute and read sensors again
      ShowLcdDisplay(0);
      digitalWrite(RELAY_3, HIGH);
      //write to log or serial
      printVariablesToSerial();   // found in SensorLogging.ino
      // ATmega328P, ATmega168
      //LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF,
      //              SPI_OFF, USART0_OFF, TWI_OFF);
      //delay(1000);
      // ATmega2560
      LowPower.idle(SLEEP_8S, ADC_OFF, TIMER5_OFF, TIMER4_OFF, TIMER3_OFF,
                    TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART3_OFF, USART2_OFF, USART1_OFF,
                    USART0_OFF, TWI_OFF);
      Serial.println("SleepCycle");
      delay(50);
      break;
    case 1:
      //Serial.println("Mode 1");
      //do something when var equals 1 --> Battery info
      // read sensor values and populate variables
      readAllSensorsDelay();           // found in SensorLogging.ino
      ShowLcdDisplay(1);
      displayScreen1();
        digitalWrite(RELAY_3, LOW);
      break;
    case 2:
      // Serial.println("Mode 2");
      // read sensor values and populate variables
      readAllSensorsDelay();           // found in SensorLogging.ino
      ShowLcdDisplay(1);
      displayScreen2();
      webastoSettings();
      digitalWrite(RELAY_3, LOW);
      break;
    default:
      // Serial.println("start over");
      // operating mode is bigger than possible options
      operatingMode = 1;
      break;
  }
}
