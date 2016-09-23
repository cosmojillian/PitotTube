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
Actuator::Actuator (int dir1Pin, int dir2Pin, int enPin, int feedPin, float startPos, float tol, int _id, float scalar, float offset) : DataCollector(feedPin, _id, scalar, offset, ACTUATORAVGLEN, 'P', 40)  {
	dir1 = dir1Pin;
	dir2 = dir2Pin;
	enable = enPin;
	setPosition = startPos;
	tolerance = tol;
	runout = 0;
	fault = false;

	dependent = NULL;

	pinMode(dir1, OUTPUT);
	pinMode(dir2, OUTPUT);
}

/**
 * Called every loop()
 * Checks what needs to be updated and updates them
 */
void Actuator::Update () {
	DataCollector::Update();

	if (controlUpdate >= CONTROLUPDATE && !fault) {
		controlUpdate -= CONTROLUPDATE;
		double curPos = ReadData();

		if (abs(AvgData() - setPosition) > tolerance) {
			MoveActuator(curPos);
			float difference = GetData()[ACTUATORAVGLEN-1] - GetData()[0];
			if (abs(difference)  >= .05) {
				//Serial.println(difference);
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
float Actuator::AvgData () {
	return DataCollector::AvgData();
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
	if (newPosition > 2.9) {
		newPosition = 2.9;
	} else if (newPosition < -2.9) {
		newPosition = -2.9;
	}

	//AddAvg(ReadPot(ACTUATORPOT1), pos1);   todo reimplement?

	if (dependent) {
		if (dependent->GetSetPosition() <= -2 || newPosition < setPosition) {
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
float Actuator::ReadData () {
	return DataCollector::ReadData();
}

/**
 * Adds a new value to the position array
 */
void Actuator::AddData() {
	DataCollector::AddData();
}

/**
 * Disables sending actuator position over Serial
 */
void Actuator::DisableSerial () {
	DataCollector::DisableSerial();
}

float* Actuator::GetData () {
	return DataCollector::GetData();
}
