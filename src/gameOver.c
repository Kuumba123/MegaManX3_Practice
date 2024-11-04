#include <common.h>

extern uint8_t weaponAmmo[9 * 2];

#define maverick_stages_cleared 0x800d8116
#define doppler_stages_cleared 0x800d80f2

void GameOver(Thread * thread)
{
    thread->destP = 0x8001e64c;
    *((char*)maverick_stages_cleared) = 0;
    *((char*)doppler_stages_cleared) = 0;
    gameMode2 = 0;
    gameMode3 = 0;
    gameMode4 = 0;

    for (size_t i = 0; i < 9; i++)
    {
        weaponAmmo[i * 2] = 0;
        weaponAmmo[i * 2 + 1] = 0;
    }
}

#undef maverick_stages_cleared
#undef doppler_stages_cleared
