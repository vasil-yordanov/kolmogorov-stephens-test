#ifndef KOLMOGOROV_FILE
#define KOLMOGOROV_FILE

#include <stdbool.h>
#include <stdarg.h>

double dinstance(double (*CDF)(double x, va_list params), double x[], int n, bool verbose, ...);

double dinstance_normal(double x[], int n, bool verbose, double *mu, double *sigma);

double dinstance_normal_know_mu_sigma(double x[], int n, bool verbose, double mu, double sigma);

double dinstance_uniform(double x[], int n, bool verbose);

#endif
