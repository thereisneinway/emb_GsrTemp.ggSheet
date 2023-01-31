#include <OneWire.h>
#include <DallasTemperature.h>
#include "WiFi.h"
#include <HTTPClient.h>
const char* ssid = "NBAX";        
const char* password = "14235678"; 
String GOOGLE_SCRIPT_ID = "AKfycbzMhO8yS5M842qRPvpNMYk2WwaslMNab6D4iMORlvRZZ6Qlx4SSi_mp1jVLdK3SJL3t";

const char* ntpServer = "pool.ntp.org";

OneWire oneWire(2);
DallasTemperature sensors(&oneWire);
int St = 0;
int Sg = -1;

void setup() {
  Serial.begin(9600);
  delay(1000);
  // connect to   
  Serial.println();
  Serial.print("Connecting to wifi: ");
  Serial.println(ssid);
  Serial.flush();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  configTime(25200 , 0, ntpServer);
  sensors.begin();
}
void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    struct tm timeinfo;
    getLocalTime(&timeinfo);
    char timeStringBuff[50];
    strftime(timeStringBuff, sizeof(timeStringBuff), "%H:%M:%S", &timeinfo);
    String asString(timeStringBuff);

    St = sensors.getTempCByIndex(0);
    Sg = analogRead(32);
    sensors.requestTemperatures();

    String urlFinal = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?"+ "date=" + asString + "&temp=" + St + "&gsr=" + Sg;
    Serial.printf("%s Temp=%d GSR=%d\n",asString,St,Sg);
    HTTPClient http;
    http.setTimeout(500);
    http.begin(urlFinal.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET(); 
    Serial.print("HTTP Status Code: ");
    Serial.println(httpCode);
    http.end();
  }
  delay(1000);
} 