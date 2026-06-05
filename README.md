# Smart Helmet for Accident Prevention and Detection

## Overview

Smart Helmet for Accident Prevention and Detection is an IoT-based rider safety system designed to reduce motorcycle accidents and improve emergency response. The system integrates helmet-wearing detection, alcohol monitoring, accident detection, ignition control, GPS tracking, and GSM-based emergency alerting.

The project uses two ESP32 microcontrollers communicating through ESP-NOW to provide low-latency wireless communication between the helmet unit and the motorcycle unit.

---

## Features

* Helmet Wearing Detection using IR Sensor
* Alcohol Detection using MQ-3 Sensor
* Accident Detection using MPU6050 Accelerometer and Gyroscope
* Real-Time Wireless Communication using ESP-NOW
* Vehicle Ignition Control through Relay
* GPS-Based Location Tracking
* GSM Emergency Alert System
* Buzzer-Based Warning System
* Low-Power IoT Architecture

---

## System Architecture

Helmet Unit (Transmitter)

* ESP32
* MPU6050
* MQ-3 Alcohol Sensor
* IR Sensor
* Buzzer

↓

ESP-NOW Communication

↓

Bike Unit (Receiver)

* ESP32
* GPS Module (NEO-10M)
* GSM Module (A7670C LTE)
* Relay Module

↓

Emergency SMS Alert + Ignition Control

---

## Hardware Components

| Component      | Quantity |
| -------------- | -------- |
| ESP32          | 2        |
| MPU6050        | 1        |
| MQ-3 Sensor    | 1        |
| IR Sensor      | 1        |
| GPS NEO-10M    | 1        |
| GSM A7670C LTE | 1        |
| Relay Module   | 1        |
| Buzzer         | 1        |
| Li-ion Battery | 1        |

---

## Project Structure

docs/

* Project Report
* Presentation
* Circuit Diagram
* Block Diagram

hardware/

* EasyEDA Design Files
* PCB Design Files

code/

* Transmitter Code
* Receiver Code

images/

* Prototype Photos
* Testing Images

---

## Working Principle

1. IR sensor checks whether the rider is wearing the helmet.
2. MQ-3 sensor detects alcohol concentration.
3. MPU6050 continuously monitors acceleration and orientation.
4. ESP32 processes sensor data and determines safety conditions.
5. Data is transmitted wirelessly using ESP-NOW.
6. Receiver ESP32 controls ignition using a relay.
7. During an accident, GPS coordinates are acquired.
8. GSM module sends emergency SMS alerts containing location information.

---

## Future Enhancements

* Mobile Application Integration
* Cloud-Based Monitoring Dashboard
* Fleet Management System
* Voice-Based Rider Alerts
* Health Monitoring Sensors
* AI-Based Accident Prediction

---

## Authors

Final Year Engineering Project

Smart Helmet for Accident Prevention and Detection

---

## License

This project is intended for educational and research purposes.
