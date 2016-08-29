/*
 * globalClass.h
 *
 *  Created on: 2016. 8. 16.
 *      Author: ChoYG
 */

#ifndef SRC_GLOBALCLASS_GLOBALCLASS_H_
#define SRC_GLOBALCLASS_GLOBALCLASS_H_

#include "cupdroneapi.h"

class globalVal
{
public :
	static globalVal* getInstance() { return &globalValInstance; }

	uint8_t arm;

	uint8_t stopActuator;
private :
	globalVal()
	{
		arm = 0;
		stopActuator = 0;
	}

	static globalVal globalValInstance;
};



#endif /* SRC_GLOBALCLASS_GLOBALCLASS_H_ */
