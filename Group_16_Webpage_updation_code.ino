#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
String readString;
const char* ssid = "SSID";  // Wifi network user id
const char* password = "Password"; // wifi network password
const char* host = "script.google.com";
const int httpsPort = 443;
WiFiClientSecure client;
const char* fingerprint = "46 B2 C3 44 9C 59 09 8B 01 B6 F8 BD 4C FB 00 74 91 2F EF F6";
String GS_ID = "Put your GS ID here";  // Replace by your GAS service id
String slot_1 = "Available";  

#define slot_1_available D0 // HIGH when slot is available , otherwise LOW
#define slot_1_busy D1 // HIGH when slot is busy , otherwise LOW
#define slot_1_occupied D2 // HIGH when slot is occupied , otherwise LOW

void setup()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  pinMode(D0, INPUT);
  pinMode(D1, INPUT);
  pinMode(D2, INPUT);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }       
}
void loop()
{
  if (digitalRead(slot_1_available)== HIGH){
    // The slot is available 
    slot_1 = "Available";
    sendData(slot_1,"Slot1")
  }
  else if (digitalRead(slot_1_busy)== HIGH){
    // The slot is busy
    slot_1 = "Busy";
    sendData(slot_1,"Slot1")
  }
  else if (digitalRead(slot_1_occupied)== HIGH){
    // The slot is occupied
    slot_1 = "Occupied";
    sendData(slot_1,"Slot1")
  }
  else{
    continue;
  }
}

// x is the status and y is the sheet id
// function to update the spreadsheet 
void sendData(String x, String y)
{
 client.setInsecure();
 Serial.print("connecting to ");
 Serial.println(host);
 if (!client.connect(host, httpsPort)) {
   Serial.println("connection failed");
   return;
 }
 if (client.verify(fingerprint, host)) {
 Serial.println("certificate matches");
 } else {
 Serial.println("certificate doesn't match");
 }
 String url = "/macros/s/" + GS_ID + "/exec?id=" + y + "&status=" + x;
 Serial.print("requesting URL: ");
 Serial.println(url);
 client.print(String("GET ") + url + " HTTP/1.1\r\n" +
        "Host: " + host + "\r\n" +
        "User-Agent: BuildFailureDetectorESP8266\r\n" +
        "Connection: close\r\n\r\n");
 Serial.println("request sent");
 while (client.connected()) {
 String line = client.readStringUntil('\n');
 if (line == "\r") {
   Serial.println("headers received");
   break;
 }
 }
 String line = client.readStringUntil('\n');
 if (line.startsWith("{\"state\":\"success\"")) {
 Serial.println("esp8266/Arduino CI successfull!");
 } else {
 Serial.println("esp8266/Arduino CI has failed");
 }
 Serial.println("reply was:");
 Serial.println("==========");
 Serial.println(line);
 Serial.println("==========");
 Serial.println("closing connection");
}
