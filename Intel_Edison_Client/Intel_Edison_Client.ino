#include "Seeed_QTouch.h"
#include <Wire.h>

int counter = 0;

void setup() {
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(A5, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  Serial.println("Waiting for first num");
  while (QTouch.touchNum() == -1 || QTouch.touchNum() > 3);
  int num1 = QTouch.touchNum();
  delay(500);
  
  Serial.println("Waiting for second num");
  while (QTouch.touchNum() == -1 || QTouch.touchNum() > 3);
  int num2 = QTouch.touchNum();
  delay(500);
  
  Serial.println("Waiting for third num");
  while (QTouch.touchNum() == -1 || QTouch.touchNum() > 3);
  int num3 = QTouch.touchNum();
  delay(500);
  
  if (num1 == 2 && num2 == 1 && num3 == 0) {
    Serial.println("Correct!");
  }
  else {
    Serial.println("Incorrect!");
    
    setOutput(HIGH);
    delay(1000);
    setOutput(LOW);
    delay(1000);
  }

}

void setOutput(int in) {
  digitalWrite(15, in);
  digitalWrite(16, in);
  digitalWrite(14, in);
  digitalWrite(A5, in);
  digitalWrite(3, in);
  digitalWrite(2, in);
  digitalWrite(0, in);
  digitalWrite(1, in);
  
  if (in == 1)
    analogWrite(A5, 128);
  else
    digitalWrite(A5, 0);
}
