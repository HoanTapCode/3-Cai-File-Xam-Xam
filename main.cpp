//############ DEFINE ############
// ##### SENSOR #####
// PIR MOTION SENSOR
#define PIR_PIN 23

// TEMPERATURE & HUMIDITY SENSOR
#define DHT_PIN 25 

// ULTRASONIC
#define TRIG_PIN 27
#define ECHO_PIN 29

// MQ2 GAS SENSOR 
#define GAS_PIN A1

// LDR PHOTORESISTOR
#define LDR_PIN A0


// ##### ACTUATOR #####
// RELAY
#define RELAY_PIN 13

// LED RGB
#define R_PIN 10
#define G_PIN 9
#define B_PIN 8

// LED
#define LED_PIN 12

// BUZZER
#define BUZZER_PIN 11

// OLED
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0X3c



//############ LIBRARY ############
#include <math.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "RTClib.h"
#include "DHT.h"



//############ VARIABLES & CONSTANTS ############
float temperature, humidity, GAMMA = 0.7, RL10 = 50;
//############
Adafruit_SSD1306 oled_display(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET);
RTC_DS1307 rtc;
DHT dht(DHT_PIN,DHT22);



//############ SETUP ############
void setup() {
  Serial.begin(9600);
  // ##### SENSOR #####
  // // TIMER RTC
  // if (! rtc.begin())
  // {
  //   Serial.println("Ko co sensor");
  //   while (1);
  //   rtc.adjust(DateTime(2025,5,11,19,38,20));    
  // }

  // PIR MOTION SENSOR
  pinMode(PIR_PIN, INPUT);

  // TEMPERATURE & HUMIDITY SENSOR
  dht.begin();

  // ULTRASONIC
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);


  // ##### ACTUATOR #####
  // LED & LED RGB
  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);

  // RELAY
  pinMode(RELAY_PIN, OUTPUT);

  // BUZZER
  pinMode(BUZZER_PIN, OUTPUT);

  // OLED
  oled_display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  oled_display.clearDisplay();
  oled_display.setTextSize(3);
  oled_display.setTextColor(SSD1306_WHITE);
}



void loop() {
  // ##### SENSOR #####
  // // TIMER RTC
  // DateTime now = rtc.now();
  // Serial.print(now.hour());
  // Serial.print(':');
  // Serial.print(now.minute());
  // Serial.print(':');
  // Serial.println(now.second());

  // // PIR MOTION SENSOR
  // Serial.println(digitalRead(PIR_PIN));

  // // TEMPERATURE & HUMIDITY SENSOR
  // temperature = dht.readTemperature();
  // humidity = dht.readHumidity();
  // Serial.print(temperature);
  // Serial.print(' ');
  // Serial.println(humidity);  

  // //ULTRASONIC
  // digitalWrite(TRIG_PIN, HIGH);
  // delayMicroseconds(10);
  // digitalWrite(TRIG_PIN, LOW);
  // int duration = pulseIn(ECHO_PIN, HIGH);
  // Serial.print("Distance in CM: ");
  // Serial.println(int(duration / 58.2));

  // // GAS SENSOR
  // Serial.println(analogRead(GAS_PIN));

  // // LDR PHOTORESISTOR
  // int analogValue = analogRead(LDR_PIN);
  // float voltage = analogValue / 1024. * 5;
  // float resistance = 2000 * voltage / (1 - voltage / 5);
  // float lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));
  // lux = roundf(lux*10.0f)/10.0f;
  // if (lux<1.0f)
  // {
  //   Serial.println(lux);
  // }
  // else
  // {
  //   int32_t lux_int = int32_t(lux);
  //   Serial.println(lux_int);
  // }

  // ##### ACTUATOR #####
  // // LED & LED RGB
  // digitalWrite(R_PIN, HIGH);
  // digitalWrite(LED_PIN, HIGH);
  // delay(1000);
  // digitalWrite(R_PIN, LOW);
  // digitalWrite(LED_PIN, LOW);

  // // RELAY
  // digitalWrite(RELAY_PIN,HIGH);
  // delay(1000);
  // digitalWrite(RELAY_PIN,LOW);

  // // BUZZER
  // for (int i = 100; i <= 500; ++i)
  // {
  //   // tone(BUZZER_PIN, i);
  //   // delay(150);
  //   tone(BUZZER_PIN, i, 100);
  // }
  // noTone(BUZZER_PIN);

  // //OLED
  // oled_display.clearDisplay();
  // oled_display.setCursor(0,0);
  // oled_display.println("TEST");
  // oled_display.display();

  // // ULTRASONIC
  // digitalWrite(TRIG_PIN, HIGH);
  // delayMicroseconds(10);
  // digitalWrite(TRIG_PIN, LOW);
  // int duration = pulseIn(ECHO_PIN, HIGH);
  // duration = int (duration/2/29.1);
  // Serial.print("Distance in CM: ");
  // Serial.println(duration);




  delay(1000);
}
