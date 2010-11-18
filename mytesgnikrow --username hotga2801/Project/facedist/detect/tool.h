#ifndef __TOOL_H__
#define __TOOL_H__

#ifndef PI
#define PI 3.141592653589793238
#endif

/* definitions for RGB<->HVC */
#define V(x) (11.6 * pow(x,1.0/3.0) - 1.6)
#define CUBE(x) ((x)*(x)*(x))
#define FINV(x) CUBE(((x) + 1.6) / 11.6)
#define E 0.00001

unsigned char *readim( int*, int*, int*, char* );
BOOL writeim( unsigned char*, int, int, int, char* );
void rgbtohvc( double, double, double, double*, double*, double* );
void hvctorgb( double, double, double, double*, double*, double* );



#endif
