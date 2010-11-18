#ifndef MyHebpIncluded
#define MyHebpIncluded

#ifndef WINNT
extern "C" {
#endif
#ifndef NoTcl
#include <tcl.h>
#endif

#ifndef NoHEBP
#undef EXTERN
#define INCLUDE_PROTOTYPES_HERE
#include <general.h>
#include <hebp.h>
#include <hebp_externs.h>
#endif
#ifndef WINNT
}
#endif

#include "fast2.hpp"

extern FastForwardStruct *real_fast;
#ifndef NoHEBP
extern network *real_net;
#endif
extern float real_inputs[30*30];
extern float real_outputs[30*30*2];
extern float real_targets[30*30*2];
extern int real_numNetworks;
extern FastForwardStruct **real_fast_list;

#ifndef NoHEBP
void HEBPInit(char *directory, int numNetworks, char *networkList[],
              int needbuf);
#endif

void FastInit(char *directory, int numNetworks, char *networkList[]);
void FastAllocateNetworks(int num);
void FastInitNetwork(int num, int numNetworks, char *networkList[]);

void MyHebp_InitNoTcl();
#ifndef NoTcl
int MyHebp_Init(Tcl_Interp *interp);
#endif

#endif

