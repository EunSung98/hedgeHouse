#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

#include <ArduinoJson.h>

/* 2. If work with RTDB, define the RTDB URL and database secret */
#define DATABASE_URL "firebase주소"
#define DATABASE_SECRET "pw"

/* 1 wifi  */
#define WIFI_SSID "id"
#define WIFI_PASSWORD "pw"

/* 3. Define the Firebase Data object */
FirebaseData fbdo;
FirebaseData cageControl;

int manual = 0;

int cooler = 0;
int heater = 0;
int humidifier = 0;
int curtain = 0;
int feeder = 0;

int temp;
int lux;
int humid;

void setup() {
  //Serial1 케이지 제어 & Serial2 파이어베이스 전송
  Serial.begin(115200);
  Serial2.begin(115200);
  
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Firebase.reconnectWiFi(true);

  Firebase.begin(DATABASE_URL,DATABASE_SECRET);
  Serial.println("ESP ready.");
}

StaticJsonDocument<256> jsonBuffer;

void loop() {
  String From2560;
  char controlJson[128];

  Firebase.getInt(cageControl, F("/cageControl/manual"));
  manual = cageControl.to<int>();
  Firebase.getInt(cageControl, F("/cageControl/cooler"));
  cooler = cageControl.to<int>();
  Firebase.getInt(cageControl, F("/cageControl/heater"));
  heater = cageControl.to<int>();
  Firebase.getInt(cageControl, F("/cageControl/curtain"));
  curtain = cageControl.to<int>();
  Firebase.getInt(cageControl, F("/cageControl/humidifier"));
  humidifier = cageControl.to<int>();
  Firebase.getInt(cageControl, F("/cageControl/feed"));
  feeder = cageControl.to<int>();

  //json 형식으로 보내야 함
  Serial.printf("%d %d %d %d %d %d\n", manual,cooler,heater,humidifier,curtain,feeder);
  
  if(Serial2.available()){
   From2560 = Serial2.readStringUntil('\n');
   Serial.println(From2560);
   DeserializationError error = deserializeJson(jsonBuffer, From2560.c_str());

   if (error) Serial.println("Deserialization Error");

   temp = jsonBuffer["temp"];
   lux  = jsonBuffer["lux"];
   humid = jsonBuffer["humid"];

   Firebase.setInt(fbdo,"/cageStatus/temp",temp);
   Firebase.setInt(fbdo,"/cageStatus/lux",lux);
   Firebase.setInt(fbdo,"/cageStatus/humid",humid);
  }
    
    sprintf(controlJson,
      "{\"m\":%d,\"c\":%d,\"h\":%d,\"hu\":%d,\"cu\":%d,\"f\":%d}\n",
      manual,cooler, heater, humidifier, curtain,feeder);
    
    Serial2.printf("%s",controlJson);
    
    delay(1000);
}
