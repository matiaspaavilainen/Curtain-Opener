#include "secrets.h"
#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Time.h>
#include <DHT.h>
#include <ArduinoJson.h>

#define DHTPIN 13
#define DHTTYPE DHT22
// only 2 opening and closing times, don't see the need for more
#define ARRAY_LEN 2

DHT dht(DHTPIN, DHTTYPE);

ESP32Time rtc(0);

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
String closing_times[ARRAY_LEN];

int TOTAL_ROTATIONS = 32;

// default position is closed, so curtain needs to be closed when arduino is powered on
// true == closed, false == open
bool closed = true;
// If false, will not automatically open
bool alarm_on = true;

// Includes weather, time, alarmStatus and curtainStatus
void get_arduino_status() {
  JsonDocument doc;

  doc["curtainStatus"] = closed;
  doc["alarmStatus"] = alarm_on;
  doc["time"] = rtc.getDateTime();

  doc["temperature"] = dht.readTemperature() - 4;
  doc["humidity"] = dht.readHumidity();

  JsonArray opening = doc["opening_times"].to<JsonArray>();
  JsonArray closing = doc["closing_times"].to<JsonArray>();
  for (int i = 0; i < ARRAY_LEN; i++) {
    opening.add(opening_times[i]);
    closing.add(closing_times[i]);
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

    server.send(201, "text/plain", "Time set to");
  };
}

void set_op_clo_time() {
  if (server.hasArg("plain")) {
    String input = server.arg("plain");

    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, input);

    if (error) {
      Serial.println("Failed to parse JSON");
      server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
      return;
    }
    
    // get the indicies to change
    JsonArray index = doc["index"].as<JsonArray>();
    JsonArray opening = doc["opening_times"].as<JsonArray>();
    JsonArray closing = doc["closing_times"].as<JsonArray>();

    // change only those indicies
    // extra checks on the app to keep amount of code here low
    for (int i = 0; i < index.size(); i++) {
      int idx = index[i].as<int>();
      opening_times[idx] = opening[i].as<String>();
      closing_times[idx] = closing[i].as<String>();
    }

    Serial.println("Times set successfully");
    server.send(200, "text/plain", "Times set!");
  } else {
    server.send(400, "application/json", "{\"error\":\"No JSON received\"}");
  }
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

void open_and_close() {
  // Only open automatically if alarm is set on
  if (!alarm_on) {
    return;
  }
  // Get current time as a formatted string "hh:mm"
  String current_time = rtc.getTime("%H:%M");

  for (int i = 0; i < 2; i++) {
    if (opening_times[i] == current_time && closed) {
      Serial.println("Opening action triggered");
      rotate(TOTAL_ROTATIONS, true);
      closed = false;
      return;
    }
    
    if (closing_times[i] == current_time && !closed) {
      Serial.println("Closing action triggered");
      rotate(TOTAL_ROTATIONS, false);
      closed = true;
      return;
    }
  }
}

// takes time, use to add progress spinner/bar and stop other functions
// time to make 1 rotation is constantish, total rotations is constant, ez progress bar
// might drift over time
void open_manually() {
  rotate(TOTAL_ROTATIONS, true);
  closed = false;
  server.send(200, "plain/text", "Opened");
}

void close_manually() {
  rotate(TOTAL_ROTATIONS, false);
  closed = true;
  server.send(200, "plain/text", "Closed");
}

void move_manually() {
  if (server.hasArg("plain")) {
    String input = server.arg("plain");
    if (input.startsWith("-")) {
      rotate(abs(input.toInt()), false);
      server.send(200, "plain/text", "-1");
    } else {
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
  server.on("/open", HTTP_GET, open_manually);
  server.on("/close", HTTP_GET, close_manually);

  server.on("/set/time", HTTP_POST, set_arduino_time);
  server.on("/set/open_close", HTTP_POST, set_op_clo_time);
  server.on("/move", HTTP_POST, move_manually);

  server.begin();
  dht.begin();
}

void loop() {
  server.handleClient();
  open_and_close();
}