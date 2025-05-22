//#################### DEFINE ####################
#define SlaveAddress 0x11
#define SDA_PIN 21
#define SCL_PIN 22



//#################### LIBRARY ###################
#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <math.h>
#include <ArduinoJson.h>



//############ VARIABLES & CONSTANTS #############
const char *ssid = "Kfloor";
const char *password = "Chaongaymoi"; //0888556404

const char *mqtt_broker = "broker.emqx.io";
const char *mqtt_user = "emqx";
const char *mqtt_pass = "public";
const int mqtt_port = 1883;

String clientId = "esp32-" + String(WiFi.macAddress());

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastSend = 0; // Biến dùng millis
const unsigned long interval = 5000;


struct MegaData
{
  float temperature;
  float humidity;
  float lux;
  float ppm;
  float distance;
  bool air_condi_status;
  bool pir_status;
  bool timeup;
  byte padding[1];
};

struct EspData
{
  float hightemperature;
  float lowtemperature;

  uint8_t RedValue;
  uint8_t GreenValue;
  uint8_t BlueValue;
  bool ProtectMode;

  bool GasBuzzerOff;
  bool AutoMode;
  bool RGBOn;
  bool LedOn;

  uint8_t OnTimer;
  byte padding[3];
};

MegaData DataFromMega;
EspData DataFromEsp;

float humidity,temperature;
bool air_condi_status;



//################### PROCEDUCE ##################
void callback(char *topic, byte *payload, unsigned int length);
void callback(char *topic, byte *payload, unsigned int length) 
{
  String msg;
  for (unsigned int i = 0; i < length; i++) 
    msg += (char)payload[i];
  if (strcmp(topic, "Swi_4") == 0) 
  {
    bool on = (msg.equalsIgnoreCase("true"));
    DataFromEsp.ProtectMode = on;
  }
  else if (strcmp(topic, "Switch_4") == 0) 
  {
    bool on = (msg.equalsIgnoreCase("true"));
    DataFromEsp.GasBuzzerOff = on;
  }
  else if (strcmp(topic, "LS_4") == 0) 
  {
    bool on = (msg.equalsIgnoreCase("true"));
    DataFromEsp.RGBOn = on;
  }
  else if (strcmp(topic, "Auto_4") == 0) 
  {
    bool on = (msg.equalsIgnoreCase("true"));
    DataFromEsp.AutoMode = on;
  }
  else if (strcmp(topic, "OnOff_4") == 0) 
  {
    bool on = (msg.equalsIgnoreCase("true"));
    DataFromEsp.LedOn = on;
  }
  else if (strcmp(topic, "aop_4") == 0)
  {
    DataFromEsp.hightemperature = msg.toFloat();
  }
  else if (strcmp(topic, "Ledoff_4") == 0)
  {
    DataFromEsp.OnTimer = msg.toInt();
  }
  else if (strcmp(topic, "aof_4") == 0)
  {
    DataFromEsp.lowtemperature = msg.toFloat();
  }
  else if (strcmp(topic, "LC_4") == 0) 
  {
    StaticJsonDocument<128> doc;
    DeserializationError error = deserializeJson(doc, payload, length);
    if (doc.containsKey("r") && doc.containsKey("g") && doc.containsKey("b")) {
      DataFromEsp.RedValue = doc["r"];
      DataFromEsp.GreenValue = doc["g"];
      DataFromEsp.BlueValue = doc["b"];
    }
    Serial.print(DataFromEsp.RedValue);
    Serial.print(" | ");
    Serial.print(DataFromEsp.GreenValue);
    Serial.print(" | ");
    Serial.println(DataFromEsp.BlueValue);
  }
//   else if (strcmp(topic, "LED/blue") == 0) 
//     digitalWrite(B_PIN, on);
// }
}
void reconnect(); 
void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Loading...");
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass))
    {
      Serial.println("Connected");

      client.subscribe("aof_4");
      client.subscribe("aop_4");
      client.subscribe("LC_4");
      client.subscribe("Swi_4");
      client.subscribe("Switch_4");
      client.subscribe("Auto_4");
      client.subscribe("LS_4");
      client.subscribe("Ledoff_4");
      client.subscribe("OnOff_4");
    }
    else
    {
      delay(5000);
    }
  }
}

void receiveEvent(int howMany)
{
  if (howMany == sizeof(MegaData))
  {
    Wire.readBytes((byte*)&DataFromMega, sizeof(MegaData));
  }
  else
  {
    while (Wire.available())
    {
      Wire.read();
    }
  }
}

void requestEvent()
{
  // truyen
  Wire.write((byte*)&DataFromEsp, sizeof(EspData));
}



//#################### SETUP #####################
void setup() {
  Serial.begin(9600);
  // WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);

  // I2C
  Wire.begin(SlaveAddress, SDA_PIN, SCL_PIN, 100000);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  // Default
  DataFromEsp.hightemperature = 35;
  DataFromEsp.lowtemperature = 29;
}



//#################### MAIN #####################
void loop() {  
  if (!client.connected()) reconnect();
  client.loop();
  unsigned long now = millis();
  if (now - lastSend > interval)
  {
    lastSend = now;
    if (!isnan(temperature)) 
    client.publish("Temp_4", String(DataFromMega.temperature, 2).c_str());
    if (!isnan(humidity)) 
    client.publish("Humid_4", String(DataFromMega.humidity, 2).c_str());
    client.publish("Status_4", DataFromMega.air_condi_status ? "true":"false");
    client.publish("Lux_4", String(DataFromMega.lux,2).c_str());
    client.publish("Gas_4", String(DataFromMega.ppm,2).c_str());
    client.publish("PIR_4", DataFromMega.pir_status ? "1":"0");
    client.publish("LDR_4", String(DataFromMega.distance,2).c_str());
    if (DataFromMega.timeup == true) client.publish("tol_4","true");
  }
  Serial.print(DataFromMega.temperature);
  Serial.print(' ');
  Serial.print(DataFromMega.humidity);
  Serial.print(' ');
  Serial.println(DataFromEsp.OnTimer);  
  // Serial.print(' ');
  delay(1000);
}
