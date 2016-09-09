/*
 * Actuator.cpp
 *
 *  Created on: Aug 24, 2016
 *      Author: Cosmo Longnecker
 */

#include "Actuator.h"

/**
 * Constructs a new actuator object
 */
Actuator::Actuator (int dir1Pin, int dir2Pin, int enPin, int feedPin, float startPos, float tol, char num) {
	dir1 = dir1Pin;
	dir2 = dir2Pin;
	enable = enPin;
	setPosition = startPos;
	tolerance = tol;
	runout = 0;
	oldPosition = ReadPosition();
	feedback = feedPin;
	fault = false;
	id = num;
	serial = true;

	dependent = NULL;
	serialUpdate = 0;
	avgUpdate = 0;
	controlUpdate = 0;

	int i;
	for (i = 0; i < RUNNINGAVGLEN; i++) {
		position[i] = oldPosition;
	}

	pinMode(dir1, OUTPUT);
	pinMode(dir2, OUTPUT);
}

/**
 * Called every loop()
 * Checks what needs to be updated and updates them
 */
void Actuator::Update () {
	if (avgUpdate >= AVGUPDATE) {
		avgUpdate -= AVGUPDATE;
		AddPosition();
	}

	if (serialUpdate >= SERIALUPDATE) {
		serialUpdate -= SERIALUPDATE;

		if (serial) {
			Serial.print("P" + String(id) + "S");
			Serial.println(AvgPosition(), MAXSERIALDIGITS);
		}
	}

	if (controlUpdate >= CONTROLUPDATE && !fault) {
		controlUpdate -= CONTROLUPDATE;
		double curPos = ReadPosition();

		if (abs(AvgPosition() - setPosition) > tolerance) {
			MoveActuator(curPos);
			float difference = position[RUNNINGAVGLEN-1] - position[0];
			if (abs(difference)  >= .05) {
				Serial.println(difference);
				if (difference > 0 && (curPos - setPosition) > 0) {
					//Fault();
					Serial.println("Wrong way");
				} else if (difference < 0 && (curPos - setPosition) < 0) {
					//Fault();
					Serial.println("Wrong way");
				}
			}
		}
		//todo Check for faults, etc?
	}
}

/**
 * Sets the actuator to move if the current position is outside of tolerance
 */
bool Actuator::MoveActuator (double curPos) {
	double difference = abs(curPos - setPosition);

	if (difference > tolerance) {
		if (difference > 1) {
			analogWrite(enable, FASTSPEED);
		} else {
			int speed = (pow(difference,3)*70) + 80;
			analogWrite(enable, speed);
		}
		if (setPosition > curPos) {
			digitalWrite(dir1, LOW);
			digitalWrite(dir2, HIGH);
		} else {
			digitalWrite(dir1, HIGH);
			digitalWrite(dir2, LOW);
		}
		return true;
	} else {
		digitalWrite(dir1, LOW);
		digitalWrite(dir2, LOW);
		analogWrite(enable, 0);
		return false;
	}
}

/**
 * Finds the average position from the position array
 */
float Actuator::AvgPosition () {
	double avg = 0;

	for (int i = 0; i < RUNNINGAVGLEN; i++) {
		avg += position[i];
	}

	return avg/RUNNINGAVGLEN;
}

/**
 * Disables the actuator in case of a fault and notifies the user via Serial
 */
void Actuator::Fault () {
	fault = true;
	Serial.println("E1SACTUATOR FAULT");
	digitalWrite(dir1, LOW);
	digitalWrite(dir2, LOW);
	analogWrite(enable, 0);
}

/**
 * Set the actuator to a new position
 */
void Actuator::SetPosition (double newPosition) {
	if (newPosition > 5.5) {
		newPosition = 5.5;
	} else if (newPosition < 0) {
		newPosition = 0;
	}

	//AddAvg(ReadPot(ACTUATORPOT1), pos1);   todo reimplement?

	if (dependent) {
		if (dependent->GetSetPosition() <= 1 || newPosition < setPosition) {
			setPosition = newPosition;
			runout = 0;
		}
	} else {
		if (newPosition < setPosition) {
			setPosition = newPosition;
			runout = 0;
		}
	}
}

/**
 * Sets this actuator's position to be dependent on another actuator object
 *
 */
void Actuator::SetDependent (Actuator *depend) {
	dependent = depend;
}

/**
 * Returns the position the actuator is set to move to
 * @return The position the actuator is set to move to
 */
float Actuator::GetSetPosition () {
	return setPosition;
}

/**
 * Reads the analog value from the actuator feedback pot and returns the position
 * @return position of the actuator
 */
float Actuator::ReadPosition () {
	double position = analogRead(feedback);
	position *= -0.00164; // 6 inches / 4095 analog units * 1.1195 real inches moved (also is inverted)
	position += 6; // subtract ~ 1 for 0 offset (turns into addition since inverted units), add 6 to get a positive 0 to 6 range (0" position is ~ at actuator's 7" position)
	return position;
}

/**
 * Adds a new value to the position array
 */
void Actuator::AddPosition() {
	int i;
	for (i = 0; i < RUNNINGAVGLEN; i++) {
		if (i < RUNNINGAVGLEN-1) {
			position[i] = position[i+1];
		}
		else {
			position[i] = ReadPosition();
		}
	}
}

/**
 * Disables sending actuator position over Serial
 */
void Actuator::DisableSerial () {
	serial = false;
}
