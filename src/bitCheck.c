#include <common.h>

uint8_t SpawnBitCheck()
{
    // Spawn Bit if holding L1, but not R1
    return (buttonsHeld & PAD_L1) == PAD_L1 && (buttonsHeld & PAD_R1) != PAD_R1;
}
