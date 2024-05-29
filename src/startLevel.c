#include <common.h>

void SetupUpgrades();

static void StartLevel(Thread * t)
{
    SetupUpgrades();
    *((char*)0x800d7ff5) = 0; //draw flags
    *((char*)0x800d8118) = 0; //pause menu related
    gameMode3 = 1;
}