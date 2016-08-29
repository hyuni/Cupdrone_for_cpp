/*
 * actuatorMode.cpp
 *
 *  Created on: 2016. 8. 23.
 *      Author: ChoYG
 */

#include "actuatorMode.h"

#include "hardwareSerial.h"
#include "motor.h"
#include "globalClass.h"

actuatorMode actuatorMode::actuatorModeInstance;

void actuatorMode::receiveOutput(int16_t* pid, int16_t throttle)
{
	uint8_t i;
	for(i = 0; i < 3; i++)
	{
		this->output.pid[i] = pid[i];
	}
	this->output.throttle = throttle;
}

void actuatorMode::receiveCommand(int16_t * rcCommand)
{
	uint8_t i;
	for(i = 0; i < 3; i++)
	{
		this->output.command[i] = rcCommand[i];
	}
}

void actuatorMode::actuatorStop(void)
{
	uint8_t i;
	quadMotor* motor = quadMotor::getInstance();
	for(i = 0; i < 4; i++)
	{
		motor->write(i,1000);
	}
}

typedef struct motorTable_t
{
	float roll;
	float pitch;
	float yaw;
	float throttle;
} motorTable_t;

void actuatorMode::quadXwrite(void)
{
	quadMotor* motor = quadMotor::getInstance();

	static const motorTable_t tableQUADX[] =
	{
		//ROLL	PITCH	YAW		THROTTLE
		{-1.0f	,  1.0f	,  1.0f	, 1.0f}, //REAR_R
		{-1.0f	, -1.0f	, -1.0f	, 1.0f}, //FRONT_R
		{ 1.0f	,  1.0f	, -1.0f	, 1.0f}, //REAR_L
		{ 1.0f	, -1.0f	,  1.0f	, 1.0f}, //FRONT_L
	};

	int16_t pidYaw;
	pidYaw = constrain(	this->output.pid[Axis_t::YAW]
						, -100 -abs(this->output.command[Axis_t::YAW])
						, 100 + abs(this->output.command[Axis_t::YAW]));

	uint8_t i;

	int16_t out[4];

	for(i = 0; i < quad_t::MAXMOTOR; i++)
	{
		out[i] = 	this->output.pid[Axis_t::ROLL] * tableQUADX[i].roll
					+ this->output.pid[Axis_t::PITCH] * tableQUADX[i].pitch
					+ pidYaw * tableQUADX[i].yaw
					+ this->output.throttle * tableQUADX[i].throttle;
	}

	int16_t maxout = out[0];
	for(i = 1; i < quad_t::MAXMOTOR; i++)
	{
		if(out[i] > maxout)
			maxout = out[i];
	}

	for(i = 0; i < quad_t::MAXMOTOR; i++)
	{
		if(maxout > 1850)
			out[i] -= maxout - 1850;

		out[i] = constrain(out[i], 1150, 1850);
	}

	for(i = 0; i < quad_t::MAXMOTOR; i++)
		motor->write(i, out[i]);


	/*
	char str[100];
	hardwareSerial* Serial = hardwareSerial::getInstance();

	static uint8_t count = 0;
	count++;
	if(count > 40)
	{
		count = 0;

		sprintf(str, "0 : %d, 1 : %d, 2 : %d, 3 : %d\n", out[0], out[1], out[2], out[3]);
		Serial->print(Serial_t::USB, str);
	}
	*/
}

void actuatorMode::quadXmode(void)
{
	globalVal* glov = globalVal::getInstance();

	if(glov->arm)
	{
		if(!glov->stopActuator)
		{
			this->quadXwrite();
			//this->testWrite();
		}
		else
		{
			this->actuatorStop();
			glov->stopActuator = 0;
		}
	}
	else
	{
		this->actuatorStop();
	}
}


void actuatorMode::testWrite(void)
{
	quadMotor* motor = quadMotor::getInstance();

	motor->write(0, 1500);
	motor->write(1, 1500);
	motor->write(2, 1500);
	motor->write(3, 1500);
}
