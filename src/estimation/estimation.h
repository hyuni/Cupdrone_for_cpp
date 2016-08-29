/*
 * estimation.h
 *
 *  Created on: 2016. 8. 17.
 *      Author: ChoYG
 */

#ifndef SRC_ESTIMATION_ESTIMATION_H_
#define SRC_ESTIMATION_ESTIMATION_H_

#include "cupdroneapi.h"

class estimation
{
public :
	static estimation* getInstance() { return &estimationInstance; }

	void process(uint32_t period);
	void testPrint(void);
private :
	uint8_t inertialEstimate(int16_t* acc, int16_t* angularVelocity ,int16_t* anglem, uint32_t period);

	static estimation estimationInstance;
};



#endif /* SRC_ESTIMATION_ESTIMATION_H_ */
