/*
 * Actuator.h
 *
 *  Created on: Aug 24, 2016
 *      Author: Cosmo Longnecker
 */

#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "Arduino.h"

#define RUNNINGAVGLEN 20

#ifndef AVGUPDATE
#define AVGUPDATE 2
#endif

#ifndef CONTROLUPDATE
#define CONTROLUPDATE 5
#endif

#ifndef SERIALUPDATE
#define SERIALUPDATE 40
#endif

#ifndef MAXSERIALDIGITS
#define MAXSERIALDIGITS 3
#endif

#ifndef FASTSPEED
#define FASTSPEED 240
#endif

class Actuator {
public:
	elapsedMillis avgUpdate;
	elapsedMillis serialUpdate;
	elapsedMillis controlUpdate;

	Actuator (int dir1Pin, int dir2Pin, int enPin, int feedPin, float startPos, float tol, char num);
	void Update();
	bool MoveActuator (double currentPosion);
	float AvgPosition ();
	void Fault ();
	void SetPosition (double newPosition);
	void SetDependent (Actuator *depend);
	float GetSetPosition ();
	float ReadPosition ();
	void AddPosition ();
	void DisableSerial ();

private:
	char id;
	bool fault;
	bool serial;
	int dir1;
	int dir2;
	int enable;
	int feedback;
	float runout;
	float tolerance;
	float setPosition;
	float oldPosition;
	float position[RUNNINGAVGLEN];

	Actuator *dependent;
};

#endif /* ACTUATOR H */
