#include <string.h>

int pin = 2;
int led_state = 0;
String s = "";

const int INPUT_BUFFER_LENGTH = 100;
char input_buffer[INPUT_BUFFER_LENGTH] = {'\0'};
char *input_ptr = input_buffer;

String content = "";

struct led_action{
  int pin;
  bool state;  
};

void flush_input_buffer() {
   memset(input_buffer, 0, INPUT_BUFFER_LENGTH);
   input_ptr = input_buffer;
}

struct led_action parse_led_action(char *input, int len) {
  char buff[INPUT_BUFFER_LENGTH] = {0};
  memcpy(buff, input, len);
  
  struct led_action a = {-1, 0}; //default error state

  char *pin_str;
  pin_str = strtok(buff, " ");
  
  if(NULL == pin_str) return a;
  
  char * state_str = strtok(NULL, " ");
  
  if(NULL == state_str) return a;
  
  a.pin = atoi(pin_str);
  
  if(0 == strcmp(state_str, "on")) {
    a.pin = 1;
  }
  else {
    a.pin = 0; 
  }
  
  return a;
}

// #######################

void setup() {
  Serial.begin(115200);
  pinMode(pin, OUTPUT);
  pinMode(7, OUTPUT);
  int i;
  for(i = 0; i < 8; i++) {
    pinMode(i, OUTPUT); 
  }
  //struct led_action a = parse_led_action("2 on\0", 5);
  //digitalWrite(a.pin, a.state);
  
}

void loop() {
  
  while (Serial.available() > 0) {
    //byte iByte = Serial.read(); 
    //Serial.write(iByte);
    //digitalWrite(pin, (led_state = !led_state));
    
    char c = (char)Serial.read();
    if ('\r' == c) {
      Serial.print(input_buffer);
      delay(10);
      if (0 == strcmp(&input_buffer[0], "success")) {
        digitalWrite(3, HIGH);
      }
      
      pin = atoi(&input_buffer[0]);

      if(0 == strcmp(&input_buffer[2], "on")) {
        //Serial.println(pin);
        //Serial.println(": on.");
        digitalWrite(pin, HIGH);
      }
      else if(0 == strcmp(&input_buffer[2], "off")) {
        //Serial.print(pin);
        //Serial.println(": off.");
        digitalWrite(pin, LOW);
      }
      else{
        //Serial.println(pin);
        //Serial.println(": no change.");
      }
      
      flush_input_buffer();
    }
    else {
      *input_ptr = c;
      ++input_ptr;
    }
  }
}
