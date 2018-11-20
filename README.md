## Purpose 
This code is apart of a mass flow rate lab. It is meant to run on a Teensy 3.2 with a modified Adafruit MAX31855 (for use with T type thermocouples), 2 Omega differential pressure sensors, 1 Omega absolute pressure sensor, and 2 Firgelli linear actuators with 10k feedback potentiometers. The program takes commands over serial (delivered by a Labview program) and moves the pitot tubes to the desired position. It reports temperature, pressure, and position data over serial to the same program to calculate airspeed velocity and mass flow rate.

**Please note**, this project is depreciated code. Updated code is available from the MassAirFlow repository.

## Structure 
This program is divided into one main class (DataCollector) that is built on by the other classes (Actuator, Differential, Transducer, and Thermocouple). They all have methods like Update (should be called every loop()), AvgData (returns the average of the running average), ReadData (reads an analog pin or data via SPI and returns the data), AddData (adds new data to the running average (and shifts all data)), and DisableSerial (disables reporting data over serial for debugging purposes). Each object may build on DataCollector, or in the case of Thermocouple, may override the base methods since it gathers data from a different source (a better solution I may switch this to is changing DataCollector to be an interface and creating a new object for analog devices to inherit from).

## Dependencies 
This code depends on the SPI library and the Adafruit MAX31855 library. If you are using Teensy, make sure you have installed Teensyduino+the included libraries. If you are not using Teensy, make sure you have the ElapsedMillis library installed as well. DataCollector and several of its children rely on it.
