# Automatic curtain opener

During summer it is difficult to wake up in the morning, as the room is dark because of blackout curtains. However, it is also hard to sleep when the sun is shining at 22:32. With this in mind, I wanted to be able to open the curtain at a specific time in the morning, to make waking up easier by taking advantage of the early sunlight. I have a sunrise alarm clock, but it doesn't work as well as the real thing.

This project consist of an ESP32 controlling a 28BYJ-48 stepper motor and a Jetpack Compose Android app. The app will be used to control the ESP32, which hosts a simple HTTP server to accept commands. Features include opening and closing the curtain manually and setting a time at which the curtain should open or close. I'm using [ESP32Time](https://github.com/fbiego/ESP32Time) library to keep track of time on the board. The ESP32, along with the necessary components to drive the stepper motor will be mounted to the window side face of the pelmet in a nice box. The curtain is going to be moved using a string, which is attached to one of the curtain clips.

## Current status
This project is currently on hold, as I'm quite busy with other things. I would like to get this working before summer though. Currently I have the Arduino code to drive the stepper motor, but I need to design, implement and test the mechanics of the project.

## Techonologies used

[![Arduino](https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=arduino&logoColor=white)](https://www.arduino.cc/)
[![Android](https://img.shields.io/badge/Android-3DDC84?style=for-the-badge&logo=android&logoColor=white)](https://developer.android.com/)
[![Jetpack Compose](https://img.shields.io/badge/Jetpack%20Compose-4285F4?style=for-the-badge&logo=android&logoColor=white)](https://developer.android.com/jetpack/compose)
[![Kotlin](https://img.shields.io/badge/Kotlin-0095D5?style=for-the-badge&logo=kotlin&logoColor=white)](https://kotlinlang.org/)
