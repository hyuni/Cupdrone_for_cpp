/*
 * attitudeMode.cpp
 *
 *  Created on: 2016. 8. 16.
 *      Author: ChoYG
 */

#include "attitudeMode.h"

#include "actuatorMode.h"
#include "globalClass.h"
#include "hardwareSerial.h"

attitudeMode attitudeMode::attitudeModeInstance;

static hardwareSerial* Serial = hardwareSerial::getInstance();

void attitudeMode::receiveCommand(const uint8_t data[Axis_t::COMMAND_LENGTH])
{
	uint8_t buf;
	int i;
	for(i = 0; i < Axis_t::COMMAND_LENGTH; i++)
	{
		buf = data[i];

		if(i < 2) // ROLL, PITCH
		{
			this->desired.rawData[i] = (buf*4) - 500;
			this->desired.command[i] = this->desired.rawData[i];
			if(this->desired.command[i] > 450)
			{
				this->desired.command[i] = 450;
			}
			else if(this->desired.command[i] < -450)
			{
				this->desired.command[i] = -450;
			}
		}
		else if(i == 2) // YAW
		{
			this->desired.rawData[i] = -(buf*4) + 500;
			this->desired.command[i] = this->desired.rawData[i];
		}
		else // THROTTLE
		{
			this->desired.rawData[i] = (buf*4) + 1000;
			this->desired.command[i] = this->desired.rawData[i];
			if(this->desired.command[i] > 1850)
			{
				this->desired.command[i] = 1850;
			}
			else if(this->desired.command[i] < 1150)
			{
				this->desired.command[i] = 1150;
			}
		}
	}
}

void attitudeMode::receiveInertial(const int16_t* acc, int16_t* angularVelocity, int16_t* angle)
{
	uint8_t i;
	for(i = 0; i < 3; i++)
	{
		this->estimated.acc[i] = acc[i];
		this->estimated.angularVelocity[i] = angularVelocity[i];
		this->estimated.angle[i] = angle[i];
	}
}

void attitudeMode::controller_t::stdPIDcontroller(int16_t* desireAngle, int16_t* estimateAngle, int16_t* angularVelo, int16_t* output)
{
	int32_t errorAngle;
	int32_t errorYaw;

	const uint8_t 	anglekP = 90
					, anglekI = 10;

	static int32_t 	angleIBuf[2] = {0,0}
					,yawIBuf;

	int32_t 		anglePTerm = 0
					, angleITerm = 0
					, yawPTerm = 0
					, yawITerm = 0;

	const uint8_t 	gyrokP[3] = {40, 40, 85}
					, gyrokI[3] = {30, 30 , 45}
					, gyrokD[3] = {23, 23, 0};

	static int32_t 	lastGyro[3] = {0,0,0}
					, gyroDbuf1[3] = {0,0,0}
					, gyroDbuf2[3] = {0,0,0};

	int32_t gyroPTerm = 0
			, gyroDTerm = 0;
	int32_t gyroDelta
			, deltaSum;

	int i;
	for(i = 0; i < 3; i++)
	{
		if(i != 2) // ROll, PITCH
		{
			errorAngle = constrain(2 * desireAngle[i], -500, 500) - estimateAngle[i];

			anglePTerm = errorAngle * anglekP / 100;
			anglePTerm = constrain(anglePTerm, -500, 500);

			angleIBuf[i] = constrain(angleIBuf[i] + errorAngle, -10000, 10000);
			angleITerm = (angleIBuf[i] * anglekI) >> 12;

			output[i] = anglePTerm + angleITerm;
		}
		else //YAW
		{
			yawPTerm = desireAngle[i];

			errorYaw = (desireAngle[i] * 10 * 8 / gyrokP[i]) - angularVelo[i];
			yawIBuf = constrain(yawIBuf + errorYaw, -16000, 16000);
			if(abs(angularVelo[i]) > 640)
				yawIBuf = 0;

			yawITerm = (yawIBuf / 125 * gyrokI[i]) >> 6;

			output[i] = yawPTerm + yawITerm;

		}

		gyroPTerm = angularVelo[i] * gyrokP[i] / 10 / 8;

		gyroDelta = angularVelo[i] - lastGyro[i];
		lastGyro[i] = angularVelo[i];
		deltaSum = gyroDbuf1[i] + gyroDbuf2[i] + gyroDelta;
		gyroDbuf2[i] = gyroDbuf1[i];
		gyroDbuf1[i] = gyroDelta;
		gyroDTerm = (deltaSum * gyrokD[i]) / 32;

		output[i] -= (gyroPTerm + gyroDTerm);
	}
}

void attitudeMode::angleMode(void)
{
	this->controller.stdPIDcontroller(	this->desired.command
										, this->estimated.angle
										, this->estimated.angularVelocity
										, this->output.pid);

	actuatorMode* actu = actuatorMode::getInstance();
	actu->receiveOutput(this->output.pid, this->desired.command[Axis_t::THROTTLE]);
	actu->receiveCommand(this->desired.command);

	if(this->desired.rawData[Axis_t::THROTTLE] < Axis_t::MIN_THROTTLE)
	{
		globalVal* glov = globalVal::getInstance();

		glov->stopActuator = 1;
	}

	/*
	globalVal* glov = globalVal::getInstance();

	glov->stopActuator = 1;
	this->testPrint();
	*/

}

void attitudeMode::testPrint(void)
{
	char str[200];

	static uint32_t time;
	static uint32_t pretime;
	time = micros();

	if((time - pretime) > 200000)
	{
		//sprintf(str, "%d %d %d %d\n", this->desired.command[0] , this->desired.command[1], this->desired.command[2], this->desired.command[3]);
		/*
		sprintf(str, "acc : %d %d %d\ngyro : %d %d %d\nangle : %d %d\n\n", 	this->estimated.acc[0],this->estimated.acc[1],this->estimated.acc[2],
																		this->estimated.angularVelocity[0],this->estimated.angularVelocity[1],this->estimated.angularVelocity[2],
																		this->estimated.angle[0],this->estimated.angle[1]);
		*/
		//sprintf(str, "PID output : %d, %d, %d\n", this->output.pid[0], this->output.pid[1], this->output.pid[2]);

		Serial->print(Serial_t::USB, str);
		pretime = time;
	}
}
