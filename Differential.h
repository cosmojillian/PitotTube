/*
 * Differential.h
 *
 *  Created on: Aug 25, 2016
 *      Author: Caroline
 */

#ifndef DIFFERENTIAL_H_
#define DIFFERENTIAL_H_

#include "Arduino.h"
#include "DataCollector.h"

class Differential : public DataCollector {
public:
	Differential(int _pin, int _id);
	void Update();
	float AvgData ();
	float ReadData ();
	void AddData ();
	void DisableSerial ();
};

#endif /* DIFFERENTIAL_H_ */
