/*
 * Actuator.h
 *
 *  Created on: Aug 24, 2016
 *      Author: Caroline Longnecker
 */

#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "Arduino.h"
#include "DataCollector.h"

#ifndef ACTUATORAVGLEN
#define ACTUATORAVGLEN 20
#endif

#ifndef CONTROLUPDATE
#define CONTROLUPDATE 5
#endif

#ifndef MAXSERIALDIGITS
#define MAXSERIALDIGITS 3
#endif

#ifndef FASTSPEED
#define FASTSPEED 240
#endif

class Actuator : public DataCollector {
public:
	elapsedMillis controlUpdate;

	Actuator (int dir1Pin, int dir2Pin, int enPin, int feedPin, float startPos, float tol, int num, float scalar, float offset);
	void Update();
	bool MoveActuator (double currentPosion);
	float AvgData ();
	void Fault ();
	void SetPosition (double newPosition);
	void SetDependent (Actuator *depend);
	float GetSetPosition ();
	float ReadData ();
	void AddData ();
	void DisableSerial ();
	float* GetData ();

private:
	bool fault;
	int dir1;
	int dir2;
	int enable;
	float runout;
	float tolerance;
	float setPosition;

	Actuator *dependent;
};

#endif /* ACTUATOR H */
