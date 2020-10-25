/*********
  (주)엠에스세미콤
  Leak Sensor   
*********/

#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

const char* ssid     = "leak";
const char* password = "passme19!";
//const char* ssid     = "PASSME_2G";
//const char* password = "passme19!";

const char* server = "api.thingspeak.com";
String apiKey = "LA8VTVNIGH44CN09"; // thingspeak 채널의 API key

const int output1 = 14;
const int output2 = 12;
const int output3 = 15;

const int output10 = 1;  //2

const int input1 = 5;
const int input2 = 4;
const int input3 = 13;

unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;
int Tcheck = 0;

int ADC_Value = 0;

int inVal1 = 0;
int inVal2 = 0;
int inVal3 = 0;

int in2Val1 = 0;
int in2Val2 = 0;
int in2Val3 = 0;

int in3Val1 = 0;
int in3Val2 = 0;
int in3Val3 = 0;

int s1 = 0;
int s2 = 0;
int s3 = 0;

String ONDO = "0";

void setup() {
  
  Serial.begin(57600);

  pinMode(output1, OUTPUT);
  pinMode(output2, OUTPUT);
  pinMode(output3, OUTPUT);

  // Relay
  pinMode(output10, OUTPUT);
  digitalWrite(output10, LOW);
  
  pinMode(input1, INPUT);
  pinMode(input2, INPUT);
  pinMode(input3, INPUT);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  //server.begin();

}

void loop(){
  WiFiClient client;
  
  digitalWrite(output1, LOW);
  digitalWrite(output1, LOW);
  digitalWrite(output1, LOW);
  
  inVal1 = digitalRead(input1);
  inVal2 = digitalRead(input2);
  inVal3 = digitalRead(input3);

  Serial.println(inVal1);
  Serial.println(inVal2);
  Serial.println(inVal3);
  
  
  ADC_Value = analogRead(A0);
  //ONDO = ADC_Value / 3.3 * 0.14;
  ONDO = analogRead(A0);
  Serial.println(ONDO);
  
  Serial.println("----");
  Tcheck = 0;
  
  if (inVal1 > 0 or inVal2 > 0 or inVal3 > 0){
    in2Val1 = inVal1;
    in2Val2 = inVal2;
    in2Val3 = inVal3;
    
    for (int x = 0; x < 255; x++) {

      Tcheck++;
      inVal1 = digitalRead(input1);
      inVal2 = digitalRead(input2);
      inVal3 = digitalRead(input3);
      if ( inVal1 == 0 and inVal2 == 0 and inVal3 == 0  or Tcheck >= 6 ) {
        x = 0;
        break;
      }
      delay(500);
    }
    
  }


  //if (inVal1 > 0 or inVal2 > 0 or inVal3 > 0){
  if (Tcheck >= 6){
  
  Tcheck = 0;
  
    digitalWrite(output10, HIGH);
    Serial.println("High");
    delay(5000); 
    
    if (client.connect(server,80)) {  //   "184.106.153.149" or api.thingspeak.com
      String postStr = apiKey;
        postStr +="&field1=";
        postStr += String((int)in2Val1);
        postStr +="&field2=";
        postStr += String((int)in2Val2);
        postStr +="&field3=";
        postStr += String((int)in2Val3);
        postStr +="&field4=";
        postStr += String(ONDO);
   
      client.print("POST /update HTTP/1.1\n"); 
      client.print("Host: api.thingspeak.com\n"); 
      client.print("Connection: close\n"); 
      client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n"); 
      client.print("Content-Type: application/x-www-form-urlencoded\n"); 
      client.print("Content-Length: "); 
      client.print(postStr.length()); 
      client.print("\n\n"); 
      client.print(postStr);  
    }
    client.stop();   
    delay(1000);
  }
  
  in3Val1 = digitalRead(input1);
  in3Val2 = digitalRead(input2);
  in3Val3 = digitalRead(input3);
  if (inVal1 == 0 and inVal2 == 0 and inVal3 == 0){
    digitalWrite(output10, LOW);
    Serial.println("Low");

    if (client.connect(server,80)) {  //   "184.106.153.149" or api.thingspeak.com
      String postStr = apiKey;
        postStr +="&field1=";
        postStr += String((int)in3Val1);
        postStr +="&field2=";
        postStr += String((int)in3Val2);
        postStr +="&field3=";
        postStr += String((int)in3Val3);
        postStr +="&field4=";
        postStr += String(ONDO);
   
      client.print("POST /update HTTP/1.1\n"); 
      client.print("Host: api.thingspeak.com\n"); 
      client.print("Connection: close\n"); 
      client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n"); 
      client.print("Content-Type: application/x-www-form-urlencoded\n"); 
      client.print("Content-Length: "); 
      client.print(postStr.length()); 
      client.print("\n\n"); 
      client.print(postStr);  
    }
    client.stop();   
    delay(1000);

    
  }
  
  
}
