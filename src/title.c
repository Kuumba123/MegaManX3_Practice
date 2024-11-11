#include <common.h>
#include "practice.h"


    /* Format:
    #0 - Line Length (Terminate via 0)
    #1 - Clut + Something else (CX)
    #2 - Background Layer (0-2)
    #3 - Column
    #4 - Row */

uint8_t titleText[] = {
    20,     //Length
    0x20,
    2,
    7,
    14,
    'P','r','a','c','t','i','c','e',' ','E','d','i','t','i','o','n',' ','1','.','2',

    14,
    0x20,
    2,
    9,
    16,
    'B','Y',' ','P','O','G','C','H','A','M','P','G','U','Y',

    15,    //Length
    0x3C,
    2,
    0xA,
    0x14,
    'A','N','Y','%',' ','A','L','L',' ','S','T','A','G','E','S',

    4,
    0x20,
    2,
    0xA,
    0x16,
    'L','O','W','%',

    0xB,
    0x20,
    2,
    0xA,
    0x18,
    'O','P','T','I','O','N',0x20,'M','O','D','E',
    0
};

uint8_t titleText2[] = {
    15,    //Length
    0x20,
    2,
    0xA,
    0x14,
    'A','N','Y','%',' ','A','L','L',' ','S','T','A','G','E','S',

    4,
    0x3C,
    2,
    0xA,
    0x16,
    'L','O','W','%',

    0xB,
    0x20,
    2,
    0xA,
    0x18,
    'O','P','T','I','O','N',0x20,'M','O','D','E',
    0
};

uint8_t titleText3[] = {
    15,    //Length
    0x20,
    2,
    0xA,
    0x14,
    'A','N','Y','%',' ','A','L','L',' ','S','T','A','G','E','S',

    4,
    0x20,
    2,
    0xA,
    0x16,
    'L','O','W','%',

    0xB,
    0x3C,
    2,
    0xA,
    0x18,
    'O','P','T','I','O','N',0x20,'M','O','D','E',
    0
};

void GameStart();
void TitleRouteSelect(Thread * t);

void (*titleSelectTable[])() = {
    TitleRouteSelect,
    TitleRouteSelect,
    0x801496b0 //option mode
};

void TitleRouteSelect(Thread * t)
{
    practice.route = *((char*)t + 0x1C); //cursor
    GameStart();
}