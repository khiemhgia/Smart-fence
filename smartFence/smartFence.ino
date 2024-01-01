/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL6sNT5wIM_"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "UTIOm0OgUo4qE0h2UzI8Umd6vSanweG7"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiManager.h>
#include <Wire.h>

#define LPWM D1
#define RPWM D2
#define btn1 D6
#define btn2 D5
#define trig D0
#define echo D7
unsigned long lastUpdate = 0;
String State = "";
int state;
int closeValue = 0, openValue = 0, onoffValue = 0;
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "AmericanStudy T1";
char pass[] = "66668888";

BlynkTimer timer;
WiFiManager wifiManager;

bool check = true;

int btn1State = LOW;
int btn2State = LOW;

int lastBtn1State = LOW;
int lastBtn2State = LOW;

unsigned long btn1PressTime = 0;
unsigned long btn2PressTime = 0;

const unsigned long longPressDuration = 1000;

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_WRITE(V1) {
  openValue = param.asInt();
  if (openValue == HIGH) {
    mo1();
  }
}
BLYNK_WRITE(V2) {
  closeValue = param.asInt();

  if (closeValue == HIGH) {
    dong1();
  }
}
BLYNK_WRITE(V5) {
  onoffValue = param.asInt();
}
void myTimerEvent() {
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V3, millis() / 1000);
}

void setup() {
  // Debug console
  Serial.begin(115200);
  pinMode(LPWM, OUTPUT);
  pinMode(RPWM, OUTPUT);
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  digitalWrite(trig, LOW);

  wifiManager.autoConnect("Hang rao", "88888888");
  Serial.println();
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  timer.setInterval(1000L, myTimerEvent);
}

void loop() {
  Blynk.run();
  timer.run();
  btn1State = digitalRead(btn1);  // nhan tren 3s ==> doi mode // nhan duoi 3s doi che do xem man hinh
  btn2State = digitalRead(btn2);

  Serial.println(readDistance());

  if (btn1State != lastBtn1State) {
    if (btn1State == LOW) {
      btn1PressTime = millis();
    } else {
      if (millis() - btn1PressTime < longPressDuration) {
        // Xử lý btn 1 nhấn ngắn (< 3 giây)

        dung();


      } else {
        // Xử lý btn 1 nhấn lâu (>= 3 giây)


        mo1();
      }
    }
  }

  if (btn2State != lastBtn2State) {
    if (btn2State == LOW) {
      btn2PressTime = millis();
    } else {
      if (millis() - btn2PressTime < longPressDuration) {
        // Xử lý btn 1 nhấn ngắn (< 3 giây)

        dung();

      } else {
        // Xử lý btn 1 nhấn lâu (>= 3 giây)


        dong1();
      }
    }
  }
  lastBtn2State = btn2State;
  lastBtn1State = btn1State;

  if (readDistance() < 40 && onoffValue == 1 && check == true) {
   dong1();
      check = false;
  } else {
    check = true;
  }
}


void mo() {
  digitalWrite(LPWM, HIGH);
  digitalWrite(RPWM, LOW);
  Serial.println("Đang mở");
  Blynk.virtualWrite(V4, "STATE: OPENING...");
  state = 1;
}

void dong() {
  digitalWrite(LPWM, LOW);
  digitalWrite(RPWM, HIGH);

  Serial.println("Đang đóng");
  Blynk.virtualWrite(V4, "STATE: CLOSING...");
  state = 2;
}
void dung() {
  digitalWrite(LPWM, LOW);
  digitalWrite(RPWM, LOW);
  if (state == 1) {
    Serial.println("Cua da mo");
    Blynk.virtualWrite(V4, "STATE: OPENED");
  } else if (state == 2) {
    Serial.println("Cua da dong");
    Blynk.virtualWrite(V4, "STATE: CLOSED");
  }
}

int readDistance() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  unsigned long duration = pulseIn(echo, HIGH);
  int distance = duration / 29.412 / 2;
  return distance;
}

void dong1() {
  dong();
  delay(7000);
  dung();
}
void mo1() {
  mo();
  delay(7000);
  dung();
}