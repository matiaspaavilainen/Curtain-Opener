# Automatic curtain opener

## Why I made this
During summer it is difficult to wake up in the morning, as the room is dark because of blackout curtains. It's hard to sleep when the sun is shining at 23:07, so blackout curtains are needed. With this in mind, I wanted to be able to open the curtain at a specific time in the morning, to make waking up easier by taking advantage of the early sunlight.

## Description
This project consists of an ESP32 controlling a 28BYJ-48 stepper motor and a Jetpack Compose Android app. I'm also using the DHT-22 temperature and humidity sensor for a simple indoor weather station. The app will be used to control the ESP32, which hosts a simple HTTP server on my Wi-Fi network to accept commands. Features include opening and "closing" the curtain manually and setting times at which the curtain should open automatically per day. The curtain needs to be closed manually, when the string has loosened after the curtain is opened. The motor can also be driven manually, specifying the number of rotations and direction. I'm using [ESP32Time](https://github.com/fbiego/ESP32Time) library to keep track of time on the board. The ESP32, along with the necessary components to drive the stepper motor is attached to my wall. The curtain is going to be moved using a string, which is attached to one of the curtain clips. There is a small weight hanging on the string to keep it tought, to stop it from getting tangled in the reel. I can't really build automatic closing, as that would require some sort of pulley system at the ending point of the curtain rail.

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
    <figcaption>The final version of the electronics box, mounted on the wall. The temperature sensor is on top of the box. Painted white to make it less obvious. I should have put the motor to the other side of the box, beacuse then it would have been hidden by the curtain almost completely. Power is supplied with a 5V wall power adapter and a USB cables to power the motor, and Arduino.
    </figcaption>
</figure>


## Current status
- Arduino part is done and in use
- Core functionality on the app is complete, set opening times, view status
- Basic UI is functional
  
## TODO
- Add manual opening functinonality, not really needed though
- Make UI prettier
- Other "decorations" (app icon, splash screen, ...)

## Techonologies used

[![Arduino](https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=arduino&logoColor=white)](https://www.arduino.cc/)
[![Android](https://img.shields.io/badge/Android-3DDC84?style=for-the-badge&logo=android&logoColor=white)](https://developer.android.com/)
[![Jetpack Compose](https://img.shields.io/badge/Jetpack%20Compose-4285F4?style=for-the-badge&logo=android&logoColor=white)](https://developer.android.com/jetpack/compose)
[![Kotlin](https://img.shields.io/badge/Kotlin-0095D5?style=for-the-badge&logo=kotlin&logoColor=white)](https://kotlinlang.org/)
