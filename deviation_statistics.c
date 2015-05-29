#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "kolmogorov.h"
#include "random_generator.h"
#include <unistd.h>

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

void parse_input(int argc, char *argv[], bool* restore, char** fileprefix, long int* count, int* n_min, int* n_max){
  int c;
  bool mand_f = false, mand_n = false, mand_a = false, mand_b = false;
  extern char *optarg;
  extern int optind, optopt;
//  if (argc != 3) {
//     fprintf(stderr, "Error: missing parameters: <file_prefix> <exp_number>\n");
//     exit(-1);
//  }
  while ( (c = getopt(argc, argv, "rf:n:a:b:")) != -1) {
        int this_option_optind = optind ? optind : 1;
        switch (c) {
        case 'r':
            printf ("option restore\n");
            *restore = true;
            break;
        case 'f':
            printf ("option file_prefix: '%s'\n",optarg);
            *fileprefix = optarg;
            mand_f = true;
            break;
        case 'n':
            printf ("option exp_number: '%s'\n", optarg);
            *count = atoi(optarg);
            mand_n = true;
            break;
        case 'a':
            printf ("option min n: '%s'\n", optarg);
            *n_min = atoi(optarg);
            mand_a = true;
            break;
        case 'b':
            printf ("option max n: '%s'\n", optarg);
            *n_max = atoi(optarg);
            mand_b = true;
            break;
        case '?':
	    exit(-1);
            break;
        default:
            printf ("?? getopt returned character code 0%o ??\n", c);
	    exit(-1);
        }
    }
    if (optind < argc) {
        printf ("non-option ARGV-elements: ");
        while (optind < argc)
            printf ("%s ", argv[optind++]);
        printf ("\n");
	exit(-1);
    }

  if (!*restore && (!mand_f || !mand_n || !mand_a || !mand_b)) {
	printf("Usage: [-r (restore)] -f file_prefix -n exp_number -a min_n -b max_n\n");
	exit(-1);
  }
}

void store(char* fileprefix, long int i, int n, long int count, int n_min, int n_max, double x, int k, long next, double* dev){
 char filename[100];
 FILE *f;
 bool debug = false;
 sprintf(filename,"store.dat");
 f = fopen(filename, "wb");
 //fwrite(fileprefix, sizeof(char), sizeof(fileprefix), f);
 fwrite(&i, sizeof(long int), 1, f);
 if (debug) printf("i=%ld\n",i);
 fwrite(&n, sizeof(int), 1, f);
 if (debug) printf("n=%d\n",n);
 fwrite(&count, sizeof(long int), 1, f);
 if (debug) printf("count=%ld\n",count);
 fwrite(&n_min, sizeof(int), 1, f);
 if (debug) printf("n_min=%d\n",n_min);
 fwrite(&n_max, sizeof(int), 1, f);
 if (debug) printf("n_max=%d\n",n_max);
 fwrite(&x, sizeof(double), 1, f);
 if (debug) printf("x=%e\n",x);
 fwrite(&k, sizeof(int), 1, f);
 if (debug) printf("k=%d\n",k);
 fwrite(&next, sizeof(long), 1, f);
 if (debug) printf("next=%ld\n",next);
 fwrite(dev, sizeof(double), i, f);
 fclose(f);
}

void restore(char* fileprefix, long int* i, int* n, long int* count, int* n_min, int* n_max, double* x, int* k, long* next, double** dev){
 char filename[100];
 FILE *f;
 bool debug = false;
 sprintf(filename,"store.dat");
 f = fopen(filename, "rb");
if (f == NULL) {
	printf("Cannot fimd file: %s\n",filename);
}
 fread(i, sizeof(long int), 1, f);
 if (debug) printf("i=%ld\n",*i);
 fread(n, sizeof(int), 1, f);
 if (debug) printf("n=%d\n",*n);
 fread(count, sizeof(long int), 1, f);
 if (debug) printf("count=%ld\n",*count);
 fread(n_min, sizeof(int), 1, f);
 if (debug) printf("n_min=%d\n",*n_min);
 fread(n_max, sizeof(int), 1, f);
 if (debug) printf("n_max=%d\n",*n_max);
 fread(x, sizeof(double), 1, f);
 if (debug) printf("x=%e\n",*x);
 fread(k, sizeof(int), 1, f);
 if (debug) printf("k=%d\n",*k);
 fread(next, sizeof(long), 1, f);
 if (debug) printf("next=%ld\n",*next);
 *dev = malloc(sizeof(double)*(*count));
 fread(*dev, sizeof(double), *i, f);
 if (debug) printf("dev loaded\n");
 fclose(f);
}

int main(int argc, char *argv[])
{
  int j, k, m, n, n_min, n_max;
  long int i;
  long int count;
  double x,x0,x1;
  double dx;
  double *rnd;
  double *dev = NULL;
  double mu = 0, sigma = 1;
  FILE *f = NULL;
  char *fileprefix;
  char filename[80];
  struct timespec tstart={0,0}, tend={0,0};
  bool restore_flag = false;
  long next;

  x0 = 0.25;
  x1 = 0.0;
  dx = 0.001;

  parse_input(argc, argv, &restore_flag, &fileprefix, &count, &n_min, &n_max);

  if (restore_flag) {
    restore(fileprefix, &i, &n, &count, &n_min, &n_max, &x, &k, &next, &dev);
    set_seed(next);
  } else {
    srand1();
    dev = malloc(sizeof(double)*count);
    k=0;
    x = x0;
  }


  clock_gettime(CLOCK_MONOTONIC, &tstart);

  for (; x > x1; x-=dx) {
	//printf("x=%f\n",x);
    if (n == (int)(1/x/x)) k++;
    else k=0;
    n = (int)(1/x/x);
    //if (n>=10000) continue;
    if (n<n_min || n >= n_max) continue;
    rnd = malloc(sizeof(double)*n);
    for (i=0; i < count; i++)
    {
      for (j = 0; j < n; j++) rnd[j] = rand_gasdev(); 
  //    dev[i] = sqrt(n)*dinstance_normal_know_mu_sigma(rnd, n, false, mu, sigma);
      dev[i] = sqrt(n)*dinstance_normal(rnd, n, false, &mu, &sigma);
      //for (j = 0; j < n; j++) rnd[j] = rand_double(); 
      //dev[i] = sqrt(n)*dinstance_uniform(rnd, n, false);
      if (((100*i)/count)*(count/100) == i)
      {
         fprintf(stdout, "\rx=%e, %ld%%", x, (100*i / count));
         fflush(stdout);
      }
      if (((10*i)/count)*(count/10) == i) {
         printf(", storing...");
         store(fileprefix, i, n, count, n_min, n_max, x, k, next_seed(), dev);
         printf("\rstored                             ");
      }
    }
    // sort deviations
    qsort(dev, count, sizeof(double), cmp1);

    clock_gettime(CLOCK_MONOTONIC, &tend);

    printf("\r%d, %.5f sec         \n", n, 
       ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - 
       ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));
    fflush(stdout);
    snprintf(filename, sizeof filename, "%s_%ld_%d_%d.txt", fileprefix, count, n, k);
    print_array(filename, dev, count);
    free(rnd);
    tstart = tend;
  }
  printf("\r\r\r\r\r\r\r\rDone!\n");
  free(dev);
  return 0;
}
