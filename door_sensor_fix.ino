
#include <ESP8266WiFi.h>


const char* ssid = "wifi";
const char* password = "66666666";
const char* host = "maker.ifttt.com";
const char* apiKey = "gBIkZA5PzjOKc-uuxa_KoviHzUySPSaO2krkSAKu6En";

#define pin 14
volatile byte state = LOW;
volatile int flag = false;
const char* door_state;
unsigned long previousMillis = 0; 
const long interval = 3000;


void changeDoorStatus() {

    unsigned long currentMillis = millis();
 
    if(currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;   

    state = !state;
        if(state) {
            door_state = "opened";
             Serial.println(door_state);   
             pinMode(D1, OUTPUT);
             analogWrite(D1, 255);
             pinMode(D2, OUTPUT);
             analogWrite(D2, 255);
             pinMode(D3, OUTPUT);
             analogWrite(D3, 255);
             pinMode(D4, OUTPUT);
             analogWrite(D4, 255);
        }
        else{
            door_state = "closed";
             Serial.println(door_state);
              ESP.restart();
        }
    }
}   
    
void setup() {
    Serial.begin(115200);
    delay(100);
    Serial.println("Preparing the Door Status Monitor project...");
     
   pinMode(pin, INPUT_PULLUP);
   attachInterrupt(pin, changeDoorStatus, RISING);
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  
    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());  
}

void loop() { 
      
      
      
      if(flag = true ){

          Serial.print("connecting to ");
          Serial.println(host);
          
          WiFiClient client;
          const int httpPort = 80;
          if (!client.connect(host, httpPort)) {
            Serial.println("connection failed");
            return;
          }
    
          String url = "/trigger/door_status/with/key/";
          url += apiKey;
          
          Serial.print("Requesting URL: ");
          Serial.println(url);
          client.print(String("POST ") + url + " HTTP/1.1\r\n" +
                       "Host: " + host + "\r\n" + 
                       "Content-Type: application/x-www-form-urlencoded\r\n" + 
                       "Content-Length: 13\r\n\r\n" +
                       "value1=" + door_state + "\r\n");
          flag = false;
      }  
      delay(5000);
}
