#include "Configuration.h"
#include <SPI.h>
#include "Adafruit_MAX31855.h"
#include "Actuator.h"
#include "Differential.h"
#include "Transducer.h"

/*
 * todo
 * -Create Temperature class
 *
 * Future work
 * -Create a velocity dependent positioning control (no more stalling)
 */

/*Change log:
 * 1.0
 * -Added versioning
 * -Added a command for version requesting
 *
 * 1.0.1 - 1.0.3
 * -Added running averages for all sensor values
 * -Created a runout reset to combat rounding errors/noise
 * -
 *
 * 1.1.0
 * -Major code refactoring:
 * 	-Divided repeated code and configurations into objects
 * 	 -Objects created for Pressure Transducers, Differential Pressure Sensors, Linear Actuators, and the MAX83155 temp sensor
 * 	-Reworked actuator safety features to use the running average instead of last value
 * 	-Reworked the actuator position finding to compare the history average with the current value (to minimize skipping and low on)
 * 	-Created a configuration file
 * 	-Fixed the lengthy serial read issue
 */

//Actuators
Actuator actuator1(MOTOR1DIR1, MOTOR1DIR2, MOTOR1SPEED, ACTUATORPOT1, .5, TOLERANCE, '1');
Actuator actuator2(MOTOR2DIR1, MOTOR2DIR2, MOTOR2SPEED, ACTUATORPOT2, .5, TOLERANCE, '2');

//Thermocouple sensor
Adafruit_MAX31855 thermocouple(TCLK, TCS, TDO); //Setup the thermocouple

//Pressure sensors
Transducer transducer(TRANSDUCERPIN, 1);
Differential differential1(DIFFERENTIALPIN1, 1);
Differential differential2(DIFFERENTIALPIN2, 2);

// to be removed
//Timers used for sending data over serial/moving actuators
elapsedMillis lastSerialAdd;
elapsedMillis lastTempGrab;

//Values for the running temperature average to smooth out fluctuations
#define TEMPADDINTERVAL 75
double tempRange[RUNNINGAVG];
//end of removed

// the setup routine runs once when you press reset:
void setup() {
	analogReadResolution(12);
	Serial.begin(BAUDRATE); //Setup serial coms at 115200 baud
	Serial.setTimeout(SERIALTIMEOUT);
	delay(500);	//Wait for the thermocouple buffer to fill a little

	actuator1.SetDependent(&actuator2);
	actuator2.SetDependent(&actuator1);

	if (SENDPOS == 0) {
		actuator1.DisableSerial();
		actuator2.DisableSerial();
	}

	if (SENDPRES == 0) {
		transducer.DisableSerial();
		differential1.DisableSerial();
		differential2.DisableSerial();
	}

	//To be removed
	//Make sure all values in the running temp array are 0
	int i;
	for (i = 0; i < RUNNINGAVG; i++) {
		tempRange[i] = 0;
	}

	//Populate the running temp array with real values before starting the main loop
	while (tempRange[0] == 0) {
		AddAvg(thermocouple.readCelsius(), tempRange);
		delay(TEMPADDINTERVAL); //A necessary evil to populate the array with good data
	}
	//end removed
}

// the loop routine runs over and over again forever:
void loop() {
	if (lastTempGrab >= TEMPADDINTERVAL) {
		lastTempGrab -= TEMPADDINTERVAL;

		if (SENDTEMP) {
			AddAvg(thermocouple.readCelsius(), tempRange);
		}
	}

	if (lastSerialAdd >= SERIALUPDATEINTERVAL) { //Send the temperature over serial every so often
		lastSerialAdd -= SERIALUPDATEINTERVAL; //subtract the update interval from the timer so if we get off a little one time it will get back to the same interval

		if (SENDTEMP) {
			Serial.print("T1S");
			Serial.println(ReadAvg(tempRange));
		}
	}
	//end removed


	transducer.Update();
	differential1.Update();
	differential2.Update();
	actuator1.Update();
	actuator2.Update();

	if (Serial.available() > 0) {
		if (isalpha(Serial.peek())) {
			String command = Serial.readStringUntil('\n');
			command = command.toUpperCase();
			char code = command.charAt(0);
			int codeNum = command.charAt(1)-'0';
			double value = command.substring(3,command.length()).toFloat();
			InterpetInput(code, codeNum, value);
		}

		else {
			while (Serial.available() > 0 && !isalpha(Serial.peek())) {
				Serial.read();
			}
		}
	}
}

void InterpetInput (char code, int codeNum, double value) {
	switch (code) {
	case 'A':
		if (codeNum == 1) {
			actuator1.SetPosition(value);
		} else {
			actuator2.SetPosition(value);
		}
		break;
	case 'V':
		Serial.print("VER");
		Serial.println(VERSION);
		break;
	default:
		break;
	}
}

//To be removed
void AddAvg (double newVal, double array[]) {
	//if (newVal > 0) {
		int i;
		for (i = 0; i < RUNNINGAVG; i++) {
			if (i < RUNNINGAVG-1) {
				array[i] = array[i+1];
			}
			else {
				array[i] = newVal;
			}
		}
	//}
}

double ReadAvg (double array[]) {
	double avg = 0;
	int i;

	for (i = 0; i < RUNNINGAVG; i++) {
		avg += array[i];
	}

	return avg/RUNNINGAVG;
}
//end removed
