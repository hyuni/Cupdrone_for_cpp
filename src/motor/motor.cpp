/*
 * motor.cpp
 *
 *  Created on: 2016. 8. 23.
 *      Author: ChoYG
 */

#include "motor.h"

quadMotor quadMotor::quadMotorInstance;

void quadMotor::write(uint8_t num, uint16_t val)
{
	pwmWriteBrushed(num, val);
}
