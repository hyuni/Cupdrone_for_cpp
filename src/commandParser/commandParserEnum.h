/*
 * commandParserEnum.h
 *
 *  Created on: 2016. 8. 12.
 *      Author: ChoYG
 */

#ifndef SRC_COMMANDPARSER_COMMANDPARSERENUM_H_
#define SRC_COMMANDPARSER_COMMANDPARSERENUM_H_

namespace Buf_t
{
	enum
	{
		MAXBUF_SIZE = 64,

		WAIT_$ = 0,
		WAIT_M,
		WAIT_ARROW,
		WAIT_BUFSIZE,
		WAIT_CMD,
		WAIT_DATA,
		CHECKSUM,
	};
}

namespace MSP_t
{
	enum
	{
		LOGGING = 148,
		FLIP,

		SET_RAW_RC_TINY,
		ARM,
		DISARM,

		TRIM_UP,
		TRIM_DOWN,
		TRIM_LEFT,
		TRIM_RIGHT,

		TRIM_UP_FAST,
		TRIM_DOWN_FAST,
		TRIM_LEFT_FAST,
		TRIM_RIGHT_FAST,

		SELECT_MODE,
	};
}



#endif /* SRC_COMMANDPARSER_COMMANDPARSERENUM_H_ */
