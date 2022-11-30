 #include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  5        /* Time ESP32 will go to sleep (in seconds) */


#define DHTPIN 23     // what digital pin the DHT22 is conected to
#define DHTTYPE DHT22
#define motorpin 4


#define TEMP_TOPIC "/node0/temp"
#define HUMIDITY_TOPIC "/node0/humidity"
#define DISTANCE_TOPIC "/node0/distance"
#define Motor_Control "Motor/state"  //suscribe 
const char* _SSID = "A08N";
const char* _PASSWORD = "130820ayisha";
const char* _mqttServer = "192.168.43.69";
const char* _mqttUsername = "november";
//const char* _mqttPassword = "Develop&mosquitto2001";
const int _mqttPort = 1883;

void _callback(char* topic, byte* payload, unsigned int length) {
  String message="";
  for(int i=0;i<length; i++) message+=(char)payload[i];
  Serial.println("Message arrived["+String(topic)+"]"+message);
  if(strcmp(topic,Motor_Control)==0){
    if(message.equals("on")){
      digitalWrite(motorpin,LOW);
     // Serial.
    } 
    else
    digitalWrite(motorpin,HIGH);
  }
  
}

DHT dht(DHTPIN, DHTTYPE);
WiFiClient _wifiClient;
PubSubClient pubClient(_mqttServer,_mqttPort,_callback,_wifiClient);

RTC_DATA_ATTR int bootCount = 0;
const int trig = 5;
const int echo = 18;
const int relay=4;
//int distance; // variable for the distance measurement
float temperature;
 float humidity;
 float distance;
#define soundSpeed 0.034 //this is in cm/microsseconds



//void print_wakeup_reason(){
//  esp_sleep_wakeup_cause_t wakeup_reason;
//
//  wakeup_reason = esp_sleep_get_wakeup_cause();
//
//  switch(wakeup_reason)
//  {
//    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
//    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
//    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
//    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
//    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
//    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
//  }
//}


void setup() {
  // put your setup code here, to run once:

 Serial.begin(115200);
 _initWifi();
  _initMqttClient();
  dht.begin();

 pinMode(trig, OUTPUT); // Sets the trigPin as an OUTPUT
  
  pinMode(echo, INPUT); // Sets the echoPin as an INPUT
  pinMode(motorpin, OUTPUT);
    
     
  // ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  //Print the wakeup reason for ESP32
//  print_wakeup_reason();
//
//  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
//  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) + " Seconds");  
//  
}

void loop() {
  // put your main code here, to run repeatedly:
 pubClient.loop();
// _initWifi();
//  _initMqttClient();
 temperature = readtemp();
 humidity = readhumid();
 distance = readdistance();
 
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print(" %\n");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" *C\n");
    
 Serial.print(distance);
 Serial.println("cm");
     _postSensorData();
     delay(10000);
     //pubClient.loop();
 //send http request
 //database("http://172.20.10.12/IOT_php/lab3.php?insert&Distance=57&Temperature="+String(temperature)+"&Humidity="+String(humidity));
//
// Serial.println("Going to sleep now");
//  Serial.flush(); 
//  pubClient.loop();
//  esp_deep_sleep_start();
//  Serial.println("This will never be printed");

}

float readtemp(){
    // Read temperature as Celsius (the default)
return dht.readTemperature(); 
 }

 //function to read hunmidity
 float readhumid(){
   return dht.readHumidity();
 }
//fucntion to read distance
float readdistance(){
digitalWrite(trig,  LOW);
delayMicroseconds(20);
digitalWrite(trig,  HIGH);
delayMicroseconds(10);
digitalWrite(trig,  LOW);

float echoTime = pulseIn(echo,HIGH);
return (soundSpeed * echoTime)/2 ; 
}

void _postSensorData() {
  char tempChar[8];
  char humidityChar[8];
  char distanceChar[8];
  pubClient.publish(TEMP_TOPIC,dtostrf(temperature,5,2,tempChar));
  pubClient.publish(HUMIDITY_TOPIC,dtostrf(humidity,5,2,humidityChar));
  pubClient.publish(DISTANCE_TOPIC,dtostrf(distance,5,2,distanceChar));
}

void _initWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(_SSID,_PASSWORD);
  int count=0;
  Serial.println("Connecting to WIFI...");
  while(WiFi.status() != WL_CONNECTED) {}
  Serial.println("Connected to WIFI");

//  delay(100);
//    count++;
//    Serial.print(".");
//    /* I had some cases where the esp was stuck in connection to wifi mode */
//    if (count > 10) {
//      Serial.println();
//      Serial.println("something bad happened, trying to reset");
//      ESP.restart();
//}
}

void _initMqttClient() {
  Serial.println("Connecting to MQTT Broker...");
  while(!pubClient.connected()) {
    if(pubClient.connect("WH-Node",_mqttUsername,"")) {
         Serial.println("Connected to MQTT");
           pubClient.subscribe(Motor_Control);
    }
  }
}
void reconnect(){
  
}
