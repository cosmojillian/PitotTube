/*
 * Transducer.h
 *
 *  Created on: Aug 24, 2016
 *      Author: Cosmo Longnecker
 */

#ifndef TRANSDUCER_H_
#define TRANSDUCER_H_

#include "Arduino.h"

#ifndef RUNNINGAVG
#define RUNNINGAVG 15
#endif

#ifndef SERIALUPDATEINTERVAL
#define SERIALUPDATEINTERVAL 250
#endif

#ifndef MAXSERIALDIGITS
#define MAXSERIALDIGITS 3
#endif

#ifndef AVGUPDATE
#define AVGUPDATE 2
#endif

class Transducer {
public:
	elapsedMillis avgUpdate = 0;
	elapsedMillis serialUpdate = 0;

	Transducer(int tPin, int num);
	void Update();
	float AvgPressure ();
	float ReadPressure ();
	void AddPressure ();
	void DisableSerial ();

private:
	int id;
	int pin;
	bool serial;
	float pressure[RUNNINGAVG];
};

#endif /* TRANSDUCER_H_ */
