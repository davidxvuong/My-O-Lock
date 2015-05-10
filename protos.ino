#include <MyoController.h>
#include <Servo.h> 
#define FIST_PIN 4
#define WAVEIN_PIN 5
#define WAVEOUT_PIN 6
#define FINGERSSPREAD_PIN 7
#define DOUBLETAP_PIN 8

Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
 
int pos = 0;    // variable to store the servo position 
int num;
int GreenPOW = 2;
int RedPOW = 4;

#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 9); // RX, TX

MyoController myo = MyoController();
int wrong = 0;

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  
  pinMode(FIST_PIN, OUTPUT);
  pinMode(WAVEIN_PIN, OUTPUT);
  pinMode(WAVEOUT_PIN, OUTPUT);
  pinMode(FINGERSSPREAD_PIN, OUTPUT);
  pinMode(DOUBLETAP_PIN, OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(GreenPOW, OUTPUT);
  pinMode(RedPOW, OUTPUT);
  
  myo.initMyo();
  
myservo.attach(9);
}


void loop() {

     myo.updatePose();
   switch ( myo.getCurrentPose() ) {
    case rest:
      digitalWrite(FIST_PIN,LOW); 
      digitalWrite(WAVEIN_PIN,LOW);
      digitalWrite(WAVEOUT_PIN,LOW);
      digitalWrite(FINGERSSPREAD_PIN,LOW);
      digitalWrite(DOUBLETAP_PIN,LOW);
      digitalWrite(GreenPOW,LOW);

      break;
    case fist:
      digitalWrite(FIST_PIN,HIGH);
      digitalWrite(RedPOW,HIGH);
      digitalWrite(GreenPOW,LOW);
      wrong++;
      break;
    case waveIn:
      digitalWrite(WAVEIN_PIN,HIGH);
      digitalWrite(GreenPOW,HIGH);
      digitalWrite(RedPOW,LOW);
      unlock();
      break;
    case waveOut:
      digitalWrite(WAVEOUT_PIN,HIGH);
      digitalWrite(RedPOW, HIGH);
      digitalWrite(GreenPOW,LOW);
      wrong++; 
      lock();
      break;
    case fingersSpread:
      digitalWrite(FINGERSSPREAD_PIN,HIGH);
      digitalWrite(RedPOW, HIGH);
      digitalWrite(GreenPOW,LOW);
      wrong++;
      break;
    case doubleTap:
      digitalWrite(DOUBLETAP_PIN,HIGH);
      break;
   
   } 
   if (wrong == 2)
   {
     for(int i = 0; i < 100; i++) {
       digitalWrite(RedPOW, HIGH);
       delay(100);
       digitalWrite(RedPOW,LOW); 
     }
       digitalWrite(RedPOW, LOW);

 
     wrong = 0;
     return;
     
   }
   
}

void unlock() {
  
if (mySerial.available() > 0 ) {
 

//works for the most part, for what we need atleast

  for(pos = 180; pos > 0; pos -= 1)  // goes from 0 degrees to 180 degrees 
             {                                    // in steps of 1 degree 
                myservo.write(pos);              // tell servo to go to position in variable 'pos' 
              delay(15);
             }
  
        }
}


void lock() {
 for(pos = 0; pos <= 180; pos +=1 ) {
    myservo.write(pos);
    delay(15);
 } 
  
}


  
