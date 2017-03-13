/*
 * Transducer.h
 *
 *  Created on: Aug 24, 2016
 *      Author: Caroline Longnecker
 */

#ifndef TRANSDUCER_H_
#define TRANSDUCER_H_

#include "Arduino.h"
#include "DataCollector.h"

class Transducer : public DataCollector {
public:
	Transducer(int _pin, int _id);
	void Update();
	float AvgData ();
	float ReadData ();
	void AddData ();
	void DisableSerial ();

};

#endif /* TRANSDUCER_H_ */
