#include <common.h>

void StateHook(Thread *thread);

static void (*gameModeTable[])() = {
    0x8001edc4,
    StateHook,
    StateHook,
    StateHook
};