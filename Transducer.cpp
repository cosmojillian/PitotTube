/*
 * Transducer.cpp
 *
 *  Created on: Aug 24, 2016
 *      Author: Cosmo Longnecker
 */

#include "Transducer.h"

Transducer::Transducer (int tPin, int num) {
	serial = true;
	id = num;
	pin = tPin;
	int pres = ReadPressure();

	for (int i = 0; i < RUNNINGAVG; i++) {
		pressure[i] = pres;
	}
}

void Transducer::Update () {
	if (serialUpdate >= SERIALUPDATEINTERVAL) {
		serialUpdate -= SERIALUPDATEINTERVAL;
		if (serial) {
			Serial.print("A" + String(id) + "S");
			Serial.println(ReadPressure(), MAXSERIALDIGITS);
		}
	}

	if (avgUpdate >= AVGUPDATE) {
		avgUpdate -= AVGUPDATE;

		AddPressure();
	}
}

float Transducer::AvgPressure () {
	double avg = 0;

	for (int i = 0; i < RUNNINGAVG; i++) {
		avg += pressure[i];
	}

	return avg/RUNNINGAVG;
}

float Transducer::ReadPressure () {
	double raw = analogRead(pin);
	raw *= 0.00733; // 30 PSI / 4095 analog units
	return raw;
}

void Transducer::AddPressure () {
	for (int i = 0; i < RUNNINGAVG; i++) {
		if (i < RUNNINGAVG-1) {
			pressure[i] = pressure[i+1];
		}
		else {
			pressure[i] = ReadPressure();
		}
	}
}

void Transducer::DisableSerial () {
	serial = false;
}

