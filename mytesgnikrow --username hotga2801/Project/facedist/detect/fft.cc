#ifdef hpux
#include <math.h>
#endif

#ifndef WINNT
extern "C" {
#endif
#ifndef WINNT
#include <sys/time.h>
#endif
#ifndef hpux
#include <math.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#ifndef WINNT
}
#endif

#include "system.hpp"
#include "fft.hpp"

#ifndef M_PI
#define M_PI (4.0*atan(1))
#endif
#define SWAP(a,b) { Complex tmp=(a); (a)=(b); (b)=tmp; }

void ComputeFFT(Complex *data, int n, int forward)
{
  int isign=forward?-1:1;
  int j=0;
  for (int i=0; i<n; i++) {
    if (i<j) SWAP(data[i],data[j]);
    int m=n>>1;
    while (m>=2 && (j & m)!=0) {
      j-=m;
      m>>=1;
    }
    j+=m;
  }
  int max=2;
  int max2=1;
  while (max<=n) {
    switch (max) {
      case 2: {
        for (int i=0; i<n; i+=2) {
          int j=i+1;
          Complex temp=data[i];
          data[i]+=data[j];
          data[j]=temp-data[j];
        }}
        break;
      case 4: {
        for (int i=0; i<n; i+=4) {
          int j=i+2;
          Complex temp=data[i];
          data[i]+=data[j];
          data[j]=temp-data[j];
          int ii=i+1;
          int jj=ii+2;
          temp=data[ii];
          if (forward) data[jj].timesI3(); else data[jj].timesI();
          data[ii]+=data[jj];
          data[jj]=temp-data[jj];
        }}
        break; 
      default: {
        double theta=isign*M_PI/max2;
        Complex wtemp(cos(theta),sin(theta));
        Complex w(1.0,0.0);
        for (int m=0; m<max2; m++) {
          for (int i=m; i<n; i+=max) {
            j=i+max2;
            Complex temp=w*data[j];
            data[j]=data[i]-temp;
            data[i]+=temp;
          }
          w=w*wtemp;
        }}
        break;
    }
    max2=max;
    max<<=1;
  }
  if (!forward) {
    Complex factor(1.0/n,0);
    for (int i=0; i<n; i++)
      data[i]=data[i]*factor;
  }
}

#if 0

#define MAX 32

static struct timeval StartFrameTime;

void StartFrameTimer()
{
  gettimeofday(&StartFrameTime);
}

void StopFrameTimer()
{
  struct timeval thistime;
  gettimeofday(&thistime);
  double diff=1e-6*(thistime.tv_usec-StartFrameTime.tv_usec)+
    (thistime.tv_sec-StartFrameTime.tv_sec);
  fprintf(stderr,"FPS=%.2f  SPF=%.2f\n",1.0/diff,diff);
}

int main(int argc, char *argv[])
{
  int original[MAX];
  Complex data[MAX];
  for (int i=0; i<MAX; i++)
  {
    original[i]=random() % 256;
    data[i]=Complex(original[i]);
    //    fprintf(stderr,"%g\n",data[i].a);
  }
  //  fprintf(stderr,"\n");

  StartFrameTimer();
  for (int j=0; j<1000*32; j++)
  {
    ComputeFFT(data,MAX,1);
    ComputeFFT(data,MAX,0);
  }
  StopFrameTimer();

  for (i=0; i<MAX; i++)
  {
    int val=(int)floor(0.5+data[i].a);
    if (val!=original[i]) 
      fprintf(stderr,"%g+i%g ERROR\n",data[i].a,data[i].b); else
        fprintf(stderr,"%g+i%g\n",data[i].a,data[i].b);
  }
  return 0;
}

#endif
