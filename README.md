# Automatic curtain opener

## Why I made this
During summer it is difficult to wake up in the morning, as the room is dark because of blackout curtains. It's hard to sleep when the sun is shining at 22:32, so blackout curtains are needed. With this in mind, I wanted to be able to open the curtain at a specific time in the morning, to make waking up easier by taking advantage of the early sunlight.

## Components
This project consists of an ESP32 controlling a 28BYJ-48 stepper motor and a Jetpack Compose Android app. I'm also using DHT-22 temperature and humidity sensor for a simple indoor weather station. The app will be used to control the ESP32, which hosts a simple HTTP server on my Wi-Fi network to accept commands. It will only be accesible from that network. Features include opening and closing the curtain manually and setting a time at which the curtain should open automatically. The manual opening and closing is only necessary in fault situations, or testing. I'm using [ESP32Time](https://github.com/fbiego/ESP32Time) library to keep track of time on the board. The ESP32, along with the necessary components to drive the stepper motor will be attached to a mounting board on a wall. The curtain is going to be moved using a string, which is attached to one of the curtain clips. The curtain needs to be closed manually, when the string has loosened after the curtain is opened. When the curtain opens, it immediately loosens the string after reeling it in. There is a small weight hanging on the string to keep it tought. I can't really build automatic closing, as that would require some sort of pulley system at the ending point of the curtain rail.

## Pictures

<figure>
    <img src="https://github.com/user-attachments/assets/c2759542-a949-4fa3-a3d4-4571ba6a7316"
         alt="Contents of the electronics box, missing temperature sensor."
    >
    <figcaption>All the components inside the box, apart from the temperature sensor.</figcaption>
</figure>

<br>
<br>


<figure>
    <img src="https://github.com/user-attachments/assets/997d62b1-b411-4a92-af57-61ae990d37c8"
         alt="Painted electronics box, mounted to the wall"
    >
    <figcaption>The final version of the electronics box, mounted on the wall. The temperature sensor is on top of the box, for more accurate readings. Painted white to make it less obvious. I should have put the motor to the other side of the box, then it would have been less visible. Power is supplied with a random wall power adapter and a long USB cable.</figcaption>
</figure>


## Current status
- Arduino part is done and in use
- Currently using Postman and a HTTP andoid app to send commands
- Missing the endpoint to toggle "alarm" on and off
- Need to get started on the app
- Add weather data collection and graphing to my website?

## Techonologies used

[![Arduino](https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=arduino&logoColor=white)](https://www.arduino.cc/)
[![Android](https://img.shields.io/badge/Android-3DDC84?style=for-the-badge&logo=android&logoColor=white)](https://developer.android.com/)
[![Jetpack Compose](https://img.shields.io/badge/Jetpack%20Compose-4285F4?style=for-the-badge&logo=android&logoColor=white)](https://developer.android.com/jetpack/compose)
[![Kotlin](https://img.shields.io/badge/Kotlin-0095D5?style=for-the-badge&logo=kotlin&logoColor=white)](https://kotlinlang.org/)
