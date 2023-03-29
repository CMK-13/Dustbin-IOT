#include <Servo.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DFRobotDFPlayerMini.h>
#include <Ultrasonic.h>
#include <ThingSpeak.h>
SoftwareSerial mySerial(D1, D2); 
DFRobotDFPlayerMini myDFPlayer;
WiFiClient client;
Servo myServo;
BlynkTimer timer;
TinyGPSPlus gps; 
const int trig1 = D0, echo1 = D6, trig = D4, echo = D5,servoPin = D7;;    
WidgetMap myMap(V0);  
float spd;      
float sats;     
String bearing;  

char auth[] = "rtDyeGrMzygKG4DS4BQQIM8RmJDXZCJj";           
char ssid[] = "Xiaomi";                                     
char pass[] = "11111111"; 

unsigned long channelID = 2052055;
const char * writeAPIKey = "BT9AI7FL5G2ELSBO"; 

unsigned int move_index = 1;      
long duration;
double distance1;
WidgetLCD lcd(V1);
float GetDistance()      
{
    unsigned long duration; // biến đo thời gian
    int distanceCm;           // biến lưu khoảng cách
    /* Phát xung từ chân trig */
    digitalWrite(trig,0);   // tắt chân trig
    delayMicroseconds(2);
    digitalWrite(trig,1);   // phát xung từ chân trig
    delayMicroseconds(2);   // xung có độ dài 5 microSeconds
    digitalWrite(trig,0);   // tắt chân trig
    /* Tính toán thời gian */
    // Đo độ rộng xung HIGH ở chân echo. 
    duration = pulseIn(echo,HIGH);  
    // Tính khoảng cách đến vật.
    distanceCm = int(duration/2/29.412);
    delay(10);
  return distanceCm;
}
void sendSensor()
{
  digitalWrite(trig1, 0);   // Makes trigPin low
  delayMicroseconds(2);       // 2 micro second delay
  digitalWrite(trig1, 1);  // tigPin high
  delayMicroseconds(10);      // trigPin high for 10 micro seconds
  digitalWrite(trig1, 0);   // trigPin low
  duration = pulseIn(echo1, HIGH);   //Read echo pin, time in microseconds
  distance1 = int (duration/2/29.412);   //Calculating actual/real distance
  distance1=100-distance1/33*100;
  Serial.print("Distance = ");        //Output distance on arduino serial monitor
  Serial.println(distance1);
  Blynk.virtualWrite(V1,"Dustbin IOT");
  if(distance1<80) Blynk.virtualWrite(V2,"Empty");
  else Blynk.virtualWrite(V2,"Full");
  if(distance1<0)  Blynk.virtualWrite(V3, 0);
  else Blynk.virtualWrite(V3,(int)distance1);
}
float sendSensor1()
{
 digitalWrite(trig1, 0);   // Makes trigPin low
  delayMicroseconds(2);       // 2 micro second delay
  digitalWrite(trig1, 1);  // tigPin high
  delayMicroseconds(10);      // trigPin high for 10 micro seconds
  digitalWrite(trig1, 0);   // trigPin low
  duration = pulseIn(echo1, HIGH);   //Read echo pin, time in microseconds
  distance1 = int (duration/2/29.412);   //Calculating actual/real distance
  distance1=100-distance1/33*100;
  Serial.print("Distance = ");        //Output distance on arduino serial monitor
  Serial.println(distance1);
  return distance1;
}
void runServo()          // chay Servo
{        
  myServo.write(180);  // Cho servo quay một góc 180 độ
  delay(4000);                 // giữ nguyên mở nắp 3 giây
  myServo.write(0); // đóng nắp về vị trí ban              
}
void displayInfo()     // Hien thi thong so
{ 
  if (gps.location.isValid() ) 
  {    
    float latitude = (gps.location.lat());     //Storing the Lat. and Lon. 
    float longitude = (gps.location.lng()); 
    
    Serial.print("LAT:  ");
    Serial.println(latitude, 6);  // float to x decimal places
    Serial.print("LONG: ");
    Serial.println(longitude, 6);
    Blynk.virtualWrite(V1, String(latitude, 6));   
    Blynk.virtualWrite(V2, String(longitude, 6));  
    myMap.location(move_index, latitude, longitude, "GPS_Location");
    spd = gps.speed.kmph();               //get speed
       Blynk.virtualWrite(V3, spd);
       
       sats = gps.satellites.value();    //get number of satellites
       Blynk.virtualWrite(V4, sats);

       bearing = TinyGPSPlus::cardinal(gps.course.value()); // get the direction
       Blynk.virtualWrite(V5, bearing);                   
  }
  Serial.println();
}
void checkGPS()        // kiem tra du lieu gps
{
  if (gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    Blynk.virtualWrite(V4, "GPS ERROR");  // Value Display widget  on V4 if GPS not detected
  }
}


void setup() {
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



void loop() {
  int distance = GetDistance();
  int distance1 = sendSensor1();


  int fieldNumber = 1; // Số Field cần ghi (trong trường hợp này, Field số 1)
  ThingSpeak.writeField(channelID, fieldNumber, distance1 , writeAPIKey);
  if (distance1 >= 80 && distance < 25 ) 
  {
    myDFPlayer.play(2);
    delay(3000);
  }   
  else if( distance < 25 && distance1 < 80)
  {   
    myDFPlayer.play(1);
    myServo.write(180);  // Cho servo quay một góc 180 độ
    delay(8000);      
    myServo.write(0); // đóng nắp về vị trí ban
  }
  delay(2000);
  Blynk.run();
  timer.run();
}