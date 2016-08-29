/*
 * cupdroneHardwareSetup.h
 *
 *  Created on: 2016. 8. 11.
 *      Author: ChoYG
 */

#ifndef CUPDRONEAPI_CUPDRONEHARDWARESETUP_H_
#define CUPDRONEAPI_CUPDRONEHARDWARESETUP_H_

#include "cupdroneapi.h"

void cupdroneHardwareSetup(void);

void dbg_print( char *str );

void HardFault_Handler(void);

#endif /* CUPDRONEAPI_CUPDRONEHARDWARESETUP_H_ */
