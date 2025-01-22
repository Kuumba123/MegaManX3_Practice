#include <sys/types.h>

extern u_char titleText[];
extern u_char titleText2[];
extern u_char titleText3[];
extern u_char titleText4[];
extern u_char titleText5[];

static int * titleP[] = {
    &titleText,&titleText2,&titleText3,&titleText4,&titleText5
};