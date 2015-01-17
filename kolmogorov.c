#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdarg.h>

int cmp(const void *x, const void *y);

double dinstance(double (*CDF)(double x, va_list params), double x[], int n, bool verbose, ...)
{
  int i, m;
  double D_plus, D_minus, D_plus_max, D_minus_max, D;
  double CDF_val; 
  va_list argp;

  /* sort the array */
  if (verbose) printf("Sorting array...\n");
  qsort(x, n, sizeof(double), cmp);
  if (verbose) printf("Done!\n");

  // calculate D_max
  D_plus_max = 0;
  D_minus_max = 0;

  m = 0;
  if (verbose) printf("Calculating D...\n");
  for (i=0; i<n; i++)
  {
    va_start(argp, verbose);
    CDF_val = CDF(x[i],argp);
    va_end(argp);

    D_plus = ((i+1)/((double)n) - CDF_val); 
    D_minus = (CDF_val - i/((double)n));
    if (D_plus_max<D_plus) D_plus_max = D_plus;
    if (D_minus_max<D_minus) D_minus_max = D_minus;

    // print calculation progress
    if (verbose) 
    if (((int)(100*(i/(n+0.)))) == m)
    {
      printf ("\r\r\r\r%d%%",m);
      fflush(stdout);
      m++;
    }
  }

  // print calculation progress
  if (verbose) printf("\r\r\r\rDone!\n");
  if (D_plus_max > D_minus_max) D = D_plus_max;
  else D = D_minus_max;

  return D;
}

int cmp(const void *x, const void *y)
{
  double xx = *(double*)x, yy = *(double*)y;
  if (xx < yy) return -1;
  if (xx > yy) return  1;
  return 0;
}


/*  Specific distribution - Normal Distribution */

double normal_CDF(double x, va_list argp)
{
  double mu;
  double sigma;
  mu = va_arg(argp, double);
  sigma = va_arg(argp, double);

  return 0.5*(1+erf((x-mu)/(sqrt(2)*sigma)));
}

double avg(double x[], int n, bool verbose)
{
  double mu;
  int m,i;
  // Calculate average value
  mu = 0.;
  m = 0;
  if (verbose) printf("Calculating mu...\n");
  for (i=0; i<n; i++)
  {
    mu += x[i]; 
    if (verbose) 
    if (((int)(100*(i/(n+0.)))) == m)
    {
      printf ("\r\r\r\r%d%%",m);
      fflush(stdout);
      m++;
    }
  }
  mu /= n+0.;
  if (verbose) printf("\r\r\r\rDone!\n");
  return mu;
}

double std(double x[], int n, double mu, bool verbose)
{
  double sigma;
  int m,i;
  // Calculate standart deviation
  sigma = 0.;
  m = 0;
  if (verbose) printf("Calculating sigma...\n");
  for (i=0; i<n; i++)
  {
    sigma += (x[i]-mu)*(x[i]-mu); 
    if (verbose) 
    if (((int)(100*(i/(n+0.)))) == m)
    {
      printf ("\r\r\r\r%d%%",m);
      fflush(stdout);
      m++;
    }
  }
  sigma = sqrt(sigma/(n-1.)); 
  if (verbose) printf("\r\r\r\rDone!\n");
  return sigma;
}

double dinstance_normal_know_mu_sigma(double x[], int n, bool verbose, double mu, double sigma)
{
  return dinstance(normal_CDF, x, n, verbose, mu, sigma);
}

double dinstance_normal(double x[], int n, bool verbose, double *mu, double *sigma)
{
  *mu = avg(x,n,false);
  *sigma = std(x,n,*mu,false);
  return dinstance(normal_CDF, x, n, verbose, *mu, *sigma);
}

/*  Specific distribution - Uniform Distribution */

double uniform_CDF(double x, va_list argp)
{
  return x;
}

double dinstance_uniform(double x[], int n, bool verbose)
{
  return dinstance(uniform_CDF, x, n, verbose);
}
