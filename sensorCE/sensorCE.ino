#include <HTTPClient.h>
#include "time.h"
#include <OneWire.h>                
#include <DallasTemperature.h>
 
OneWire ourWire(4);                //Se establece el pin 2  como bus OneWire
 
DallasTemperature sensors(&ourWire); 

const char* ssid = "FABLAB";         // change SSID
const char* password = "#$IICA2019$#";
// Example for demonstrating the TSL2591 library - public domain!
String GOOGLE_SCRIPT_ID = "AKfycby7NaATH8En6tVgyTwe7p_apH7Fl_vJTdCfYDvZiuN2IlWxsVMZqX8zGAzWqFjbwWzX";
long interval = 30000;
long tiempo;
float r2 = 0;

// Potenciometro conectado a GPIO 35
const int portPin = 34;
// Almacenamiento del valor de puerto (Rango de 0-4095)
int valorPin = 0;
float constante = 1;
float ce = 0;
float ceT = 0;
float temperatura;
String EC="";
float ecTotal = 0;
char inchar;
bool dato=true;
bool pol = false;
int cont = 0;

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  tiempo = millis();
  sensors.begin();
  pinMode(33,OUTPUT);
  pinMode(14,OUTPUT);
  delay(1000);
}
void loop() {
  // Lectura del valor en cada vuelta del bucle
    // Serial.println(pol);
    // Serial.println(!pol);
    
    digitalWrite(33,pol);
    digitalWrite(14,!pol);
    delay(100);    
    valorPin = analogRead(portPin);
    Serial.println(analogRead(portPin));
    ce = float(map(valorPin, 0, 4095, 0, 3300))/1000; 
    Serial.println(String(pol)+"-------------------");       
        
    if(pol){
      ce = 3.3 -ce;
      r2 = 0.917;
    }else{
      r2 = 0.982; 
    }
    Serial.println(ce,7);
    ce= (1.0/r2)*((3.3/ce)-1.0)*constante;
    if(!pol){
      ce = (13.67*ce-1)/13.67;
      if(ce<0){
        ce  = 0;      
      }
    }    
    Serial.println(ce,7);
    Serial.println(1/ce);
    ceT += ce;
    pol = !pol;

    EC="";
    dato = true;
    //Serial.println(temperatura);
    while(dato){
      if (Serial2.available() > 0) {                     //if we see that the Atlas Scientific product has sent a character
        inchar = Serial2.read(); 
        if(inchar == '\r'){
          dato = false;
        }else{
          EC  += inchar; 
        }
            
      }
    }
    ecTotal += EC.toFloat();
    
    cont ++;

  Serial.println(EC);
 if (WiFi.status() == WL_CONNECTED && millis()-tiempo>interval) {
    sensors.requestTemperatures(); 
    temperatura = sensors.getTempCByIndex(0);
    ceT = ceT/cont;
    ecTotal = ecTotal/cont;
    //Serial.println(ceT,7);  //Envío del valor al puerto serie
    
    //Serial.println(EC);
    static bool flag = false;    
    //String urlFinal = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?"+"ce=" + String(ecTotal) + "&temperatura=" + String(temperatura)+ "&g=" + String( ceT,7);
    
    String urlFinal = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?"+"ce=" + String(ecTotal) + "&temperatura=" + String(temperatura)+ "&g=" + String( ceT,7);
    
                        // Serial.print("POST data to spreadsheet:");
                        // Serial.println(urlFinal);
    HTTPClient http;
    http.begin(urlFinal.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET(); 
                // Serial.print("HTTP Status Code: ");
                // Serial.println(httpCode);
    //---------------------------------------------------------------------
    //getting response from google sheet
    String payload;
    if (httpCode > 0) {
        payload = http.getString();
                                //Serial.println("Payload: "+payload);    
    }
    //---------------------------------------------------------------------
    http.end();
    tiempo = millis();
    ceT = 0 ;
    ecTotal = 0;
    cont = 0;
  }
}
