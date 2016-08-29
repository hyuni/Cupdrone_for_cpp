/*
 * inertialSensor.cpp
 *
 *  Created on: 2016. 8. 18.
 *      Author: ChoYG
 */

#include "inertialSensor.h"

inertialSensor inertialSensor::inertialSensorInstance;

// range: +/- 8192; +/- 2000 deg/sec
void gyroSensor::read(int16_t* gyroData)
{
	mpu6050GyroRead(gyroData);
}

void accSensor::read(int16_t* accData)
{
	mpu6050AccRead(accData);
}

//그냥 평균 구하는 겁니다. 내
uint8_t inertialSensor::calibrate(int16_t* acc, int16_t* gyro, int16_t accZoffset)
{
	static uint8_t retval = 0;

	static uint8_t completG = 0;
	static uint8_t completA = 0;

	static uint16_t calibG = 1000;
	static uint16_t calibA = 400;

	static int32_t g[3] = {0,0,0};

	static int32_t a[3] = {0,0,0};

	if(calibG > 0)
	{
		if(calibG == 1000)
		{
			g[0] = 0; g[1] = 0; g[2] = 0;
		}

		uint8_t i;
		for(i = 0; i < 3; i++)
			g[i] += gyro[i];

		if(calibG == 1)
		{
			for(i = 0; i < 3; i++)
			{
				this->baseGyro[i] = (g[i] + (1000 / 2)) / 1000;
									//g[i] /1000;
			}

			completG = 1;
		}

		calibG--;
	}

	if(calibA > 0)
	{
		if(calibA == 400)
		{
			a[0] = 0; a[1] = 0; a[2] = 0;
		}

		uint8_t i;
		for(i = 0; i < 3; i++)
			a[i] += acc[i];

		if(calibA == 1)
		{
			for(i = 0; i < 3; i++)
			{
				this->baseAcc[i] = 	(a[i] + (400 / 2)) / 400;
									//a[i] / 400;
			}
			this->baseAcc[2] -= 4096; //1기압시 측정된 값, 센서마다 다르다고 합니다. 낄낄

			completA = 1;
		}

		calibA--;
	}

	if(completG && completA)
	{
		uint8_t i;
		for(i = 0; i < 3; i++)
		{
			gyro[i] -= this->baseGyro[i];
			acc[i] -= this->baseAcc[i];
		}
		retval = 1;
	}

	return retval;
}

/*
uint8_t inertialSensor::calibrate(void)
{
	uint8_t retval = 0;

	static bool isCalib = false;

	int16_t accBuf[3];
	int16_t gyroBuf[3];

	int32_t accSum[3];
	int32_t gyroSum[3];

	if(!isCalib)
	{
		retval = 0;

		uint8_t i;
		for(i = 0; i < 10; i++)
		{
			this->acc.read(accBuf);
			this->gyro.read(gyroBuf);

			uint8_t j;
			for(j = 0; j < 3; j++)
			{
				accSum[j] += accBuf[j];
				gyroSum[j] += gyroBuf[j];
			}
		}

		for(i = 0; i < 3; i++)
		{
			accSum[i] /= 10;
			gyroSum[i] /= 10;

			this->baseAcc[i] = accSum[i];
			this->baseGyro[i] = gyroSum[i];
		}

		isCalib = true;
	}
	else
	{
		retval = 1;
	}

	return retval;
}

void inertialSensor::calculateAngle(int16_t* acc, int16_t* gyro, int16_t* angle, uint32_t period)
{
	float gyroBuf[3], accBuf[3];
	static float angleBufGyro[3] = {0.0, 0.0, 0.0};
	static float angleBufAcc[3] = {0.0, 0.0, 0.0};
	static float angleBuf[3] = {0.0, 0.0, 0.0};

	enum
	{
		axisX = 0,
		axisY,
		axisZ
	};

	uint8_t i;
	for(i = 0; i < 3; i++)
	{
		gyroBuf[i] = gyro[i] * (2.0f/8.192f);
		angleBufGyro[i] = angleBuf[i] + gyroBuf[i]*(period/1000000.0f);
	}

	for(i = 0; i < 3; i++)
	{
		accBuf[i] = acc[i];
	}
	angleBufAcc[0] = atan(-1*accBuf[axisX]/sqrt(accBuf[axisY]*accBuf[axisY] + accBuf[axisZ]*accBuf[axisZ])) * (180.0f/3.14159f);
	angleBufAcc[1] = atan(accBuf[axisY]/sqrt(accBuf[axisX]*accBuf[axisX] + accBuf[axisZ]*accBuf[axisZ])) * (180.0f/3.14159f);
	angleBufAcc[2] = 0;

	float factor = 0.96f;
	angleBuf[0] = angleBufGyro[0] * factor + angleBufAcc[0] * (1.0f - factor);
	angleBuf[1] = angleBufGyro[1] * factor + angleBufAcc[1] * (1.0f - factor);
	angleBuf[2] = angleBufGyro[2];

	for(i = 0; i < 3; i++)
	{
		angle[i] = angleBuf[i];
	}
}
*/

void inertialSensor::calculateAngle(int16_t* acc, int16_t* gyro, int16_t* angle, uint32_t period)
{
	calculateAngleMW(acc, gyro, angle, period);
}
