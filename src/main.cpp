#include "cupdroneHardwareSetup.h"

#include "modeController.h"
#include "commandParser.h"
#include "estimation.h"
#include "attitudeMode.h"

//-- main
//
int main(void)
{
	cupdroneHardwareSetup();

	commandParser* cmd = commandParser::getInstance();
	modeController* mode = modeController::getInstance();
	estimation* esti = estimation::getInstance();

    while(1)
    {
    	cmd->process(3500); //3.5ms
    	esti->process(3500); //3.5ms

    	mode->process(3500); //3.5ms
    }
}
