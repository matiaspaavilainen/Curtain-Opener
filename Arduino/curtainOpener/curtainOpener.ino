#include "secrets.h"
#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Time.h>
#include <DHT.h>
#include <ArduinoJson.h>

#define DHTPIN 13
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

ESP32Time rtc(0);

WebServer server(302);

IPAddress ip(192, 168, 1, 77);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

float hum;
float temp;

int Pin0 = 2;
int Pin1 = 0;
int Pin2 = 4;
int Pin3 = 16;
int _step = 0;

String opening_times[4];
String closing_times[4];

bool opened[4] = {false};
bool closed[4] = {false}; 

int current_index = 0;

void get_root() {
  String message = "Curtain opener ready!";
  server.send(200, "text/plain", message);
};

void get_arduino_weather() {
  JsonDocument doc;
  doc["temperature"] = dht.readTemperature();
  doc["humidity"] = dht.readHumidity();
  doc["time"] = rtc.getDateTime();

  String jsonres;
  serializeJson(doc, jsonres);
  server.send(200, "application/json", jsonres);
}

void get_arduino_time() {
  if (rtc.getEpoch() < 17400000) {
    server.send(302, "plain/text", "Time has not been set!");
    return;
  };
  server.send(200, "text/plain", rtc.getDateTime());
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

void get_op_clo_times() {
  JsonDocument doc;
  JsonArray opening = doc["opening_times"].to<JsonArray>();
  JsonArray closing = doc["closing_times"].to<JsonArray>();
  for (byte i = 0; i < 4; i++) {
    opening.add(opening_times[i]);
    closing.add(closing_times[i]);
  };

  String jsonres;
  serializeJson(doc, jsonres);

  server.send(200, "application/json", jsonres);
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

    opening_times[current_index] = doc["opening_time"].as<String>();
    closing_times[current_index] = doc["closing_time"].as<String>();

    current_index++;

    server.send(200, "text/plain", String(current_index));  // Correct content type
  } else {
    server.send(400, "application/json", "{\"error\":\"No JSON received\"}");
  }
}

void del_op_clo_time() {
  int index_to_del = server.arg("plain").toInt();
  opening_times[index_to_del] = "";
  closing_times[index_to_del] = "";
  current_index--;
  server.send(204);
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
  // Get current time as a formatted string "hh:mm"
  String current_time = rtc.getTime("%H:%M");

  for (int i = 0; i < 4; i++) {
    if (opening_times[i] == current_time && !opened[i]) {
      Serial.println("Opening action triggered");
      opened[i] = true;  // Mark as triggered
      rotate(2, false);
    } else {
      opened[i] = false;
    }
    
    if (closing_times[i] == current_time && !closed[i]) {
      Serial.println("Closing action triggered");
      closed[i] = true;  // Mark as triggered
      rotate(2, true);
    } else {
      closed[i] = false;
    }
  }
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

  // GET
  server.on("/", get_root);
  server.on("/weather", get_arduino_weather);
  server.on("/time", get_arduino_time);
  server.on("/open_close_times", get_op_clo_times);

  // SET
  server.on("/set/time", HTTP_POST, set_arduino_time);
  server.on("/set/open_close_time", HTTP_POST, set_op_clo_time);
  server.on("/del/op_clo", HTTP_DELETE, del_op_clo_time);

  server.begin();
  dht.begin();
}

void loop() {
  server.handleClient();
  open_and_close();
}