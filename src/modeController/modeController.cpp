/*
 * modeController.cpp
 *
 *  Created on: 2016. 8. 11.
 *      Author: ChoYG
 */

#include "modeController.h"

#include "attitudeMode.h"
#include "actuatorMode.h"
#include "hardwareSerial.h"

modeController modeController::modeControllerInstance;

static hardwareSerial* Serial = hardwareSerial::getInstance();

void noUse()
{
	//Serial->print(Serial_t::USB, "No Use Mode\n");
}

void posDebug()
{
	//Serial->print(Serial_t::USB, "position debug mode\n");
}

void attitudeDebug()
{
	//Serial->print(Serial_t::USB, "attitude debug mode\n");
}

void actuatorDebug()
{
	//Serial->print(Serial_t::USB, "actuator debug mode\n");
}

void posPrimary()
{
	//Serial->print(Serial_t::USB, "position primary mode\n");
}

void attitudePrimary()
{
	attitudeMode* atti = attitudeMode::getInstance();

	atti->angleMode();
	//Serial->print(Serial_t::USB, "attitude primary mode\n");
}

void actuatorPrimary()
{
	actuatorMode* actu = actuatorMode::getInstance();

	actu->quadXmode();
	//Serial->print(Serial_t::USB, "actuator primary mode\n");
}

void (*modeController::posMode[])(void) = {noUse, posDebug, posPrimary};
void (*modeController::attitudeMode[])(void) = {noUse, attitudeDebug, attitudePrimary};
void (*modeController::actuatorMode[])(void) = {noUse, actuatorDebug, actuatorPrimary};

void modeController::testPrint(void)
{
	this->commandParse(0x0001, 0x1003, 0x1007);

	Serial->print(Serial_t::USB, "select mode and running\n");

	this->modeSelect();
}

void modeController::process(uint32_t period)
{
	static uint32_t time;
	static uint32_t pretime;
	time = micros();

	if((time - pretime) > period)
	{
		this->modeSelect();

		pretime = time;
	}
}

void modeController::commandParse(uint16_t pos, uint16_t attitude, uint16_t actuator)
{
	this->posModeUse = pos >> 15;
	this->posModeBit = pos & 0x7fff;

	this->attitudeModeUse = attitude >> 15;
	this->attitudeModeBit = attitude & 0x7fff;

	this->actuatorModeUse = actuator >> 15;
	this->actuatorModeBit = actuator & 0x7fff;
}

void modeController::modeSelect(void)
{
	if(this->posModeAvailable())
	{
		uint16_t i;
		for(i = 0; i < 15; i++)
		{
			if((this->posModeBit >> i) & 0x01)
			{
				this->posMode[i]();
			}
		}
	}

	if(this->attitudeModeAvailable())
	{
		uint16_t i;
		for(i = 0; i < 15; i++)
		{
			if((this->attitudeModeBit >> i) & 0x01)
			{
				this->attitudeMode[i]();
			}
		}
	}

	if(this->actuatorModeAvailable())
	{
		uint16_t i;
		for(i = 0; i < 15; i++)
		{
			if((this->actuatorModeBit >> i) & 0x01)
			{
				this->actuatorMode[i]();
			}
		}
	}
}

uint8_t modeController::posModeAvailable(void)
{
	if(this->posModeUse)
		return 1;

	return 0;
}

uint8_t modeController::attitudeModeAvailable(void)
{
	if(this->attitudeModeUse)
		return 1;

	return 0;
}

uint8_t modeController::actuatorModeAvailable(void)
{
	if(this->actuatorModeUse)
		return 1;

	return 0;
}
