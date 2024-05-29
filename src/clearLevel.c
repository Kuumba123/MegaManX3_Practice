#include <common.h>

extern u_char weaponAmmo[9 * 2];

void ClearedLevel(Thread * thread)
{
    thread->destP = 0x8001e64c;
    *((char*)0x800d8116) = 0;
    *((char*)0x800d80f2) = 0; //Cleared Dopler Stages
    gameMode2 = 0;
    gameMode3 = 0;
    gameMode4 = 0;

    for (size_t i = 0; i < 9; i++)
    {
        weaponAmmo[i * 2] = 0;
        weaponAmmo[i * 2 + 1] = 0;
    }
}