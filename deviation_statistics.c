#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "kolmogorov.h"
#include "random_generator.h"

int cmp1(const void *x, const void *y)
{
  double xx = *(double*)x, yy = *(double*)y;
  if (xx < yy) return -1;
  if (xx > yy) return  1;
  return 0;
}

void print_array(char* file_name, double* x, int n)
{
    FILE *f;
    int i;
    f = fopen(file_name, "w");
    if (f == NULL)
    {
      printf("Error opening file for write!\n");
      exit(1);
    }
    for (i = 0; i < n; i++)
    {
      fprintf(f, "%f\n", x[i]);
    }
    fclose(f);
}

int main(int argc, char *argv[])
{
  int i, j, m, n, count;
  double x,x0,x1;
  double dx;
  double *rnd;
  double *dev;
  double mu, sigma;
  FILE *f = NULL;
  char *fileprefix;
  char filename[80];
  if (argc != 3) {
     fprintf(stderr, "Error: missing parameters: <file_prefix> <exp_number>\n");
     exit(-1);
   }
  fileprefix = argv[1];
  count = atoi(argv[2]);

  dev = malloc(sizeof(double)*count);

  srand1();
  x0 = 0.5;
  x1 = 0.0;
  dx = 0.001;
  x = x0;
  for (x = x0; x > x1; x-=dx) {
    n = (int)(1/x/x);
    rnd = malloc(sizeof(double)*n);
    m = 0;
    for (i=0; i < count; i++)
    {
      for (j = 0; j < n; j++) rnd[j] = rand_gasdev(); 
      dev[i] = dinstance_normal(rnd, n, true, &mu, &sigma);
      if (((int)(100*(i/(count+0.)))) == m)
      {
         printf("\rx=%e, %d%%", x, m);
         fflush(stdout);
         m++;
      }
    }
    // sort deviations
    qsort(dev, count, sizeof(double), cmp1);
    printf("\r%d, %e            \n", n, dev[(int)(95.0/100.0*count)]);
    fflush(stdout);
    snprintf(filename, sizeof filename, "%s_%d_%d.txt", fileprefix, count, n);
    print_array(filename, dev, count);
    free(rnd);
  }
  printf("\r\r\r\r\r\r\r\rDone!\n");
  free(dev);
  return 0;
}
