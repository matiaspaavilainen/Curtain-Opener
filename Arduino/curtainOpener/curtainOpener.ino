#include "BluetoothSerial.h"
#include <ESP32Time.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

ESP32Time rtc(0);

int Pin0 = 2;
int Pin1 = 0;
int Pin2 = 4;
int Pin3 = 16;
int _step = 0;

void setup() {
  pinMode(Pin0, OUTPUT);
  pinMode(Pin1, OUTPUT);
  pinMode(Pin2, OUTPUT);
  pinMode(Pin3, OUTPUT);

  Serial.begin(115200);
  SerialBT.begin("ESP32");

  while (!Serial) {
    delay(100);
  }

  SerialBT.println("Ready");

  rtc.setTime(1731259680);

  // Initial setup
  // Get current time from phone
  //rtc.setTime(SerialBT.readStringUntil('\n').toInt());
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

void loop() {
  if (SerialBT.available() > 0) {
    String command = SerialBT.readStringUntil('\n');

    int start = 0;
    int split = command.indexOf(',') + 1;

    int rotations = command.substring(start, split).toInt();
    int direction = command.substring(split, command.length()).toInt();

    SerialBT.println(rtc.getDateTime());

    rotate(rotations, direction);
  }
}