#include "secrets.h"
#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Time.h>
#include <DHT.h>
#include <ArduinoJson.h>

#define DHTPIN 13
#define DHTTYPE DHT22
// one time for each day of the week
#define ARRAY_LEN 7

DHT dht(DHTPIN, DHTTYPE);

ESP32Time rtc(3600*2);

WebServer server(302);

IPAddress ip(192, 168, 1, 77);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

float hum;
float temp;

int Pin0 = 16;
int Pin1 = 4;
int Pin2 = 0;
int Pin3 = 2;
int _step = 0;

String opening_times[ARRAY_LEN];

int TOTAL_ROTATIONS = 32;

// If false, will not automatically open
bool alarm_on = true;

void rotate(int rotations, bool dir) {
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

// Includes weather, time, alarmStatus and curtainStatus
void get_arduino_status() {
  JsonDocument doc;

  doc["alarmStatus"] = alarm_on;
  doc["time"] = rtc.getDateTime();

  doc["temperature"] = dht.readTemperature() - 4;
  doc["humidity"] = dht.readHumidity();

  JsonArray opening = doc["opening_times"].to<JsonArray>();
  for (int i = 0; i < ARRAY_LEN; i++) {
    opening.add(opening_times[i]);
  };

  String jsonres;
  serializeJson(doc, jsonres);
  server.send(200, "application/json", jsonres);
}

void set_arduino_time() {
  if (server.hasArg("plain")) {
    String body = server.arg("plain");
    Serial.println("Received time: ");
    int time = body.toInt();
    Serial.println(time);

    rtc.setTime(time);

    server.send(201, "text/plain", "Time set");
  };
}

void set_open_time() {
  if (server.hasArg("plain")) {
    String input = server.arg("plain");

    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, input);

    if (error) {
      Serial.println("Failed to parse JSON");
      server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
      return;
    }

    JsonArray opening = doc["opening_times"].as<JsonArray>();

    // set all each time
    for (int i = 0; i < ARRAY_LEN; i++) {
      opening_times[i] = opening[i].as<String>();
    }
    server.send(200, "text/plain", "Times set!");
  } else {
    server.send(400, "application/json", "{\"error\":\"No JSON received\"}");
  }
}

void open_and_close() {
  // Only open automatically if alarm is set on
  if (!alarm_on) {
    return;
  }
  // Get current time as a formatted string "hh:mm"
  String current_time = rtc.getTime("%H:%M");
  int current_day = rtc.getDayofWeek();
  for (int i = 0; i < ARRAY_LEN; i++) {
    // Check if it's the correct day and time
    if (i == current_day && opening_times[i] == current_time && closed) {
      Serial.println("Opening action triggered");
      rotate(TOTAL_ROTATIONS, true);
      // close immediately afterwards
      delay(1000);
      rotate(TOTAL_ROTATIONS, false);
      return;
    }
  }
}

void toggle_alarm() {
  alarm_on = !alarm_on
  server.send(200, "plain/text", String(alarm_on))
}

void move_manually() {
  if (server.hasArg("plain")) {
    String input = server.arg("plain");
    if (input.startsWith("-")) {
      // negative close
      rotate(abs(input.toInt()), false);
      server.send(200, "plain/text", "-1");
    } else {
      // positive open
      rotate(input.toInt(), true);
      server.send(200, "plain/text", "1");
    }
  };
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

  server.on("/status", HTTP_GET, get_arduino_status);
  server.on("/alarm", HTTP_GET, toggle_alarm);

  server.on("/set/time", HTTP_POST, set_arduino_time);
  server.on("/set/open_close", HTTP_POST, set_open_time);
  server.on("/move", HTTP_POST, move_manually);

  server.begin();
  dht.begin();
}

void loop() {
  server.handleClient();
  open_and_close();
}
