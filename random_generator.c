#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "random_generator.h"

#define IA 16807
#define IM 2147483647
#define AM (1.0/IM)
#define IQ 127773
#define IR 2836
#define NTAB 32
#define NDIV (1+(IM-1)/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)

long next = -1;

double rand_gasdev()
{
  static int iset=0;
  static float gset;
  double fac, rsq, v1, v2;
  if (iset == 0) 
  {
    do
    {
      v1 = 2.0*rand_double()-1.0;
      v2 = 2.0*rand_double()-1.0;
      rsq = v1*v1+v2*v2;
    }
    while ((rsq>=1.0) || (rsq == 0.0));
    fac = sqrt(-2*log(rsq)/rsq);
    gset=v1*fac;
    iset=1;
    return v2*fac;
  } 
  else 
  {
    iset=0;
    return gset;
  }
}

long next_seed(){
  return next;
}

void set_seed(long seed){
  next = seed;
}

void srand1(){
  next = - time( NULL );
}

// worst rand generator
/*
double ran1()
{
    return rand() / (RAND_MAX + 1.);
}
*/

double ran1(){
  int j;
  long k;
  static long iy=0;
  static long iv[NTAB];
  double temp;

  if (next <= 0 || !iy){
     if (-next<1) next=1;
     else next = - next;
     for (j=NTAB+7;j>=0;j--){
       k=next/IQ;
       next=IA*(next-k*IQ)-IR*k;
       if (next<0) next += IM;
       if (j<NTAB) iv[j] = next;
     }
     iy=iv[0];
  }
  k=next/IQ;
  next=IA*(next-k*IQ)-IR*k;
  if (next<0) next += IM;
  j=iy/NDIV;
  iy=iv[j];
  iv[j] = next;
  if ((temp=AM*iy)>RNMX) return RNMX;
  else return temp;
}

double rand_double() {
  return ran1();
}

