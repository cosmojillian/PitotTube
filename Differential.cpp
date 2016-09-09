/*
 * Differential.cpp
 *
 *  Created on: Aug 25, 2016
 *      Author: cosmo
 */

#include "Differential.h"

Differential::Differential (int tPin, int num) {
	serial = true;
	id = num;
	pin = tPin;
	int pres = ReadPressure();

	for (int i = 0; i < RUNNINGAVG; i++) {
		pressure[i] = pres;
	}
}

void Differential::Update () {
	if (serialUpdate >= SERIALUPDATEINTERVAL) {
		serialUpdate -= SERIALUPDATEINTERVAL;
		if (serial) {
			Serial.print("D" + String(id) + "S");
			Serial.println(ReadPressure(), MAXSERIALDIGITS);
		}
	}

	if (avgUpdate >= AVGUPDATE) {
		avgUpdate -= AVGUPDATE;

		AddPressure();
	}
}

float Differential::AvgPressure () {
	double avg = 0;

	for (int i = 0; i < RUNNINGAVG; i++) {
		avg += pressure[i];
	}

	return avg/RUNNINGAVG;
}

float Differential::ReadPressure () {
	double raw = analogRead(pin)-1;
	raw *= 0.06077; // 248.84 Pascals / 4095 analog units
	return raw;
}

void Differential::AddPressure () {
	for (int i = 0; i < RUNNINGAVG; i++) {
		if (i < RUNNINGAVG-1) {
			pressure[i] = pressure[i+1];
		}
		else {
			pressure[i] = ReadPressure();
		}
	}
}

void Differential::DisableSerial () {
	serial = false;
}


