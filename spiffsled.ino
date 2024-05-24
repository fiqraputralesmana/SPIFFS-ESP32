#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

const char* ssid = "MyWifi";
const char* password = "12345678";
AsyncWebServer server(80);
String ledState;

String processor(const String& var){
  if(var == "STATE"){
    if(digitalRead(2)){
      ledState = "checked";
    }
    else{
      ledState = "";
    }
    return ledState;
  }
  return String();
}

void setup() {
  pinMode(2, OUTPUT);
  Serial.begin(115200);
  if(!SPIFFS.begin(true)){
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
    }
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
   Serial.println(WiFi.localIP());
   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
     request->send(SPIFFS, "/index.html", String(), false, processor);
  });
   server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String parameter;
    // GET input1 value on <ESP_IP>/update?state=<inputMessage>
    if (request->hasParam("state")) {
      parameter = request->getParam("state")->value();
      digitalWrite(2, parameter.toInt());
    }
   
    request->send(200, "text/plain", "OK");
  });
    server.begin();
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
