# Automatic curtain opener

During summer it is difficult to wake up in the morning, as the room is dark because of blackout curtains. However, it is also hard to sleep when the sun is shining at 22:32. With this in mind, I wanted to be able to open the curtain at a specific time in the morning, to make waking up easier by taking advantage of the early sunlight. I have a sunrise alarm clock, but it doesn't work as well as the real thing.

## Techonologies used

This project consist of an ESP32 controlling a  28BYJ-48 stepper motor and a Jetpack Compose Android app. The app is used to control the ESP32 over serial bluetooth. Features include opening and closing the curtain, setting a time at which the curtain should open, and closing the curtain when the temperature gets over a certain threshold during the day. Alternatively specific times at which the curtain should be open or close coudl be defined. I'm using ESP32Time library to keep track of time. The ESP32, along with the necessary components to drive the stepper motor are mounted to the window side face of the pelmet. The curtain is moved using a string, that is attached to one of the curtain clips.
