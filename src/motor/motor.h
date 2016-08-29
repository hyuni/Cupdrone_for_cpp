/*
 * motor.h
 *
 *  Created on: 2016. 8. 23.
 *      Author: ChoYG
 */

#ifndef SRC_MOTOR_MOTOR_H_
#define SRC_MOTOR_MOTOR_H_

#include "cupdroneapi.h"
#include "motorEnum.h"

class quadMotor
{
public :
	static quadMotor* getInstance() { return &quadMotorInstance; }
	void write(uint8_t num, uint16_t val);

private :
	static quadMotor quadMotorInstance;
};



#endif /* SRC_MOTOR_MOTOR_H_ */
