


#include "main.h"

//initialization or configuration related to DKP
void init_digital_keypad(void)
{
    /* Set Keypad Port as input */
    KEYPAD_PORT_DDR = KEYPAD_PORT_DDR | INPUT_LINES;
}


//detect which switch is pressed and it will return the switch press value
//input mode , level or state
unsigned char read_digital_keypad(unsigned char mode)   //STATE
{
    static unsigned char once = 1;

    if (mode == LEVEL_DETECTION)  // 1 == 0
    {
        return KEYPAD_PORT & INPUT_LINES;
    }
    else
    {
        if (((KEYPAD_PORT & INPUT_LINES) != ALL_RELEASED) && once)
        {
            once = 0;

            return KEYPAD_PORT & INPUT_LINES;
        }
        else if ((KEYPAD_PORT & INPUT_LINES) == ALL_RELEASED)
        {
            once = 1;
        }
    }

    return ALL_RELEASED;
}

