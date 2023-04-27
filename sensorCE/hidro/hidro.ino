#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLn0diOy0A"
#define BLYNK_TEMPLATE_NAME "Soltek"
#define BLYNK_AUTH_TOKEN "QwCEj1uHdHau7GgTxCREWNqfs9O8-rew"

#define n5 4096
#define n4 1300
#define n3 1000
#define n2 650
#define n1 450


#define intervalo 900000

#include <BlynkSimpleEsp32.h>
#include <WiFi.h>

const char* ssid = "FABLAB";         // change SSID
const char* pass = "#$IICA2019$#";
char auth[] = BLYNK_AUTH_TOKEN;

unsigned long previousMillis = 0;
unsigned long interval = 30000;

int nivel = 1;
int sensor = 0;
bool listo = false;
bool bomba = true;
long time1 = 0;
bool wifiC = true;

void setup() {
  Serial.begin(115200);
  //put your setup code here, to run once:
  Blynk.config(auth);
  //Blynk.connectWiFi(ssid, pass);
  WiFi.begin(ssid, pass);
  //Blynk.begin(auth, ssid, pass);
  time1 = millis();
  pinMode(26,OUTPUT);
  pinMode(25,OUTPUT);
  
}

void loop() {
   if ((WiFi.status() != WL_CONNECTED) && (millis() - previousMillis >=interval)) {
    Serial.print(millis());
    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.reconnect();
    previousMillis = millis();
  }
  Blynk.run();
  if(bomba){
      digitalWrite(25,HIGH);
    }else{
      digitalWrite(25,LOW);
    }
  if(millis()-time1>intervalo){
      time1 = millis();
    if(bomba){
      bomba= false;
    }else{
      bomba= true;
    }
  }
  digitalWrite(26,HIGH);
  
  sensor = analogRead(32);
  // delay(500);
  // digitalWrite(25,LOW);
  // delay(500);
  Serial.println(sensor);
  listo = false;
  Blynk.virtualWrite(V0, nivel);
  Blynk.virtualWrite(V1, bomba);
  // put your main code here, to run repeatedly:
  
  if(sensor < n1 && !listo){
    nivel = 1;
    listo = true;
  }
  if(sensor < n2 && !listo){
    nivel = 2;
    listo = true;
  }
  if(sensor < n3 && !listo){
    nivel = 3;
    listo = true;
  }
  if(sensor < n4 && !listo){
    nivel = 4;
    listo = true;
  }
  if(sensor < n5 && !listo){
    nivel = 5;
    listo = true;
  }
  Serial.println(nivel);
  delay(1500);
}
