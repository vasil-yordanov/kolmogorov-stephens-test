/*
To compile this code run: gcc stephens.c -o stephens -lm
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

void printTable();
void readArray(FILE *fp, double x[], int n);
void printArray(double x[], int n);
int cmp(const void *x, const void *y);

/*
M. A. Stephens,
"EDF Statistics for Goodness of Fit and Some Comparisons"
J. of Amer. Statist. Assoc., 69, 730-737 (1974).
http://www.math.utah.edu/~morris/Courses/6010/p1/writeup/ks.pdf
Empirical Distribution Function EDF  F[i].
Reject hypothesis at chosen level of significance.
(double) sqN= sqrt(N) - .01 + .85/sqrt(n);
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

double stephens(double x[], int n, bool quiet)
{
  FILE* f;
  int i, m, i_max, i_plus_max, i_minus_max;
  double Deviation, deviation, dev, D_i, D_plus, D_minus, D_plus_max, D_minus_max, D;
  double mu, sigma, EDF, PDF; 

  // calculate sqN
  //double sqN= sqrt(n) - .01 + .85/sqrt(n);
  double sqN= sqrt(n);

  /* sort the array */
  if (!quiet) printf("Sorting array...\n");
  qsort(x, n, sizeof(double), cmp);
  if (!quiet) printf("Done!\n");

  // Calculate average value
  mu = 0.;
  m = 0;
  if (!quiet) printf("Calculating mu...\n");
  for (i=0; i<n; i++)
  {
    mu += x[i]; 
    if (!quiet) 
    if (((int)(100*(i/(n+0.)))) == m)
    {
      printf ("\r\r\r\r%d%%",m);
      fflush(stdout);
      m++;
    }
  }
  mu /= n+0.;
  if (!quiet) printf("\r\r\r\rDone!\n");

  // Calculate standart deviation
  sigma = 0.;
  m = 0;
  if (!quiet) printf("Calculating sigma...\n");
  for (i=0; i<n; i++)
  {
    sigma += (x[i]-mu)*(x[i]-mu); 
    if (!quiet) 
    if (((int)(100*(i/(n+0.)))) == m)
    {
      printf ("\r\r\r\r%d%%",m);
      fflush(stdout);
      m++;
    }
  }
  sigma = sqrt(sigma/(n-1.)); 
  if (!quiet) printf("\r\r\r\rDone!\n");


  // calculate D_max
  D_plus_max = 0;
  D_minus_max = 0;
  i_plus_max = 0;
  i_minus_max = 0;

  m = 0;
  //dev = 0;
  if (!quiet) printf("Calculating D...\n");
  for (i=0; i<n; i++)
  {
    // Parameterized Distribution Fit (PDF). PDF(x_i, mu, sigma, etc)

    // For normal distribution w(x)=1/(sqrt(2*Pi)*sigma)*exp(-(x-mu)^2/(2*sigma^2))
    PDF = 0.5*(1+erf((x[i]-mu)/(sqrt(2)*sigma)));

    // For uniform distribution w(x)=1 
    //PDF = x[i];

    // Experimentaly Determined Function (EDF)
    EDF = (i+0.5)/n;
    D_plus = ((i+1)/((double)n) - PDF); 
    //D_i = EDF - PDF; 
    //if (fabs(D_i) > fabs(dev)) dev = D_i;
    D_minus = (PDF - i/((double)n));
    if (D_plus_max<D_plus) D_plus_max = D_plus;
    if (D_minus_max<D_minus) D_minus_max = D_minus;

    // print calculation progress
    if (!quiet) 
    if (((int)(100*(i/(n+0.)))) == m)
    {
      printf ("\r\r\r\r%d%%",m);
      fflush(stdout);
      m++;
    }
  }
// print calculation progress
  if (!quiet) printf("\r\r\r\rDone!\n");
  if (D_plus_max > D_minus_max) D = D_plus_max;
  else D = D_minus_max;

  //deviation = dev*sqN;
  Deviation = D*sqN;
  //printArray(x, n);
  if (!quiet) 
  {
    printf("\n");
    printTable();
    printf("n         = %d\n",n);
    printf("mu        = %f\n",mu);
    printf("sigma     = %f\n",sigma);
    printf("D         = %f\n",D);
    printf("sqN       = %f ,   sqN = sqrt(n) - 0.01 + 0.85/sqrt(n);\n",sqN); 
    //printf("deviation = %f\n",deviation);
    printf("D*sqN     = %f\n",Deviation);
    printf("\n");
  }
  return Deviation;
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

void printArray(double x[], int n)
{
  int index;
  for (index=0; index<n; index++)
  {
     printf("x[%d]=%e\n",index,x[index]);
  }
}

int cmp(const void *x, const void *y)
{
  double xx = *(double*)x, yy = *(double*)y;
  if (xx < yy) return -1;
  if (xx > yy) return  1;
  return 0;
}

void printTable()
{
  printf("=============================================\n");
  printf("alpha   0.15    0.1     0.5     0.25    0.01 \n");
  printf("Dmax    0.775   0.819   0.895   0.995   1.035\n");
  printf("=============================================\n");
}

int main2(int argc, char *argv[])
{
  FILE *fp;
  double *x = NULL;
  int n;
  int index = 0;

  if (argc != 2) {
    fprintf(stderr, "Error: missing input file name\n");
    exit(-1);
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

  stephens(x, n, false);

  free(x);
}



