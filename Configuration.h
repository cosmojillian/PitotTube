/*
 * Configuration.h
 *
 *  Created on: Aug 26, 2016
 *      Author: cosmo
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#define VERSION "1.1.0"
#define BAUDRATE 115200
#define SERIALTIMEOUT 20

//Serial setup
#ifndef RUNNINGAVG
#define RUNNINGAVG 15
#endif

#ifndef SERIALUPDATEINTERVAL
#define SERIALUPDATEINTERVAL 250
#endif

#ifndef MAXSERIALDIGITS
#define MAXSERIALDIGITS 3
#endif


#define SENDTEMP 1
#define SENDPRES 1
#define SENDPOS 1

//Absolute pressure gauge pin def
#define TRANSDUCERPIN 0

//Differential pressure pin defs
#define DIFFERENTIALPIN1 1
#define DIFFERENTIALPIN2 2

//Actuator pin defs
#define MOTOR1DIR1 4
#define MOTOR1DIR2 5
#define MOTOR1SPEED 6
#define ACTUATORPOT1 4 //Analog pin
#define MOTOR2DIR1 1
#define MOTOR2DIR2 2
#define MOTOR2SPEED 3
#define ACTUATORPOT2 3 //Analog pin
#define ACTUATORRUNOUT 0.2
#define TOLERANCE 0.015

 //What pins on the teensy are the amp board pins connected to DO, CS, and CLK are labled on the amp board
#define TDO 11
#define TCS 8
#define TCLK 7

#endif /* CONFIGURATION_H_ */
