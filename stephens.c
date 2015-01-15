#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdarg.h>
#include "kolmogorov.h"

void printTable();

/*
M. A. Stephens,
"EDF Statistics for Goodness of Fit and Some Comparisons"
J. of Amer. Statist. Assoc., 69, 730-737 (1974).
http://www.math.utah.edu/~morris/Courses/6010/p1/writeup/ks.pdf
Empirical Distribution Function EDF  F[i].
Reject hypothesis at chosen level of significance.
(double) sqN= sqrt(n) - .01 + .85/sqrt(n);
Probability{max [sqN*fabs(F-F^*)]>D}=alpha
Table 1, 1.3 Modification, mu and sigma unknown.
D_alpha refers to upper tail critical value of the 
Kolmogorov-Smirnov statistics at level alpha,
giving a (1-alpha) confidential interval.
--------------
alpha    D
==============
15%     0.775
10%     0.819
5%      0.895
2.5%    0,995
1.0%    1.035
--------------
*/
void stephens(double x[], int n)
{
  double D, dinstance; 
  double mu, sigma;
  double sqN;
  D = dinstance_normal(x, n, false, &mu, &sigma);
  sqN =  sqrt(n) - .01 + .85/sqrt(n);
  dinstance = D*sqN;

  printf("\n");
  printTable();
  printf("n         = %d\n",n);
  printf("mu        = %f\n",mu);
  printf("sigma     = %f\n",sigma);
  printf("D         = %f\n",D);
  printf("sqrt(N)   = %f\n",sqN); 
  printf("D*sqrt(N) = %f\n",dinstance);
  printf("\n");
}

void printTable()
{
  printf("=============================================\n");
  printf("alpha   0.15    0.1     0.5     0.25    0.01 \n");
  printf("Dmax    0.775   0.819   0.895   0.995   1.035\n");
  printf("=============================================\n");
}


void readArray(FILE *fp, double x[], int n)
{
  int i = 0;
  double temp;
  int m;

  m = 0;
  /* Read the file of numbers into array */  
  printf("Reading array....\n");
  while( fscanf(fp,"%lf\n",&temp) != EOF )
  {
    x[i] = temp;
    i++;
    if (((int)(100*(i/(n+0.)))) == m)
    {
      printf ("\r\r\r\r%d%%",m);
      fflush(stdout);
      m++;
    }
  }   
  printf("\r\r\r\rDone!\n");
}

int main(int argc, char *argv[])
{
  FILE *fp;
  double *x = NULL;
  int n;

  if (argc != 2) {
    fprintf(stderr, "Error: missing input file name\n");
    exit(-2);
  }
  fp = fopen(argv[1],"r");
  
  if (fp == NULL) {
    fprintf(stderr, "Error: Can not open input file: %s\n",argv[1]);
    exit(-1);
  }

  /* Read the number of elements */
  fscanf(fp,"#%d\n",&n);
  x = malloc(sizeof(double)*n);

  readArray(fp, x, n);
  fclose(fp);

  stephens(x,n);
  
  free(x);
}



