#include <stdint.h>
#include "COMM/LIN_SLAVE/LIN_SlaveAppData.h"


extern uint16_t Controls;
volatile uint16_t  Switches_Status;


void Transfere(void)
{
    SlaveGetControls(&Switches_Status);
    SlaveSendControls(Controls);
}
