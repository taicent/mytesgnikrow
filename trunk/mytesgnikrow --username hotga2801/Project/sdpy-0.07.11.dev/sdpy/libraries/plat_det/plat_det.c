#include "plat_det.h"

int hasSSE2()
{
    int a,b,c,d;
    cpuid(1,a,b,c,d);
    return d & (1 << 26);

}
