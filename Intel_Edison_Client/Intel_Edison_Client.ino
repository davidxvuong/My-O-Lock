#include "Seeed_QTouch.h"
#include <Wire.h>
#include <WiFi.h>

#define MAX_BUFFER_SIZE 50

int counter = 0;
char ssid[] = "The D";
char pass[] = "Microsoft1328";

//network information of server (arduino expansion)
IPAddress ip (192, 168, 43,75);
IPAddress gateway (192, 168, 1,1);
IPAddress subnet (255,255,255,0);
WiFiClient client;

char wifi_server_buffer[MAX_BUFFER_SIZE] = {'\0\}'};
int wifi_server_buffer_top_index = 0;

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
  WiFi.begin(ssid, pass);
  Serial.begin(9600);
}

int get_wifi_server_input_until_cr(WiFiClient &client, char *buff, int buff_size) {
  for (int i =0; i < buff_size && client.available() > 0; i++) {
    char c = client.read();
    wifi_server_buffer[i] = c;
    wifi_server_buffer_top_index++;
    if ('\r' == c) {
      return i+1;
    }
  }
}

void flush_wifi_server_input_buffer() {
  memset(wifi_server_buffer, '\0', wifi_server_buffer_top_index);
  wifi_servver_buffer_top_index = 0;
}

void loop() {
  
  client.connect(ip,3000);
  
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
    //send true over to server
    client.print("true\r");
  }
  else {
    //send false to server
    client.print("false\r");
  }
  
  while (!client.available());
  int chars_copied = get_wifi_server_input_until_cr(client, &wifi_sever_buffer[0], MAX_BUFFER_SIZE);
  
  if ('\r' == wifi_server_buffer[wifi_server_buffer_top_index]) {
    char *tok = strtok(wifi_server_buffer, "\r");
    
    if (NULL != tok) {
      if (strcmp("false", wifi_server_buffer) {
        setOutput(HIGH);
        delay(1000);
        setOutput(LOW);
        delay(1000);
      }
    }
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
