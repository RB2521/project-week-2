//Code written by RB2521
//This code is to be used for the University of Bath, Department of Electrical and Electronic Engineering, Project Week 2 - Digital Health Monitoring.

//NOTE - This Code uses Libraries to run the Heart rate click, OLED screen and Interfaces. These libraries need to be downloaded in order for this code to run.

//Loading the Libraries
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>


//Defining a reporting period that does not change - used in a comparative statement
#define REPORTING_PERIOD_MS 1000

//Defining the dimensions of the OLED screen
#define OLED_WIDTH 96
#define OLED_HEIGHT 16

#define OLED_ADDR   0x3C

//Loading PulseOximeter. PulseOximeter is the higher level interface to the sensor providing: -Beat detection reporting, -SpO2 calculation and -Heart rate calculation
PulseOximeter pox;

uint32_t tsLastReport = 0;

//Configuring the OLED display
Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT);

//Defining constant and variable variables.
const int TopButtonPin = 4;  // the number of the pushbutton pin
const int RedLedPin = 9;
const int BotButtonPin = 5;
const int GreenLedPin = 8;
int HeartRate = 0;
int SpO2Rate = 0;

//Function to define when a pulse is detected. Fires when a pulse is detected.
void onBeatDetected()
{
    digitalWrite(RedLedPin, HIGH);
    delay(75);
    digitalWrite(RedLedPin, LOW);
}


void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  pinMode(RedLedPin, OUTPUT);
  pinMode(GreenLedPin, OUTPUT);
  pinMode(TopButtonPin, INPUT);
  pinMode(BotButtonPin, INPUT);
  
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();

//For loop used ot hold the text on the screen. Using Delay makes the OLED screen become out of sync and hence no readings can be taken from the sensor.
for ( int i; i < 500; i++) {
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("Initializing");
    display.display();
  }
  //Initialize PuleOximeter. Self-handling failure detection. Failures are usually "due to an improper I2C wiring, missing power supply or wrong target chip"
  //https://github.com/gabriel-milan/Arduino-MAX30100/blob/master/examples/MAX30100_Minimal/MAX30100_Minimal.ino lines 46-47
  if (!pox.begin()) {
    digitalWrite(RedLedPin, HIGH);
    //Displays to the OLED screen if there is an error
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("Setup failed.\nCheck connections");
    display.display();
  
    for (;;);
  }
  else {
    //Displays to the OLED screen if all setup is successful, then progresses to the following code.
    //Set the Green LED to HIGH
    digitalWrite(GreenLedPin, HIGH);

    //Clear the display
    display.clearDisplay();
    display.display();    
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("Setup \nSuccessfull");
    display.display();

    display.clearDisplay();
    display.display();

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("Project Week 2 -Team 6!");
    display.display();
  }

//Register a callback for the beat detection function.
  pox.setOnBeatDetectedCallback(onBeatDetected);
}




void loop(){
  //Calling the update as quickly as possible
    pox.update();

    // Dump both heart rate and oxidation levels to the serial at the same time
    // For both, a value of 0 means "invalid"
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {    
        
        //Display text to the OLED screen
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0, 0);
        HeartRate = int(pox.getHeartRate());
        display.print("Heart rate: ");
        //Improved UI. Displays "--" instead of 0
        if (HeartRate == 0){
          display.print("--");
        } else {
          //Prints heart rate to both the serial monitor and the OLED screen
          display.print(HeartRate);
          Serial.print(HeartRate);
          Serial.print("\n");
        }
        SpO2Rate = int(pox.getSpO2());
        display.print("\n%SpO2: ");
        //Improved UI. Displays "--" instead of 0
        if (SpO2Rate == 0) {
          display.print("--");
        } else {
          //Prints heart rate to both the serial monitor and the OLED screen
          display.print(SpO2Rate);
          Serial.print(SpO2Rate);
          Serial.print("\n");
        }
        //Updates the display
        display.display();

        tsLastReport = millis();
    }
}
