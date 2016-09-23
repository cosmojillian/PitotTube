/*
 * DataCollector.cpp
 *
 *  Created on: Sep 13, 2016
 *      Author: cosmo
 */

#include "DataCollector.h"

DataCollector::DataCollector(int _pin, int _id, float _conversion, float _offset, int _dataSize, char _prefix, int _serialInterval) {
	id = _id;
	pin = _pin;
	dataSize = _dataSize;
	serial = true;
	data = new float[dataSize];
	conversion = _conversion;
	prefix = _prefix;
	offset = _offset;
	serialInterval = _serialInterval;

	int _data = ReadData();

	for (int i = 0; i < dataSize; i++) {
		data[i] = _data;
	}
}

DataCollector::DataCollector(int _pin, int _id, float _conversion, float _offset, int _dataSize, char _prefix) {
	id = _id;
	pin = _pin;
	dataSize = _dataSize;
	serial = true;
	data = new float[dataSize];
	conversion = _conversion;
	prefix = _prefix;
	offset = _offset;
	serialInterval = 250;

	int _data = ReadData();

	for (int i = 0; i < dataSize; i++) {
		data[i] = _data;
	}
}

DataCollector::DataCollector(int _id, int _dataSize, char _prefix) {
	id = _id;
	pin = -1;
	dataSize = _dataSize;
	serial = true;
	data = new float[dataSize];
	conversion = 0;
	prefix = _prefix;
	offset = 0;
	serialInterval = 250;
}

void DataCollector::Update() {
	if (serialUpdate >= serialInterval) {
		serialUpdate -= serialInterval;
		if (serial) {
			Serial.print(prefix);
			Serial.print(id);
			Serial.print("S");
			Serial.println(AvgData(), 3);
		}
	}

	if (avgUpdate >= 2) {
		avgUpdate -= 2;
		AddData();
	}
}

float DataCollector::AvgData () {
	double avg = 0;

	for (int i = 0; i < dataSize; i++) {
		avg += data[i];
	}

	return avg/(float)dataSize;
}

float DataCollector::ReadData () {
	double raw = analogRead(pin);
	raw *= conversion; //Convert the raw data to a value we can use
	raw += offset;
	return raw;
}

void DataCollector::AddData () {
	for (int i = 0; i < dataSize; i++) {
		if (i < dataSize-1) {
			data[i] = data[i+1];
		} else {
			data[i] = ReadData();
		}
	}
}

void DataCollector::DisableSerial () {
	serial = false;
}

float* DataCollector::GetData () {
	return data;
}

