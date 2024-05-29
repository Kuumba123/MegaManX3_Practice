#include <sys/types.h>

extern u_char titleText[];
extern u_char titleText2[];
extern u_char titleText3[];

static int * titleP[] = {
    &titleText,&titleText2,&titleText3
};