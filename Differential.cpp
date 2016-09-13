/*
 * Differential.cpp
 *
 *  Created on: Aug 25, 2016
 *      Author: cosmo
 */

#include "Differential.h"

Differential::Differential (int _pin, int _id) : DataCollector(_pin, _id, 0.06077, 15, 'D') {

}

void Differential::Update () {
	DataCollector::Update();
}

float Differential::AvgData () {
	return DataCollector::AvgData();
}

float Differential::ReadData() {
	return DataCollector::ReadData();
}

void Differential::AddData () {
	DataCollector::AddData();
}

void Differential::DisableSerial () {
	DataCollector::DisableSerial();
}


