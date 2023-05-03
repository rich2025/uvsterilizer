#include <LiquidCrystal_I2C.h>
#include "Adafruit_LTR390.h"
#include "Countimer.h"

Adafruit_LTR390 ltr = Adafruit_LTR390();
LiquidCrystal_I2C lcd(0x27,16,2);

int solenoidPin = 13; // information port for the solenoid
int x = 0; // temporary variable
int S = 0; // count seconds 
int M = 3; // count minutes
int H = 1;

void setup() {

// UV LED port

   pinMode(6, OUTPUT);

// UV Sensor
 
  Serial.begin(115200);
  Serial.println("Adafruit LTR-390 test");

  if ( ! ltr.begin() ) { // detect UV sensor
    Serial.println("Couldn't find LTR sensor!");
    while (1) delay(10);
  }
  Serial.println("Found LTR sensor!");

  ltr.setMode(LTR390_MODE_UVS); // Set LTR390 to read UV values
  if (ltr.getMode() == LTR390_MODE_ALS) {
    Serial.println("In ALS mode");
  } else {
    Serial.println("In UVS mode");
  }

  ltr.setGain(LTR390_GAIN_3);
  Serial.print("Gain : ");
  switch (ltr.getGain()) {
    case LTR390_GAIN_1: Serial.println(1); break;
    case LTR390_GAIN_3: Serial.println(3); break;
    case LTR390_GAIN_6: Serial.println(6); break;
    case LTR390_GAIN_9: Serial.println(9); break;
    case LTR390_GAIN_18: Serial.println(18); break;
  }

  ltr.setResolution(LTR390_RESOLUTION_16BIT);
  Serial.print("Resolution : ");
  switch (ltr.getResolution()) {
    case LTR390_RESOLUTION_13BIT: Serial.println(13); break;
    case LTR390_RESOLUTION_16BIT: Serial.println(16); break;
    case LTR390_RESOLUTION_17BIT: Serial.println(17); break;
    case LTR390_RESOLUTION_18BIT: Serial.println(18); break;
    case LTR390_RESOLUTION_19BIT: Serial.println(19); break;
    case LTR390_RESOLUTION_20BIT: Serial.println(20); break;
  }

  ltr.setThresholds(100, 1000);
  ltr.configInterrupt(true, LTR390_MODE_UVS);

// Solenoid Lock

  pinMode(solenoidPin, OUTPUT);
  
// LCD Screen

  lcd.init();
  lcd.clear();         
  lcd.backlight();      // Make sure backlight is on
  
 
 
}

void loop() {

// Turn on UV LEDSd
 digitalWrite(6, HIGH);
//  delay(2000);
//  digitalWrite(6, LOW);
//  delay(2000);
// Read UV data

  if (ltr.newDataAvailable()) {
      Serial.print("UV data: "); 
      Serial.print(ltr.readUVS());
      Serial.print('\n');

  }


 
// If UV LEDS are off

if (ltr.readUVS() == 0) {

x = 0;

digitalWrite(solenoidPin, HIGH); // Set solenoid to high, "unlock"

//  Print a message on both lines of the LCD.
  lcd.setCursor(0,0);   //Set cursor to character 2 on line 0
  lcd.print("UV OFF         ");
  lcd.setCursor(0,1);   //Move cursor to character 2 on line 1
  lcd.print("DOOR UNLOCKED          ");

}

// If UV LEDS are on

else if(ltr.readUVS() > 0) {

if (x == 0) { // Clear the LCD screen
  lcd.clear();
  x = 1;
}

  digitalWrite(solenoidPin, LOW); // Activate the solenoid, "lock"

  lcd.setCursor(0,0);   //Set cursor to character 2 on line 0
  lcd.print("UV ON           ");
 
 S--; // Initialize the timer, setting second as 1 second
 delay(1000);
  
 if(S<0)
 {
   M--;
   S=59;
 }
 if(M<0)
 {
   H--;
   M=59;
 }
 if(H<0) { H=23; M=59; S=59; } 

   lcd.setCursor(0,1);

   lcd.print(M);
   lcd.setCursor(1,1);
   lcd.print(":");

  
 if(S>9)
 {
   lcd.setCursor(2,1);
   lcd.print(S);
 }
 else
 {
   lcd.setCursor(2,1);
   lcd.print("0"); 
   lcd.setCursor(3,1);
   lcd.print(S);
   lcd.setCursor(5,1);
   lcd.print("          ");
 }
}
}


