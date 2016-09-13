/*
 * Transducer.cpp
 *
 *  Created on: Aug 24, 2016
 *      Author: Cosmo Longnecker
 */

#include "Transducer.h"

Transducer::Transducer (int _pin, int _id) : DataCollector(_pin, _id, 0.00733, 15, 'A') {

}

void Transducer::Update () {
	DataCollector::Update();
}

float Transducer::AvgData () {
	return DataCollector::AvgData();
}

float Transducer::ReadData () {
	return DataCollector::ReadData();
}

void Transducer::AddData () {
	DataCollector::AddData();
}

void Transducer::DisableSerial () {
	DataCollector::DisableSerial();
}

