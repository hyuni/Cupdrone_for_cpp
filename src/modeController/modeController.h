/*
 * modeController.h
 *
 *  Created on: 2016. 8. 11.
 *      Author: ChoYG
 */

#ifndef SRC_MODECONTROLLER_MODECONTROLLER_H_
#define SRC_MODECONTROLLER_MODECONTROLLER_H_

#include "cupdroneapi.h"

class modeController
{
public:
	static modeController* getInstance() { return &modeControllerInstance; }

	void testPrint(void);
	void commandParse(uint16_t pos, uint16_t attitude, uint16_t actuator);
	void process(uint32_t period);

	uint8_t posModeAvailable(void);
	uint8_t attitudeModeAvailable(void);
	uint8_t actuatorModeAvailable(void);

private:
	modeController()
	{
		posModeUse = 0;
		attitudeModeUse = 1;
		actuatorModeUse = 1;

		posModeBit = 1;
		attitudeModeBit = 4;
		actuatorModeBit = 4;
	}

	static void (*posMode[])(void);
	static void (*attitudeMode[])(void);
	static void (*actuatorMode[])(void);

	uint8_t posModeUse, attitudeModeUse, actuatorModeUse;
	uint16_t posModeBit, attitudeModeBit, actuatorModeBit;

	void modeSelect(void);

	static modeController modeControllerInstance;
};

#endif /* SRC_MODECONTROLLER_MODECONTROLLER_H_ */
