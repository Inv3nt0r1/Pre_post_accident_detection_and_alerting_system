# Pre_post_accidental_detection_and_alerting_system

This repository contains the code of the embedded module of  our project named Pre and post accident detection and alerting system.

The project has the following features:
1. Fetch Data from OBD port of car (protocol conversion is remaining)
2. Monitor OBD data, and data from other sensors and predict if the driver is doing rash driving or not. If yes, give him an alert.
3. If an accident happens, detect it and trigger an emergency alerting routine.
4 The emergency alerting routine sends necessary information like accident location, car, and owner information to the central server. 
5. That central server has an application that finds the nearest hospital and sends a request for deploying an ambulance to the accident location.
6. If an ambulance is not available or the hospital doesn't respond, then it finds the next nearest hospital.

The components used in this prototype are:
1. Arduino Uno
2. Raspberry Pi 3B+
2. Ublox 6M GPS
3. Sim900A SIM module
4. Accelerometer

The repository contains code of embedded module was desinged for car. It performs first four features. 
