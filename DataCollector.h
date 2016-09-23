/*
 * DataCollector.h
 *
 *  Created on: Sep 13, 2016
 *      Author: cosmo
 */

#ifndef DATACOLLECTOR_H_
#define DATACOLLECTOR_H_

#include "Arduino.h"

class DataCollector {
public:
	elapsedMillis avgUpdate;
	elapsedMillis serialUpdate;

	DataCollector(int _pin, int _id, float _conversion, float _offset, int _dataSize, char _prefix, int _serialInterval);
	DataCollector(int _pin, int _id, float _conversion, float _offset, int _dataSize, char _prefix);
	DataCollector(int _id, int _dataSize, char _prefix);
	void Update();
	float AvgData ();
	float ReadData ();
	void AddData ();
	void DisableSerial ();
	float* GetData();

private:
	int id;
	int pin;
	int dataSize;
	int serialInterval;
	bool serial;
	float *data;
	float offset;
	float conversion;
	char prefix;
};

#endif /* DATACOLLECTOR_H_ */
