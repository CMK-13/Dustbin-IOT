#ifndef Function_h
#define Function_h

#include "Servo.h"
#include "TinyGPS++.h"
#include "SoftwareSerial.h"
#include "ESP8266WiFi.h"
#include "BlynkSimpleEsp8266.h"
#include "DFRobotDFPlayerMini.h"
#include "Ultrasonic.h"
#include "ThingSpeak.h"
#include "Arduino.h"

#define BLYNK_PRINT Serial

SoftwareSerial mySerial(D1, D2); 
DFRobotDFPlayerMini myDFPlayer;
WiFiClient client;
Servo myServo;
BlynkTimer timer;
TinyGPSPlus gps;
char auth[] = "rtDyeGrMzygKG4DS4BQQIM8RmJDXZCJj";           
char ssid[] = "Xiaomi";                                     
char pass[] = "11111111"; 
unsigned long channelID = 2052055;
const char * writeAPIKey = "BT9AI7FL5G2ELSBO"; 
WidgetLCD lcd(V1);
WidgetMap myMap(V0);

class Dustbin
{
  private:
      static const int trig1 = D0, echo1 = D6, trig = D4, echo = D5,servoPin = D7; 
      float spd;      
      float sats;     
      String bearing; 
      unsigned int move_index = 1;      
      static long duration;
  public:

      void Initialization_Pin()
      {
            Serial.begin(9600);
            mySerial.begin(9600);
            WiFi.begin(ssid, pass);
            ThingSpeak.begin(client);
            Blynk.begin(auth, ssid, pass, "blynk.en-26.com", 9600);
            myDFPlayer.begin(mySerial);
            myDFPlayer.volume(30); 
            myServo.attach(servoPin);
            myServo.write(0);
            pinMode(trig, OUTPUT);
            pinMode(echo, INPUT);
            pinMode(trig1, OUTPUT);
            pinMode(echo1, INPUT);
            timer.setInterval(1000L, sendSensor);
      }
      float GetDistance();
      static void sendSensor();
      //void sendSensor();
      float sendSensor1();
      void runServo();
      void displayInfo();
      void checkGPS();   
      void DustbinRun();
      void ThingSpeakRun();
      void BlynkRun();
};
#endif

