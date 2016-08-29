/*
 * commandParser.h
 *
 *  Created on: 2016. 8. 11.
 *      Author: ChoYG
 */

#ifndef SRC_COMMANDPARSER_COMMANDPARSER_H_
#define SRC_COMMANDPARSER_COMMANDPARSER_H_

#include "cupdroneapi.h"
#include "commandParserEnum.h"

class dataBuf_t
{
public :
	uint8_t cmd;
	uint8_t dataSize;
	uint8_t dataBuf[Buf_t::MAXBUF_SIZE];
private :
};

class commandParser
{
public :
	static commandParser* getInstance() { return &commandParserInstance; }

	void process(uint32_t period);

	void testPrint(void);
private :
	dataBuf_t dataBuf;

	uint8_t receiveBuf(void);
	void evaluate(void);

	static commandParser commandParserInstance;
};



#endif /* SRC_COMMANDPARSER_COMMANDPARSER_H_ */
