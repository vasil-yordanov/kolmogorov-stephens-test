#ifndef KOLMOGOROV_FILE
#define KOLMOGOROV_FILE

#include <stdbool.h>
#include <stdarg.h>

double dinstance(double (*CDF)(double x, va_list params), double x[], int n, bool verbose, ...);

double dinstance_normal(double x[], int n, bool verbose, double *mu, double *sigma);

#endif
