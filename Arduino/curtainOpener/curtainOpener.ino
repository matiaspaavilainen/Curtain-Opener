#include "secrets.h"
#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Time.h>

ESP32Time rtc(0);

WebServer server(80);

IPAddress ip(192, 168, 1, 77);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

int Pin0 = 2;
int Pin1 = 0;
int Pin2 = 4;
int Pin3 = 16;
int _step = 0;

void handleRoot() {
  String message = "Curtain opener ready!";
  server.send(200, "text/plain", message);
}

void rotate(int rotations, bool dir = true) {
  long stepsToTake = rotations * 4096;
  int steps = 0;
  while (steps != stepsToTake) {
    // http://eeshop.unl.edu/pdf/Stepper+Driver.pdf
    switch(_step) {
      case 0:
        digitalWrite(Pin0, LOW);
        digitalWrite(Pin1, LOW);
        digitalWrite(Pin2, LOW);
        digitalWrite(Pin3, HIGH);
      break;
      case 1:
        digitalWrite(Pin0, LOW);
        digitalWrite(Pin1, LOW);
        digitalWrite(Pin2, HIGH);
        digitalWrite(Pin3, HIGH);
      break;
      case 2:
        digitalWrite(Pin0, LOW);
        digitalWrite(Pin1, LOW);
        digitalWrite(Pin2, HIGH);
        digitalWrite(Pin3, LOW);
      break;
      case 3:
        digitalWrite(Pin0, LOW);
        digitalWrite(Pin1, HIGH);
        digitalWrite(Pin2, HIGH);
        digitalWrite(Pin3, LOW);
      break;
      case 4:
        digitalWrite(Pin0, LOW);
        digitalWrite(Pin1, HIGH);
        digitalWrite(Pin2, LOW);
        digitalWrite(Pin3, LOW);
      break;
      case 5:
        digitalWrite(Pin0, HIGH);
        digitalWrite(Pin1, HIGH);
        digitalWrite(Pin2, LOW);
        digitalWrite(Pin3, LOW);
      break;
      case 6:
        digitalWrite(Pin0, HIGH);
        digitalWrite(Pin1, LOW);
        digitalWrite(Pin2, LOW);
        digitalWrite(Pin3, LOW);
      break;
      case 7:
        digitalWrite(Pin0, HIGH);
        digitalWrite(Pin1, LOW);
        digitalWrite(Pin2, LOW);
        digitalWrite(Pin3, HIGH);
      break;
      default:
        digitalWrite(Pin0, LOW);
        digitalWrite(Pin1, LOW);
        digitalWrite(Pin2, LOW);
        digitalWrite(Pin3, LOW);
      break;
    }
    if(dir){
      _step++;
    }else{
      _step--;
    }
    if(_step>7){
      _step=0;
    }
    if(_step<0){
      _step=7;
    }
    steps++;
    delay(1);
  }

  steps = 0;
  digitalWrite(Pin0, LOW);
  digitalWrite(Pin1, LOW);
  digitalWrite(Pin2, LOW);
  digitalWrite(Pin3, LOW);
}

void setup() {
  pinMode(Pin0, OUTPUT);
  pinMode(Pin1, OUTPUT);
  pinMode(Pin2, OUTPUT);
  pinMode(Pin3, OUTPUT);

  Serial.begin(115200);

  WiFi.config(ip, gateway, subnet);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    WiFi.begin(SECRET_SSID, SECRET_PASSWORD);
    delay(5000);
  }

  Serial.print("Connected: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);

  server.begin();

  // Initial setup
  // Get current time from phone
  //rtc.setTime(SerialBT.readStringUntil('\n').toInt());
}

void loop() {
  server.handleClient();
}