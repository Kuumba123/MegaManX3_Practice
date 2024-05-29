#include <common.h>
#define HELD_TIME 120

static int timeHeld = 0;

extern void (*MainGameTable[2])();

void QuickResetCheck(Thread * thread)
{
    if ((buttonsHeld & (PAD_SELECT | PAD_START)) == (PAD_SELECT | PAD_START))
    {
        timeHeld++;
        if (timeHeld > HELD_TIME)
        {
            /* return to Stat of Game */
            thread->destP = 0x8001ab24;
            *((u_char*)0x800d7ff5) = 0;
            ThreadSleep(3);
            return;
        }
        goto JMP;
    }
    timeHeld = 0;
JMP:
    MainGameTable[gameMode](thread);
}

#undef HELD_TIME