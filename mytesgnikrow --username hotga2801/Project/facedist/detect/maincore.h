#ifndef __MAINCORE_H__
#define __MAINCORE_H__

#define STRSIZE 1000

#define YES 1
#define NO 0
#define TRUE 1
#define FALSE 0


/* execution mode */
#define EMD_NORMAL   1
#define EMD_COV      2
#define EMD_SPECIAL  4

// mode
#define MOD_EXIT 1
#define MOD_CAPT 2


#define BOOL int

/*
typedef BOOL int
*/


typedef struct
{
  int sx, sy, ex, ey;
} RectREC;



struct timeval *StartTimer( void );
void StopTimer( char*, struct timeval* );

#endif
