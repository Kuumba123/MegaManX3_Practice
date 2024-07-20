#ifndef PRACTICE_H
#define PRACTICE_H
#include <stdint.h>
#include <stdbool.h>

typedef struct{
    bool stateMade;
    bool keepRng;
    uint8_t stateStageId;
    uint8_t route;
    bool revist;
}Practice;

extern Practice practice;

#endif