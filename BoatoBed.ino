#include <TinyGPS++.h>   //including GPS Library

TinyGPSPlus gps;

volatile float minutes, seconds;   
volatile int degree, secs, mins;  
String messageString;             // variable to store the gps location

#include <Servo.h>     //including Servo library
#define sensor_pin 7   //Define sensor pin as 2

Servo myservo1;  // create servo object to control a servo
Servo myservo2;  
int pos = 0;    // variable to store the servo position
String gsm;

void setup()
{
  myservo1.attach(5);  // attaches the servo on pin 9 to the servo object
  myservo2.attach(6);
  pinMode(sensor_pin, INPUT);  // set as sensor pin as input
  Serial.begin(9600);
  Serial1.begin(9600); 
  
}

void loop() 
{
  smartDelay(1000);
  unsigned long start;
  double lat_val, lng_val, alt_m_val;
  bool loc_valid, alt_valid;
  lat_val = gps.location.lat();
  lng_val = gps.location.lng();
  loc_valid = gps.location.isValid();
  alt_m_val = gps.altitude.meters(); 
  alt_valid = gps.altitude.isValid();  
  DegMinSec(lat_val);
  DegMinSec(lng_val);
  String latt = String(lat_val,6);
  String lngg = String(lng_val,6);
  messageString = "http://www.google.com/maps/place/"+latt+","+lngg;


  int sensor_value = digitalRead(sensor_pin);
  Serial.println(sensor_value);
  if(sensor_value == 0)
  {
    for (pos = 0; pos <= 90; pos += 1)  // goes from 0 degrees to 60 degrees // in steps of 1 degree
    {
      myservo1.write(pos);  
      // tell servo to go to position in variable 'pos'
      myservo2.write(90-pos); 
      delay(15);                       // waits 15ms for the servo to reach the position
    }
    delay(1000);
    for (pos = 90; pos >= 0; pos -= 1)   // goes from 180 degrees to 0 degrees
    { 
      myservo1.write(pos);   
      // tell servo to go to position in variable 'pos'
      myservo2.write(90-pos);
      delay(15);                       // waits 15ms for the servo to reach the position
    }
    
  
    String gsm = Serial.readString();
    gsm.remove(gsm.length()-1);
    Serial.println(gsm.length());

    Serial.println("Moisture Detected");
    //GSM Start(Msg)  
    Serial.println("AT+CMGF=1");  //set to message mode
    delay(1000); 
    Serial.println("AT+CMGS=\"+919446085692\"\r"); // mobile number to send sms
    delay(1000);
    Serial.print("GPS Location:");
    Serial.println(messageString);// The SMS text you want to send
    delay(1000);
    Serial.println((char)26);// ASCII code of CTRL+Z
    delay(10000);
    Serial.println("AT+CMGF=1");  //set to message mode
    delay(1000); 
    Serial.println("AT+CMGS=\"+919447037692\"\r");   // mobile number to send sms
    delay(1000);
    Serial.print("GPS Location:");
    Serial.println(messageString);// The SMS text you want to send
    delay(1000);
    Serial.println((char)26);// ASCII code of CTRL+Z
    delay(10000);
  }
  else if(sensor_value == 1)
    {
    myservo1.write(pos); 
    myservo2.write(90-pos); 
    }
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (Serial1.available()) 
    gps.encode(Serial1.read());
  } while (millis() - start < ms);
}

void DegMinSec( double tot_val)
{
  degree = (int)tot_val;
  minutes = tot_val - degree;
  seconds = 60 * minutes;
  minutes = (int)seconds;
  mins = (int)minutes;
  seconds = seconds - minutes;
  seconds = 60 * seconds;
  secs = (int)seconds;
}
