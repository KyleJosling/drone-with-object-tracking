# drone-with-object-detection

## Overview

This repository contains the code to create a drone that can detect and follow coloured objects. It uses a Pi for object detection and an Arduino for a flight controller. The pi uses OpenCV to detect faces and sends the location of the face in the frame to the Arduino over the Multiwii Serial Protocol, which moves the drone to follow the object.

I soldered up a pcb with the Arduino and the other sensors. The drone can achieve a stable hover, and right now the yaw is controlled based on where the object is.

## Materials

- Raspberry Pi 3 Model B (OpenCV installed with extra modules)
- Quadcopter 450 Frame
- Arduino Nano
- MPU6050 Accelerometer/Gyroscope
- HK5883 Magnetometer
- BMP180 Barometer
- Power Distribution Board
- SimonK 30A ESCs
- EMAX 2213 935KV Motors
- 5000mah 3s Lithium-Polymer battery
- 1045 Props
- Raspberry Pi Camera v2
