/*
 * hardwareSerial.cpp
 *
 *  Created on: 2016. 8. 11.
 *      Author: ChoYG
 */
#include "hardwareSerial.h"

#include "cupdroneapi.h"

hardwareSerial hardwareSerial::SerialInstance;

extern core_t core;

void hardwareSerial::write(uint8_t portnum, uint8_t ch)
{
	switch(portnum)
	{
	case Serial_t::USB :
		Hw_VCom_Putch(ch);
		break;
	case Serial_t::BLUETOOTH :
		core.mainport->vTable->serialWrite(core.mainport, ch);
		break;
	case Serial_t::EXTRA :
		core.menuport->vTable->serialWrite(core.menuport, ch);
		break;
	}
}

uint8_t hardwareSerial::read(uint8_t portnum)
{
	switch(portnum)
	{
	case Serial_t::USB :
		return Hw_VCom_Getch();
		break;
	case Serial_t::BLUETOOTH :
		return core.mainport->vTable->serialRead(core.mainport);
		break;
	case Serial_t::EXTRA :
		return core.menuport->vTable->serialRead(core.menuport);
		break;
	}

	return -1;
}

uint8_t hardwareSerial::available(uint8_t portnum)
{
	switch(portnum)
	{
	case Serial_t::USB :
		return Hw_VCom_IsReceived();
		break;
	case Serial_t::BLUETOOTH :
		return core.mainport->vTable->serialTotalBytesWaiting(core.mainport);
		break;
	case Serial_t::EXTRA :
		return core.menuport->vTable->serialTotalBytesWaiting(core.menuport);
		break;
	}

	return -1;
}

void hardwareSerial::print(uint8_t portnum, const char *str)
{
    uint8_t ch;
    while ((ch = *(str++)) != 0)
    {
    	this->write(portnum, ch);
    }
}
