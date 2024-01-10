//Code written by RB2521 and MWSO20
//This code is to be used in for the purposes of the University of Bath, Department of Electrical and Electronic Engineering, Project week 2 - Digital Health Monitoring.

//NOTE - This Code uses Libraries in order to run the Heart rate click, OLED screen and Interfaces. These libraries need to be downloaded in order for this code to run.

//Loading the Libraries
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS 1000

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define OLED_WIDTH 96
#define OLED_HEIGHT 16

#define OLED_ADDR   0x3C


PulseOximeter pox;

uint32_t tsLastReport = 0;


Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT);

const int TopButtonPin = 4;  // the number of the pushbutton pin
const int RedLedPin = 9;
const int BotButtonPin = 5;
const int GreenLedPin = 8;
int i = 0;


void onBeatDetected()
{
    Serial.println("Beat!");
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

  //Initialize PuleOximeter. Self handling failure detection. Failures are usually "due to an improper I2C wiring, missing power supply or wrong target chip"
  //https://github.com/gabriel-milan/Arduino-MAX30100/blob/master/examples/MAX30100_Minimal/MAX30100_Minimal.ino lines 46-47
for ( int i; i < 500; i++) {
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("Initializing");
    display.display();
  }

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
    //Displays to the OLED screen if all setup is successfull, then progresses
    digitalWrite(GreenLedPin, HIGH);

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


  pox.setOnBeatDetectedCallback(onBeatDetected);
}




void loop(){
    pox.update();

    // Asynchronously dump heart rate and oxidation levels to the serial
    // For both, a value of 0 means "invalid"
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        
        Serial.print("Heart rate; ");
        Serial.print(int(pox.getHeartRate()));
        Serial.print("SpO2: ");
        Serial.print(int(pox.getSpO2()));
        Serial.println("\t");
        
        
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0, 0);
        display.print("Heart rate: ");
        display.print(int(pox.getHeartRate()));
        display.print("\n%SpO2: ");
        display.print(int(pox.getSpO2()));
        display.display();

        tsLastReport = millis();
    }
}