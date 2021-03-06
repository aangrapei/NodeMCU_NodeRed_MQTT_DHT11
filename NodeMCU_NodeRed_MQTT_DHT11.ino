/*
 * ==============================NODEMCU MQTT NODE-RED=================================
 * 
 * ===============================SENSOR SUHU DHT 11===================================
 * 
 * ==================================AANG RAPE'I=======================================
 * 
 * ==================================27-01-2021========================================
 * 
 * ============================= KONEKSI PIN DAN DHT11 ==============
`* D7 --> OUT
 * 3v --> +
 * G  --> -
`* 
 */
#include <ArduinoJson.h> //Library JSON
#include <ESP8266WiFi.h> //Library  ESP8266
#include<PubSubClient.h> //Library MQTT
#include <dht.h> //Library DHT11
dht DHT;

#define DHTPIN D7

//Setup WIFI
const char*   ssid        = "XXX"; //Nama SSID Wifi yang akan diakses!
const char*   pass        = "XXX"; //Password Wifi

//Setup MQTT broker
const char*   mqtt_server = "broker.hivemq.com"; //Server MQTT/Broker
const int     mqtt_port   = 1883; //MQTT Port
const char*   mqttuser    = ""; //MQTT Username 
const char*   mqttpass    = ""; //MQTT Password
String        clientId    = "ESP8266Client-"; //MQTT ClientID

//Setup pin lampu
const int lamp1 = D5;
const int lamp2 = D6;
const int lamp3 = D7;

//Variabel status lampu
int stat_lamp1 = 0;
int stat_lamp2 = 0;
int stat_lamp3 = 0;

WiFiClient espclient;
PubSubClient client(espclient);

//Fungsi Menyambungkan ke Wifi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


//Fungsi Menyambungkan Ulang ke MQTT Broker
void reconnect() {
  while (!client.connected()) {
    clientId += String(random(0xffff), HEX);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(mqtt_server);
    if (client.connect(clientId.c_str(), mqttuser, mqttpass )) {
      Serial.println("connected");
    } else {
      Serial.print("failed with state, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

//Fungsi ini dijalankan sekali ketika NodeMCU mulai start
void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

//Fungsi ini dijalakan berulang-ulang selama NodeMCU menyala
void loop(){
   
 if (!client.connected()) { //menyambungkan kembali jika terputus dengan MQTT Broker
    reconnect();
 }
 
 if(!client.loop()){ //menyambungkan kembali menerima pesan jika terputus dengan MQTT Broker
    client.connect("AangESP8266Client",  mqttuser, mqttpass );
 }


  DHT.read11(DHTPIN);
  Serial.print("Kelembaban udara = ");
  Serial.print(DHT.humidity);
  Serial.print("% ");
  Serial.print("Suhu = ");
  Serial.print(DHT.temperature); 
  Serial.println(" C ");
  
DynamicJsonDocument doc(100);

doc["hum"] = DHT.humidity;
doc["temp"] = DHT.temperature;

char buffer[256];
size_t n = serializeJson(doc, buffer);
client.publish("dari_alat", buffer, n); //Publish/kirim pesan ke MQTT broker
delay(2000);

}
