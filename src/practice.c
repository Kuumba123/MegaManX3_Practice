#include <sys/types.h>
#include <stdbool.h>

typedef struct{
    bool stateMade;
    bool keepRng;
    u_char stateStageId;
    u_char route;
    bool revist;
}Practice;

Practice practice;