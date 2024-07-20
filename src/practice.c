#include <stdint.h>
#include <stdbool.h>

typedef struct{
    bool stateMade;
    bool keepRng;
    uint8_t stateStageId;
    uint8_t route;
    bool revist;
}Practice;

Practice practice;