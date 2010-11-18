#include "system.hpp"

#ifdef i386

float htonf(float num)
{
  (*(long*)(&num))=htonl(*(long*)(&num));
  return num;
}

float ntohf(float num)
{
  return htonf(num);
}

double htond(double num)
{
  double tem;
  for (int j=0; j<sizeof(double); j++)
    ((char*)&tem)[j]=((char*)&num)[sizeof(double)-j-1];
  return tem;
}

double ntohd(double num)
{
  return htond(num);
}

#endif

#ifndef i386

float htonf(float num)
{
  return num;
}

float ntohf(float num)
{
  return num;
}

double htond(double num)
{
  return num;
}

double ntohd(double num)
{
  return num;
}

#endif

#ifdef __GNUC__
#include <string.h>
#endif

#ifndef WINNT
extern "C" {
#include <utmp.h>
#include <sys/param.h>
#endif
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifndef WINNT
#include <sys/time.h>
#endif
#include <fcntl.h>
#ifdef __GNUC__
#include <string.h>
#endif
#ifdef sgi
#include <bstring.h>
#else
#if !defined(__osf__) && !defined(linux)
void bcopy(const void *src, void *dst, int length);
#endif
#endif
#ifndef WINNT
}
#endif

long GetIdleTime()
{
#ifndef WINNT
#define TTYLEN 5
#define USERLOG "/etc/utmp"     /* who is logged in */
  struct utmp user;
#define LMAX sizeof(user.ut_line)
  struct stat ttystatus;
  time_t t;
  time_t idleTime;
  FILE* userLog = NULL;
  static char buffer[LMAX+TTYLEN+1] = "/dev/";

  userLog = fopen(USERLOG, "r");
  if (userLog == NULL)
  {
    fprintf(stderr, "Unable to open user log file: %s\n",
            USERLOG);
    exit(-1);
  }
  idleTime = INT_MAX;        /* initialize to "infinity" */
  while (fread((char*)&user, sizeof(user), 1, userLog) == 1)
  {
#ifdef sparc
    if (nonuser(user)) continue;
#else
    if (user.ut_type!=USER_PROCESS) continue;
#endif
    bcopy(user.ut_line, buffer + TTYLEN, LMAX);
    buffer[TTYLEN+LMAX] = 0;
    if (stat(buffer, &ttystatus) < 0) {
      //      fprintf(stderr, "idle: Can't stat %s\n", buffer);
      //      exit(-1);
      continue;
    }
    time(&t);
    t = t - ttystatus.st_atime;
    if (t<idleTime)
      idleTime = t;
  }
  fclose(userLog);
  return idleTime;
#else
  return 0;
#endif
}

#ifndef NoTcl
ByteImage *EyeMask=NULL;
ByteImage *FaceMaskImage=NULL;
#endif

long FileSize(char *fileName)
{
  struct stat statbuf;
  if (stat(fileName,&statbuf)==-1) return -1; else
    return statbuf.st_size;
}

double RandomDouble()
// Return a random number in the range 0 to 1, inclusive
{
#ifndef WINNT
  return ((double)(random())/((double)(0x7FFFFFFF)));
#else
  return ((double)(rand())/((double)(0x7FFFFFFF)));
#endif
}

#ifndef NoTcl
int RandomCmd(ClientData data, Tcl_Interp *interp,
              int argc, char *argv[])
{
  if (argc!=2)
  {
    interp->result="Usage: Random <modulus>";
    return TCL_ERROR;
  }
  int mod=atoi(argv[1]);
  sprintf(interp->result,"%ld",(long)(random() % mod));
  return TCL_OK;
}
#endif

#ifndef NoTcl
void SetEyeMask()
{
  if (EyeMask!=NULL) return;
  EyeMask=FindByteImageWithName("eyemask");
  if (EyeMask==NULL)
  {
    fprintf(stderr,"Image 'eyemask' does not exist!\n");
    exit(1);
  }
}

ByteImage *GetFaceMask()
{
  if (FaceMaskImage==NULL)
  {
    FaceMaskImage=FindByteImageWithName("facemask");
    if (FaceMaskImage==NULL)
    {
      fprintf(stderr,"Image 'facemask' does not exist!\n");
      exit(1);
    }
  }
  return FaceMaskImage;
}

ByteImage *GetEyeMask()
{
  if (EyeMask==NULL)
  {
    EyeMask=FindByteImageWithName("eyemask");
    if (EyeMask==NULL)
    {
      fprintf(stderr,"Image 'eyemask' does not exist!\n");
      exit(1);
    }
  }
  return EyeMask;
}
#endif

#ifndef WINNT
extern "C" {
extern int end[];
#include <unistd.h>
}
#endif

void Sbrk(char *message)
{
#ifndef WINNT
  fprintf(stderr,"sbrk: %s: %ld\n",message,
          ((long)(sbrk(0))-(long)(&end))/1024);
#endif
}

