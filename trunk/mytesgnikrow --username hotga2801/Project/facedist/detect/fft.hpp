#ifndef FFTIncluded
#define FFTIncluded

#ifndef hpux
extern "C" {
#include <math.h>
}
#endif

#ifdef hpux
#include <math.h>
#endif

class Complex 
{
  public:
  double a, b;
  
  Complex(double aa, double bb) { a=aa; b=bb; }
  Complex(double aa) { a=aa; b=0; }
  Complex() { a=b=0; }

  inline void timesI() { double tmp=a; a=-b; b=tmp; }
  inline void timesI2() { a=-a; b=-b; }
  inline void timesI3() { double tmp=a; a=b; b=-tmp; }
};

inline Complex operator+(Complex &n1, Complex &n2) {
  return Complex(n1.a+n2.a,n1.b+n2.b);
}

inline Complex operator-(Complex &n1, Complex &n2) {
  return Complex(n1.a-n2.a,n1.b-n2.b);
}

inline Complex operator*(Complex &n1, Complex &n2) {
  return Complex(n1.a*n2.a-n1.b*n2.b,n1.a*n2.b+n1.b*n2.a);
}

inline Complex operator-=(Complex &n1, Complex &n2) {
  n1.a-=n2.a; n1.b-=n2.b;
  return n1;
}

inline Complex operator+=(Complex &n1, Complex &n2) {
  n1.a+=n2.a; n1.b+=n2.b;
  return n1;
}

inline double Abs(Complex &n) {
  return sqrt(n.a*n.a+n.b*n.b);
}

void ComputeFFT(Complex *data, int nn, int forward);

#endif
