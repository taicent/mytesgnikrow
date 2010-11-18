#ifndef SystemIncluded
#define SystemIncluded

#ifndef WINNT
extern "C" {
#endif
#include <stdio.h>
#include <stdlib.h>
#ifdef _AIX
#include <time.h>
#include <net/nh.h>
#endif
#ifndef WINNT
}
#endif

extern "C" {
#ifdef sparc
#ifndef solaris
  char *sbrk(char *);
#endif
  int isatty();
#endif
  int printf(const char* ...);
#ifndef WINNT
  FILE* fdopen(int, const char*);
#endif
  int fflush(FILE*);
  int fclose(FILE*);
  int fprintf(FILE*, const char* ...);
  int fscanf(FILE*, const char* ...);
  int scanf(const char* ...);
  int pclose(FILE*);
#ifdef hpux
#define random lrand48
#define srandom srand48
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#endif
#ifdef i386
  int rand();
  void free(void *);
  unsigned sleep(unsigned);
#else
#ifndef WINNT
  #include <unistd.h>
#endif
  int sscanf(const char *, const char * ...);
#ifndef __osf__
#ifndef WINNT
  long random();
#endif
#endif
#ifdef sparc
  void srandom(int);
#endif
#endif

#ifdef __osf__
  unsigned int ntohl(unsigned int);
  unsigned int htonl(unsigned int);
#endif
#ifndef i386
#ifndef __osf__
#ifndef _AIX
#ifndef solaris
#ifndef hpux
  long ntohl(long);
  long htonl(long);
  short ntohs(short num);
  short htons(short num);
  short ntohs(short);
  short htons(short);
#endif
#endif
#endif
#endif
#endif
#ifdef solaris
#include <sys/byteorder.h>
#endif
#ifdef linux
#include <asm/byteorder.h>
#endif
};

#ifdef i386
extern "C" {
  int matherr();
};
#define DataPtr(foo) (char*)(foo)
#else
#define DataPtr(foo) (foo)
#endif

float ntohf(float);
float htonf(float);
double ntohd(double);
double htond(double);

long GetIdleTime();

#ifndef NoTcl
#include "img.hpp"

extern ByteImage *FaceMaskImage;
extern ByteImage *EyeMask;

ByteImage *GetFaceMask();
ByteImage *GetEyeMask();
void SetEyeMask();
#endif

long FileSize(char *fileName);

double RandomDouble();
#ifndef NoTcl
int RandomCmd(ClientData data, Tcl_Interp *interp,
              int argc, char *argv[]);
#endif

void Sbrk(char *message);

#ifdef WINNT
#define htonl(num) (num)
#define ntohl(num) (num)
#endif

#endif
