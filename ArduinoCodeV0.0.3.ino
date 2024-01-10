//Code written by RB2521 and MWSO20
//This code is to be used in for the purposes of the University of Bath, Department of Electrical and Electronic Engineering, Project week 2 - Digital Health Monitoring.

//NOTE - This Code uses Libraries in order to run the Heart rate click, OLED screen and Interfaces. These libraries need to be downloaded in order for this code to run.

//Loading the Libraries
#include <CircularBuffer.h>
#include <MAX30100.h>
#include <MAX30100_BeatDetector.h>
#include <MAX30100_Filters.h>
#include <MAX30100_PulseOximeter.h>
#include <MAX30100_Registers.h>
#include <MAX30100_SpO2Calculator.h>

#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define OLED_WIDTH 96
#define OLED_HEIGHT 16

#define OLED_ADDR   0x3C

#define REPORTING_PERIOD_MS 1000

PulseOximeter pox;

uint32_t tsLastReport = 0;


Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT);

const int TopButtonPin = 4;  // the number of the pushbutton pin
const int RedLedPin = 9;
int TopButtonState = 0;  // variable for reading the pushbutton status
const int BotButtonPin = 5;
const int GreenLedPin = 8;
int BotButtonState = 0;

void onHeartBeat() {
  Serial.println("Beat Detected");
}


void setup() {
  // put your setup code here, to run once:
  pinMode(RedLedPin, OUTPUT);
  pinMode(GreenLedPin, OUTPUT);
  pinMode(TopButtonPin, INPUT);
  pinMode(BotButtonPin, INPUT);
  
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();

  //Initialize PuleOximeter. Self handling failure detection. Failures are usually "due to an improper I2C wiring, missing power supply or wrong target chip"
  //https://github.com/gabriel-milan/Arduino-MAX30100/blob/master/examples/MAX30100_Minimal/MAX30100_Minimal.ino lines 46-47
  Serial.begin(115200);
  if (!pox.begin()) {
    digitalWrite(RedLedPin, HIGH);
    //Displays to the OLED screen if there is an error
    display.clearDisplay();

    display.setTextSize(0.1);
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

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("Setup \nSuccessfull");
    display.display();
    delay(2500);
    display.clearDisplay();
    display.display();
    digitalWrite(GreenLedPin, LOW);
  }

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Project Week 2 -Team 6!");
  display.display();
  delay(5000);

  pox.setOnBeatDetectedCallback(onHeartBeat);
}




void loop() {
  pox.update();
  if (millis() - tsLastReport > REPORTING_PERIOD_MS){
    Serial.print("Heart Rate: ");
    Serial.print(pox.getHeartRate());
    Serial.print("%SpO2: ");
    Serial.print(pox.getSpO2());
    Serial.println("%");

    tsLastReport = millis();
  }

 // read the state of the top pushbutton value:
  TopButtonState = digitalRead(TopButtonPin);
  BotButtonState = digitalRead(BotButtonPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (TopButtonState == HIGH) {
    // turn LED on:
      digitalWrite(GreenLedPin, LOW);
      digitalWrite(RedLedPin, HIGH);

      display.clearDisplay();
      display.display();

      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.clearDisplay();
      display.println("Top Button Pressed");
      display.display();
  }
    else if (BotButtonState == HIGH){
      digitalWrite(RedLedPin, LOW);
      digitalWrite(GreenLedPin, HIGH);

      display.clearDisplay();
      display.display();

      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.clearDisplay();
      display.println("Bottom Button Pressed");
      display.display();
  } 
    else {
      // turn LED off:
      digitalWrite(RedLedPin, LOW);
      digitalWrite(GreenLedPin, LOW);

      display.clearDisplay();
      display.display();

      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.clearDisplay();
      display.println("Home");
      display.display();
  }


}
