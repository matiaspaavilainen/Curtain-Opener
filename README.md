# Automatic curtain opener

During summer it is difficult to wake up in the morning, as the room is dark because of blackout curtains. In the winter, blackout curtains are not needed, because it's dark anyways. But during summer, it is hard to sleep when the sun is shining at 22:32. With this in mind, I wanted to be able to open the curtain at a specific time in the morning, to make waking up easier by taking advantage of the early sunlight.

This project consists of an ESP32 controlling a 28BYJ-48 stepper motor and a Jetpack Compose Android app. I'm also using DHT-22 temperature and humidity sensor to gather weather data for fun. The app will be used to control the ESP32, which hosts a simple HTTP server to accept commands. Features include opening and closing the curtain manually and setting a time at which the curtain should open or close. I'm using [ESP32Time](https://github.com/fbiego/ESP32Time) library to keep track of time on the board. The ESP32, along with the necessary components to drive the stepper motor will be mounted to a mounting board on my wall. The curtain is going to be moved using a string, which is attached to one of the curtain clips. The main challenge is closing the curtain again. It's relatively simple to open it, as I can pull the curtain directly towards the wall that the box will be mounted on. But closing it requires moving it to the opposite direction, away from any mounting points. It will most likely only be able to open automatically, closing needs to be manual.

## Techonologies used

[![Arduino](https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=arduino&logoColor=white)](https://www.arduino.cc/)
[![Android](https://img.shields.io/badge/Android-3DDC84?style=for-the-badge&logo=android&logoColor=white)](https://developer.android.com/)
[![Jetpack Compose](https://img.shields.io/badge/Jetpack%20Compose-4285F4?style=for-the-badge&logo=android&logoColor=white)](https://developer.android.com/jetpack/compose)
[![Kotlin](https://img.shields.io/badge/Kotlin-0095D5?style=for-the-badge&logo=kotlin&logoColor=white)](https://kotlinlang.org/)
