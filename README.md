# Group-19-fall-detection-system
Fall Detection System with Blynk Integration
This project is a fall detection system designed for elderly care and patient monitoring. It uses an MPU6050 accelerometer & gyroscope sensor connected to an ESP32 to detect falls based on motion and impact analysis. When a fall is detected, the system:
1.	Activates a buzzer and LED alert
2.	 Sends a notification via Blynk IoT
3.	Differentiates between a genuine fall and a false alarm
Features:
•	Free-fall detection using acceleration & gyroscope data
•	Impact detection to confirm hard falls
•	Stillness check to verify if the person remains immobile
•	Blynk IoT integration for remote monitoring & alerts
•	ESP32-based Wi-Fi connectivity
 Components Used:
•	ESP32 (microcontroller)
•	MPU6050 (accelerometer & gyroscope)
•	Buzzer & LED (for local alerts)
•	Blynk IoT (for remote notifications)
How It Works:
1.	The MPU6050 continuously monitors acceleration and gyroscope data.
2.	If a sudden drop in acceleration (free fall) is detected, the system starts checking for impact.
3.	If impact is detected, the system checks for stillness to confirm a real fall.
4.	If confirmed, it triggers alerts (buzzer, LED, and Blynk notification).
 Repository Contents:
 Code: Arduino sketch for fall detection logic
 Presentation: Project overview (PowerPoint)
 Simulation: Wokwi-based testing setup

video presentation in link; https://youtu.be/cBLS8h-MIAk

and power point presentation; 
https://github.com/tesla580/Group-19-Fall-detection-system./blob/527a4089defc166e6cec75e6fdeae51c2c6ba454/Fall%20Detection%20System%20for%20Elderly%20-%20Detects%20sudden%20falls%20and%20sends%20alerts%20-%20Group%2019.pptx
