/*
 * Differential.h
 *
 *  Created on: Aug 25, 2016
 *      Author: cosmo
 */

#ifndef DIFFERENTIAL_H_
#define DIFFERENTIAL_H_

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

class Differential {
public:
	elapsedMillis avgUpdate = 0;
	elapsedMillis serialUpdate = 0;

	Differential(int tPin, int num);
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

#endif /* DIFFERENTIAL_H_ */
