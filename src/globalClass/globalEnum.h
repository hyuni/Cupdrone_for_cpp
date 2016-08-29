/*
 * globalEnum.h
 *
 *  Created on: 2016. 8. 24.
 *      Author: ChoYG
 */

#ifndef SRC_GLOBALCLASS_GLOBALENUM_H_
#define SRC_GLOBALCLASS_GLOBALENUM_H_

namespace Axis_t
{
	enum
	{
		COMMAND_LENGTH = 4,
		MAX_AXIS = 3,

		ROLL = 0,
		PITCH,
		YAW,
		THROTTLE,

		MIN_THROTTLE = 1100,
	};
}

#endif /* SRC_GLOBALCLASS_GLOBALENUM_H_ */
