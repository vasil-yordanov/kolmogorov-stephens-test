#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define IA 16807
#define IM 2147483647
#define AM (1.0/IM)
#define IQ 127773
#define IR 2836
#define NTAB 32
#define NDIV (1+(IM-1)/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)

void srand1();
double rand_double();
double rand_gasdev();
void gauss(int argc, char *argv[]);
void pseudo_gauss(int argc, char *argv[]);

int main3(int argc, char *argv[])
{
 // initialize random generator
 srand1();
 if (argc == 1)
 {
    fprintf(stderr, "Error: missing input parameter type: <type - (gauss | ps_gauss) \n");
    exit(-1);
 }
 char* type = argv[1];
 if (strcmp("gauss",type) ==0 )
 {
   gauss(argc, argv);
 } else if (strcmp("ps_gauss",type) == 0 )
 {
   pseudo_gauss(argc, argv);
 }
 else 
 {
  fprintf(stderr, "Error: incorrect input parameter type: <type - (gauss | ps_gauss) \n");
    exit(-1);
 }
 return 0;
}
void gauss(int argc, char *argv[])
{
   FILE *f = NULL;
  int n;
  int i,j,k,m=0;
  double rnd;

  if (argc != 4) {
    fprintf(stderr, "Error: missing output file name and points number.\nFormat: simulate_data <file_name> <number_of_point> \n");
    exit(-1);
  }
  f = fopen(argv[2], "w");
  if (f == NULL)
  {
    printf("Error opening file for write!\n");
    exit(1);
  }
  n = atoi(argv[3]);
  fprintf(f, "#%d\n", n);
  for (i = 0; i < n; i++) 
  {
    rnd = rand_gasdev();
    fprintf(f, "%f\n", rnd);
    if (((int)(100*(i/(n+0.)))) == m)
    {
      printf ("\r\r\r\r%d%%",m);
      fflush(stdout);
      m++;
    }
  }
  printf("\r\r\r\rDone!\n");

  fclose(f);

}
void pseudo_gauss(int argc, char *argv[])
{
  FILE *f = NULL;
  int count;
  int i,j,k,m;
  double rnd;

  if (argc != 5) {
    fprintf(stderr, "Error: missing output file name and points number.\nFormat: simulate_data <type - (gauss | ps_gauss> <file_name> <number_of_point> <number_of_rnd_components>\n");
    exit(-1);
  }
  f = fopen(argv[2], "w");
  if (f == NULL)
  {
    printf("Error opening file for write!\n");
    exit(1);
  }
  count = atoi(argv[3]);
  k = atoi(argv[4]);
  m = 2*k;
 
  fprintf(f, "#%d\n", count);
  for (i = 0; i < count; i++) 
  {
    rnd = 0;
    for (j =0; j<m; j++)
    {
       rnd+=rand_double();
    }
    rnd -= k;
    rnd /= sqrt(m/12.); 
    fprintf(f, "%f\n", rnd);
  }

  fclose(f);

}

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

