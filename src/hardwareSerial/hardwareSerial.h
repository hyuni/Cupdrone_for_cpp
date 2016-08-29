/*
 * hardwareSerial.h
 *
 *  Created on: 2016. 8. 11.
 *      Author: ChoYG
 */

#ifndef SRC_HARDWARESERIAL_H_
#define SRC_HARDWARESERIAL_H_

#include "cupdroneapi.h"
#include "hardwareSerialEnum.h"

class hardwareSerial
{
public:
	static hardwareSerial* getInstance() { return &SerialInstance; }

	void write(uint8_t portnum, uint8_t ch);
	uint8_t read(uint8_t portnum);
	uint8_t available(uint8_t portnum);

	void print(uint8_t portnum, const char *str);
private:

	static hardwareSerial SerialInstance;
};



#endif /* SRC_HARDWARESERIAL_H_ */
