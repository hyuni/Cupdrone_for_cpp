/*
 * actuatorMode.h
 *
 *  Created on: 2016. 8. 23.
 *      Author: ChoYG
 */

#ifndef SRC_ACTUATORMODE_ACTUATORMODE_H_
#define SRC_ACTUATORMODE_ACTUATORMODE_H_

#include "cupdroneapi.h"
#include "actuatorModeEnum.h"
#include "globalEnum.h"

class actuatorMode
{
public :
	static actuatorMode* getInstance() { return &actuatorModeInstance; }

	void receiveOutput(int16_t* pid, int16_t throttle);
	void receiveCommand(int16_t * rcCommand);

	void quadXwrite(void);
	void quadXmode(void);

	void testWrite(void);
private :

	void actuatorStop(void);

	class output_t
	{
	public :
		int16_t throttle;
		int16_t pid[3];

		int16_t command[3];
	private :
	};
	output_t output;

	static actuatorMode actuatorModeInstance;
};

#endif /* SRC_ACTUATORMODE_ACTUATORMODE_H_ */
