#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "random_generator.h"

void gauss(int argc, char *argv[]);
void pseudo_gauss(int argc, char *argv[]);

int main(int argc, char *argv[])
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

