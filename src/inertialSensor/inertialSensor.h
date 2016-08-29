/*
 * inertialSensor.h
 *
 *  Created on: 2016. 8. 18.
 *      Author: ChoYG
 */

#ifndef SRC_INERTIALSENSOR_INERTIALSENSOR_H_
#define SRC_INERTIALSENSOR_INERTIALSENSOR_H_

#include "cupdroneapi.h"

class gyroSensor
{
public :
	 // range: +/- 8192; +/- 2000 deg/sec
	void read(int16_t* gyroData);
private :
};

class accSensor
{
public :
	void read(int16_t* accData);
private :
};

class inertialSensor
{
public :
	static inertialSensor* getInstance() { return &inertialSensorInstance; }

	gyroSensor gyro;
	accSensor acc;

	uint16_t accZoffset;

	uint8_t calibrate(int16_t* acc, int16_t* gyro, int16_t accZoffset);
	void calculateAngle(int16_t* acc, int16_t* gyro, int16_t* angle, uint32_t period);

private :
	inertialSensor()
	{
		baseAcc[0] = 0;
		baseAcc[1] = 0;
		baseAcc[2] = 0;

		baseGyro[0] = 0;
		baseGyro[1] = 0;
		baseGyro[2] = 0;

		accZoffset = 4096;
	}
	int16_t baseAcc[3];
	int16_t baseGyro[3];

	static inertialSensor inertialSensorInstance;
};


#endif /* SRC_INERTIALSENSOR_INERTIALSENSOR_H_ */
