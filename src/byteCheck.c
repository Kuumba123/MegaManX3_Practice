#include <common.h>

uint8_t SpawnByteCheck()
{
    // Spawn Bit if holding R1, but not L1
    return (buttonsHeld & PAD_L1) != PAD_L1 && (buttonsHeld & PAD_R1) == PAD_R1;
}
