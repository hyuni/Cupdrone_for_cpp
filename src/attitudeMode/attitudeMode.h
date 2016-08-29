/*
 * attitudeMode.h
 *
 *  Created on: 2016. 8. 16.
 *      Author: ChoYG
 */

#ifndef SRC_ATTITUDEMODE_ATTITUDEMODE_H_
#define SRC_ATTITUDEMODE_ATTITUDEMODE_H_

#include "cupdroneapi.h"
#include "globalEnum.h"

class attitudeMode
{
public :
	static attitudeMode* getInstance() { return &attitudeModeInstance; }

	void receiveCommand(const uint8_t data[Axis_t::COMMAND_LENGTH]);
	void receiveInertial(const int16_t* acc, int16_t* angularVelocity, int16_t* angle);

	void angleMode(void);

	void testPrint(void);

	class output_t
	{
	public :
		int16_t pid[Axis_t::MAX_AXIS];
	private :
	};
	output_t output;

private :
	class desired_t
	{
	public :
		int16_t rawData[Axis_t::COMMAND_LENGTH];
		int16_t command[Axis_t::COMMAND_LENGTH];
	private :
	};
	desired_t desired;

	class estimated_t
	{
	public :
		int16_t acc[Axis_t::MAX_AXIS];
		int16_t angularVelocity[Axis_t::MAX_AXIS];
		int16_t angle[Axis_t::MAX_AXIS];
	private :
	};
	estimated_t estimated;

	class controller_t
	{
	public :
		void stdPIDcontroller(int16_t* desireAngle, int16_t* estimateAngle, int16_t* angularValo, int16_t* output);
	private :
	};
	controller_t controller;

	static attitudeMode attitudeModeInstance;
};

#endif /* SRC_ATTITUDEMODE_ATTITUDEMODE_H_ */
