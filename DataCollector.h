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

	DataCollector(int _pin, int _id, float _conversion, int _dataSize, char _prefix);
	DataCollector(int _id, int _dataSize, char _prefix);
	void Update();
	float AvgData ();
	float ReadData ();
	void AddData ();
	void DisableSerial ();

private:
	int id;
	int pin;
	int dataSize;
	bool serial;
	float *data;
	float conversion;
	char prefix;
};

#endif /* DATACOLLECTOR_H_ */
