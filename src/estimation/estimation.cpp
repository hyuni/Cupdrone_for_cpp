/*
 * estimation.cpp
 *
 *  Created on: 2016. 8. 17.
 *      Author: ChoYG
 */

#include "estimation.h"
#include "inertialSensor.h"
#include "hardwareSerial.h"
#include "attitudeMode.h"

estimation estimation::estimationInstance;

static inertialSensor* imu = inertialSensor::getInstance();
static hardwareSerial* Serial = hardwareSerial::getInstance();
static attitudeMode* atti = attitudeMode::getInstance();

uint8_t estimation::inertialEstimate(int16_t* acc, int16_t* angularVelocity ,int16_t* angle, uint32_t period)
{
	uint8_t retval = 0;

	int16_t accBuf[3];
	int16_t gyroBuf[3];
	int16_t accZoffset;

	imu->acc.read(accBuf);
	imu->gyro.read(gyroBuf);
	accZoffset = imu->accZoffset;

	if(imu->calibrate(accBuf, gyroBuf, accZoffset))
	{
		uint8_t i;
		for(i = 0; i < 3; i++)
		{
			acc[i] = accBuf[i];
			angularVelocity[i] = gyroBuf[i];
		}

		imu->calculateAngle(acc, angularVelocity, angle, period);

		retval = 1;
	}

	return retval;
}

void estimation::process(uint32_t period)
{
	static uint32_t time;
	static uint32_t pretime;
	time = micros();

	int16_t acc[3];
	int16_t gyro[3];
	int16_t angle[3];

	if((time - pretime) > period)
	{
		if(this->inertialEstimate(acc, gyro, angle, period))
		{
			atti->receiveInertial(acc, gyro, angle);
		}

		pretime = time;
	}
}
