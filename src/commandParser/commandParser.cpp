/*
 * commandParser.cpp
 *
 *  Created on: 2016. 8. 11.
 *      Author: ChoYG
 */

#include "commandParser.h"
#include "modeController.h"
#include "attitudeMode.h"
#include "globalClass.h"
#include "hardwareSerial.h"

commandParser commandParser::commandParserInstance;

static hardwareSerial* Serial = hardwareSerial::getInstance();
static modeController* mode = modeController::getInstance();
static attitudeMode* attitudeMode = attitudeMode::getInstance();
static globalVal* glov = globalVal::getInstance();

uint8_t commandParser::receiveBuf(void)
{
	uint8_t c;

	static uint8_t cmd;
	static uint8_t dataSize;
	static uint8_t dataBuf[Buf_t::MAXBUF_SIZE];
	static uint8_t dataBufCount = 0;

	static uint8_t checkSum = 0;

	static uint8_t state = Buf_t::WAIT_$;


	uint8_t retval = 0;

	c = Serial->read(Serial_t::BLUETOOTH);

	switch(state)
	{
	case Buf_t::WAIT_$ :
		state = (c == '$') ? Buf_t::WAIT_M : Buf_t::WAIT_$;
		break;
	case Buf_t::WAIT_M :
		state = (c == 'M') ? Buf_t::WAIT_ARROW : Buf_t::WAIT_$;
		break;
	case Buf_t::WAIT_ARROW :
		state = (c == '<') ? Buf_t::WAIT_BUFSIZE : Buf_t::WAIT_$;
		break;
	case Buf_t::WAIT_BUFSIZE :
		if(c > Buf_t::MAXBUF_SIZE)
		{
			state = Buf_t::WAIT_$; //buffer size error

			//Serial->print(Serial_t::USB,"dataSize error!\n");
		}
		else
		{
			dataSize = c;
			checkSum ^= c;
			state = Buf_t::WAIT_CMD;
		}
		break;
	case Buf_t::WAIT_CMD :
		cmd = c;
		checkSum ^= c;
		state = Buf_t::WAIT_DATA;
		break;
	case Buf_t::WAIT_DATA :
		if(dataBufCount < dataSize)
		{
			dataBuf[dataBufCount]= c;
			dataBufCount++;
			checkSum ^= c;
		}
		else
		{
			if(checkSum == c) //All data set Successfully received
			{
				//copy buffer to class's buffer
				this->dataBuf.cmd = cmd;
				this->dataBuf.dataSize = dataSize;
				uint8_t i;
				for(i = 0; i < dataSize; i++)
				{
					this->dataBuf.dataBuf[i] = dataBuf[i];
				}
				state = Buf_t::WAIT_$;;

				retval = 1;

				//Serial->print(Serial_t::USB, "All data set Successfully received\n");
			}
			else // error appeared in checksum
			{
				state = Buf_t::WAIT_$;

				//Serial->print(Serial_t::USB,"checkSum error!\n");
			}
			//initialize all static variables
			dataBufCount = 0;
			checkSum = 0;
		}

		break;
	}

	return retval;
}

void commandParser::evaluate(void)
{
	switch(this->dataBuf.cmd)
	{
	case MSP_t::ARM :
		glov->arm = 1;
		break;
	case MSP_t::DISARM :
		glov->arm = 0;
		break;
	case MSP_t::SELECT_MODE :
		uint16_t pos;
		uint16_t attitude;
		uint16_t actuator;

		pos 		= (this->dataBuf.dataBuf[0] << 8) | this->dataBuf.dataBuf[1];
		attitude 	= (this->dataBuf.dataBuf[2] << 8) | this->dataBuf.dataBuf[3];
		actuator 	= (this->dataBuf.dataBuf[4] << 8) | this->dataBuf.dataBuf[5];

		mode->commandParse(pos, attitude, actuator);

		break;
	case MSP_t::SET_RAW_RC_TINY :
		attitudeMode->receiveCommand(this->dataBuf.dataBuf);
		break;
	}
}

void commandParser::process(uint32_t period)
{
	static uint32_t time;
	static uint32_t pretime;
	time = micros();

	if((time - pretime) > period)
	{
		while(Serial->available(Serial_t::BLUETOOTH) > 0)
		{
			if(this->receiveBuf())
			{
				this->evaluate();
			}
		}

		pretime = time;
	}
}

void commandParser::testPrint(void)
{
	this->receiveBuf();

	static uint8_t count = 0;
	count++;
	if(count > 20)
	{
		char str[60];
		sprintf(str, "cmd:%d, size:%d, d1:%d, d2:%d, d3:%d, d4:%d, d5:%d\n", 	this->dataBuf.cmd,
																				this->dataBuf.dataSize,
																				this->dataBuf.dataBuf[0],
																				this->dataBuf.dataBuf[1],
																				this->dataBuf.dataBuf[2],
																				this->dataBuf.dataBuf[3],
																				this->dataBuf.dataBuf[4]);

		Serial->print(Serial_t::USB, str);

		count = 0;
	}
}
