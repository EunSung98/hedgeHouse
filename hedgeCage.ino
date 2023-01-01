#include <ArduinoJson.h>
#include <DHT.h>
#define DHTPIN 10
#define DHTTYPE DHT22 //DHT타입
DHT dht(DHTPIN,DHTTYPE);

#define COUNTS 10 // 센서값 배열

boolean start = true;
int i = 0;
int size = 0;

int tempArray[COUNTS] = {};
int humidArray[COUNTS] = {};
int luxArray[COUNTS] = {};

int tempSum = 0;
int humidSum = 0;
int luxSum = 0;

int tempAvg = 0;
int humidAvg = 0;
int luxAvg = 0;

// first moter
#define curmoter1 8
#define curmoter2 9
// second moter
#define curmoter3 12
#define curmoter4 13

#define relay1 7 //heater
#define relay2 6 
#define relay3 5 //cooler
#define relay4 4 //humidifier

//magnetic sensor
int dMag1 = 2; // 센서 디지털 인터페이스
int dMag2 = 3; // 2번째 센서
int dMagVal1 ; // digital readings
int dMagVal2 ; // 2번째 센서digital readings

#define ON  true
#define OFF false

//String curtain_state = "open";
volatile bool curtain_state = false;
String feed_state = "nomove";
bool cooler_state = OFF;
bool heater_state = OFF;
bool humidifier_state = OFF;

// 수동 onoff
int manual = 1;

// 상태 조절
volatile bool curtain_open;
volatile bool curtain_close;
volatile bool feed_open;
bool heater_on;
bool heater_off;
bool cooler_on;
bool cooler_off;
bool humidifier_on;
bool humidifier_off;

int cooler;
int heater;
int curtain;
int humidifier;
int feeder;
   
void HallA() {
    if (curtain_state) STOP();
}
void HallB() {
    if (!curtain_state) STOP();
}

void avg(){
  tempSum = 0;
  humidSum = 0;
  luxSum = 0;
  
  if(start) size = i;
  else size = COUNTS;
  
  for(int j=0; j < size; j++){
    tempSum += tempArray[j];
    humidSum += humidArray[j];
    luxSum += luxArray[j];
  }
  tempAvg = tempSum/size;
  humidAvg = humidSum/size;
  luxAvg = luxSum/size;
}

void CW() {   //Clock-wise : open
  analogWrite(curmoter1, 0); 
  analogWrite(curmoter2, 255); 
  analogWrite(curmoter3, 0);  
  analogWrite(curmoter4, 255);   
}

void CCW() {  //counter-clock-wise : close
  analogWrite(curmoter1, 255);
  analogWrite(curmoter2, 0); 
  analogWrite(curmoter3, 255);
  analogWrite(curmoter4, 0);
}

void STOP() {
  analogWrite(curmoter1, 0);
  analogWrite(curmoter2, 0);
  analogWrite(curmoter3, 0);  
  analogWrite(curmoter4, 0); 
}

void initialRelay(){ /*relay는 low일때 켜지므로 high로 초기화*/
  digitalWrite(relay1,HIGH);
  digitalWrite(relay2,HIGH);
  digitalWrite(relay3,HIGH);
  digitalWrite(relay4,HIGH);  
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(19200);

  /*dht 센서 작동 시작*/
  dht.begin(); 
  
  pinMode(curmoter1, OUTPUT); 
  pinMode(curmoter2, OUTPUT);           
  pinMode(curmoter3, OUTPUT);       
  pinMode(curmoter4, OUTPUT);          

  /*magnetic*/
  pinMode (dMag1, INPUT); 
  pinMode (dMag2, INPUT); 

  int dMagVal1 = 0;
  int dMagVal2 = 0;

  attachInterrupt (digitalPinToInterrupt(dMag1), HallA, FALLING);
  attachInterrupt (digitalPinToInterrupt(dMag2), HallB, FALLING);

//사료배급기
  pinMode(11, OUTPUT);
  digitalWrite(11, LOW);
  
  /*relay 출력모드 설정*/
  pinMode(relay1,OUTPUT); //cooler
  pinMode(relay2,OUTPUT); 
  pinMode(relay3,OUTPUT); //heater
  pinMode(relay4,OUTPUT); //humidifier

  STOP();
  /*relay 초기화*/
  initialRelay(); 
}

StaticJsonDocument<256> jsonBuffer;

void loop() {
  char c = 0;
  int lux;
  int temp;
  int humid;

  String controlJson;

  if(Serial.available()){
    c = Serial.read();
  }
  
  if(Serial2.available()){
   controlJson = Serial2.readStringUntil('\n');
   
   DeserializationError error = deserializeJson(jsonBuffer, controlJson.c_str());
   Serial.println(controlJson);
   
   if (error) Serial.println("Deserialization Error");
   
   manual = jsonBuffer["m"];

   Serial.print("Manual Mode : ");
   Serial.print(manual);

   cooler = jsonBuffer["c"];
   heater = jsonBuffer["h"];
   curtain = jsonBuffer["cu"];
   humidifier = jsonBuffer["hu"];
   feeder = jsonBuffer["f"];
  }

  if (manual) {
    Serial.println("Manual Mode");
    initialRelay();

    if(cooler == 1)
      c = '1';
    else
       c = '5';
    if(heater == 1)
       c = '3';
    else
       c = '7';
    if(humidifier == 1)
       c = '4';
    else
       c = '8';
    if(curtain == 1)
       c = 'o';
    else
       c = 'c';
    if(feeder == 1)
       c = 'f';
  } else {
    Serial.println("Auto mode run.");
    lux = analogRead(A0);
    temp = dht.readTemperature();  
    humid = dht.readHumidity();

    tempArray[i] = temp;
    humidArray[i] = humid;
    luxArray[i] = lux;

    i = (i+1) % COUNTS;
    if (start && i == (COUNTS - 1)) start = false;

    avg();

    if(manual == false){ 
      if(luxAvg < 600 && curtain_open == OFF) { 
        c = 'c';}
      if(luxAvg > 800 && curtain_open == ON) {
        c = 'o';}
        
      if(humidAvg < 40 && humidifier_state == OFF) {
        c = '4';} //humidifier on
      if(humidAvg >= 40 && humidifier_state == ON) {
        c = '8';} //humidifier off
      if(tempAvg > 28 && cooler_state == OFF) {
        c = '1';} // cooler on
      if(tempAvg < 25 && heater_state == OFF) {
        c = '3';} // heater on
      if(tempAvg < 27 && cooler_state == ON) {
        c = '5';}   // cooler off
      if(tempAvg > 26 && heater_state == ON) {
        c = '7';} // heater off
    }
  }

  
  Serial.print("Command input : ");
  Serial.println(c);

  switch(c) {
    // 릴레이 조절
    case '0': //전체 끄기
      initialRelay(); 
      break;
    case '1': //cooler
      cooler_on = true;
      break;
    case '2': // no use
      digitalWrite(relay1, LOW);
      break;
    case '3': //heater
      heater_on = true;
      break;
    case '4': //humidifier
      humidifier_on = true;
      break;
    case '5':
      cooler_off = true;
      break;
    case '6': // no use
      digitalWrite(relay1, HIGH);
      break;
    case '7':
      heater_off = true;
      break;
    case '8':
      humidifier_off = true;
      break;  
    case 'm':
      manual = true;
    case 'o' :
      curtain_open = true;
      break;
    case 'c' :
      curtain_close = true;
      break;
    case 's' :
      STOP();
      break;
    case 'f':
      feed_open = true;
      break;  
    default :
      break;
  }

// [ 부품 조절 ]
   if(feed_open == true){
      digitalWrite(11,HIGH);
      delay(200);
      digitalWrite(11,LOW);
      feed_open = false;
    }
    if(curtain_open == true){
     CW();
     curtain_open = false;
     curtain_state = true;
    }
    if(curtain_close == true){
     CCW();
     curtain_close = false;
     curtain_state = false;
    }
    if(cooler_on == true){
     digitalWrite(relay2, LOW);
     cooler_state = ON;
    }
    if(cooler_off == true){
      digitalWrite(relay2, HIGH);
      cooler_state = OFF;
    }
    if(heater_on == true){
     digitalWrite(relay3, LOW);
     heater_state = ON;
    }
    if(heater_off == true){
      digitalWrite(relay3, HIGH);
      heater_state = OFF;
    }
    if(humidifier_on == true){
     digitalWrite(relay4, LOW);
     humidifier_state = ON;
    }
    if(humidifier_off == true){
      digitalWrite(relay4, HIGH);
      humidifier_state = OFF;
    }
    delay(500);
  
    char valueJson[200];
  
    sprintf(valueJson, "{\"temp\":%3d,\"humid\":%3d,\"lux\":%4d}\n", tempAvg, humidAvg, luxAvg);
    
    Serial2.write(valueJson);
    delay(1000);
}
