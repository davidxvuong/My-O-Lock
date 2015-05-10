#include <WiFi.h>

#define MAX_BUFFER_SIZE 50

// TODO: Fill in Mac address
byte mac[] = { 0
};

//network information of client (xadow)
IPAddress ip(192, 168, 43, 47);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

WiFiServer server(3000);

char *ssid = "The D";
char *pass = "Microsoft1328";

char wifi_client_buffer[MAX_BUFFER_SIZE] = {'\0'};
int wifi_client_buffer_top_index = 0;

boolean alreadyConnected = false; 

void setup() {
  WiFi.begin(ssid, pass);
  server.begin();
  
  Serial.begin(9600);
}

int get_wifi_client_input_until_cr(WiFiClient &client, char *buff, int buff_size) {
  
  int i;
  for(i = 0; i < buff_size && client.available() > 0; i++) {
    
    char c = client.read(); 
    wifi_client_buffer[i] = c;
    wifi_client_buffer_top_index++;
    if('\r' == c) {
      return i+1;
    }
  }
  return i;
}

void flush_wifi_client_input_buffer() {
  memset(wifi_client_buffer, '\0', wifi_client_buffer_top_index);
  wifi_client_buffer_top_index = 0;
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    if (!alreadyConnected) {
      client.flush();    
      alreadyConnected = true;
    }
    
    if (client.available() > 0) {
      int chars_copied = get_wifi_client_input_until_cr(client, &wifi_client_buffer[0], MAX_BUFFER_SIZE);
      
      //command has been entered; process it
      if('\r' == wifi_client_buffer[wifi_client_buffer_top_index]) {
        
        char *tok = strtok(wifi_client_buffer, "\r");
        
        if (NULL != tok) {
          
          if(0 == strcmp(wifi_client_buffer, "open")) {
            //Success
          }
          
          else if(0 == strcmp(wifi_client_buffer, "close")) {
            //Failure
          }
          
          //not sure what this command is
          else {
            
          }
        }
        
        flush_wifi_client_input_buffer();
      }
    }
  }
}
