# Automatic curtain opener

During summer it is difficult to wake up in the morning, as the room is dark because of blackout curtains. In the winter, blackout curtains are not needed, because it's dark anyways. But during summer, it is hard to sleep when the sun is shining at 22:32. With this in mind, I wanted to be able to open the curtain at a specific time in the morning, to make waking up easier by taking advantage of the early sunlight.

This project consists of an ESP32 controlling a 28BYJ-48 stepper motor and a Jetpack Compose Android app. I'm also using DHT-22 temperature and humidity sensor for a simple indoor weather station. The app will be used to control the ESP32, which hosts a simple HTTP server to accept commands. Features include opening and closing the curtain manually and setting a time at which the curtain should open automatically. I'm using [ESP32Time](https://github.com/fbiego/ESP32Time) library to keep track of time on the board. The ESP32, along with the necessary components to drive the stepper motor will be mounted to a mounting board on my wall. The curtain is going to be moved using a string, which is attached to one of the curtain clips. The curtain can only be closed manually, when the string has loosened after the curtain is closed. There is a small weight hanging on the string to keep it tought. I can't really build automatic closing, as that would require some sort of pulley system at the ending point of the curtain rail.

## Current status
- Arduino part is done and in testing
- Opening takes about 2 minutes and 20 seconds
- Currently using Postman to send commands
- Need to get started on the app

## Techonologies used

[![Arduino](https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=arduino&logoColor=white)](https://www.arduino.cc/)
[![Android](https://img.shields.io/badge/Android-3DDC84?style=for-the-badge&logo=android&logoColor=white)](https://developer.android.com/)
[![Jetpack Compose](https://img.shields.io/badge/Jetpack%20Compose-4285F4?style=for-the-badge&logo=android&logoColor=white)](https://developer.android.com/jetpack/compose)
[![Kotlin](https://img.shields.io/badge/Kotlin-0095D5?style=for-the-badge&logo=kotlin&logoColor=white)](https://kotlinlang.org/)
