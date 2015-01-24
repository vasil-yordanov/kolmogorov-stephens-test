/* Example of matrix solution using Numerical Recipes routines.
 * See also the "x*.c" example files in Numerical_Recipes/C/Demos.
 */

/* This file is downloaded from http://www.physics.drexel.edu/students/courses/physics-501/NR-examples-1/gaussj_demo.c */

#include <stdio.h>
#include <math.h>
#include "gaussj.h"
#include "nrutil.h"

#define N 5	/* dimension of the system */

void main_all_you_really_need()
{
    /* Solve the linear system Ax = b by Gauss-Jordan elimination. */

    int i, j;

    /* NR declarations of matrices: */

    double **a  = dmatrix(1, N, 1, N);
    double **b  = dmatrix(1, N, 1, 1);		/* b is really a vector, but
						   gaussj expects a matrix */

    /* Set up A and b.  Note the NR convention that indices start at 1. */

    for (i = 1; i <= N; i++)			/* i = row    */
	for (j = 1; j <= N; j++)		/* j = column */
	    a[i][j] = 1.0 / (i+j);

    for (i = 1; i <= N; i++)
	b[i][1] = 1;

    /* Solve by Gauss-Jordan.  On return, A is replaced by its inverse
     * and b is replaced by the solution matrix. */

    gaussj(a, N, b, 1);
}

main()
{
    /* Solve the linear system Ax = b by Gauss-Jordan elimination. */

    int i, j, k;

    /* NR declarations of matrices: */

    double **a  = dmatrix(1, N, 1, N);
    double **aa = dmatrix(1, N, 1, N);
    double **p  = dmatrix(1, N, 1, N);
    double **b  = dmatrix(1, N, 1, 1);		/* b is really a vector, but
						   gaussj expects a matrix */
    double *x   = dvector(1, N);

    /* Set up A and b.  Note the NR convention that indices start at 1. */

    for (i = 1; i <= N; i++)			/* i = row    */
	for (j = 1; j <= N; j++) {		/* j = column */

	    a[i][j] = 1.0 / (i+j);

	    /* Save a copy of a, for use below. */

	    aa[i][j] = a[i][j];
	}

    for (i = 1; i <= N; i++)
	b[i][1] = 1;

    /* Print out the initial A and b. */

    printf("initial A and b:\n");

    for (i = 1; i <= N; i++) {
	printf("    ");
	for (j = 1; j <= N; j++) printf("%9.2e  ", a[i][j]);
	printf("    %9.2e\n", b[i][1]);
    }

    /* Solve by Gauss-Jordan.  On return, A is replaced by its inverse
     * and b is replaced by the solution matrix. */

    gaussj(a, N, b, 1);

    /* Print out the final A and b. */

    printf("final A(inv) and b' (sol):\n");

    for (i = 1; i <= N; i++) {
	printf("    ");
	for (j = 1; j <= N; j++) printf("%9.2e  ", a[i][j]);
	printf("    %9.2e\n", b[i][1]);
    }

    /* Check that a and aa are now inverses and that we have
     * actually solved the original equation. */

    for (i = 1; i <= N; i++)
	for (j = 1; j <= N; j++) {
	    p[i][j] = 0;
	    for (k = 1; k <= N; k++)
		p[i][j] += a[i][k]*aa[k][j];
	}

    for (i = 1; i <= N; i++) {
	x[i] = 0;
	for (j = 1; j <= N; j++)
	    x[i] += aa[i][j]*b[j][1];
    }

    /* Print out the product matrices P and x. */

    printf("products A * Ainv, A * b':\n");

    for (i = 1; i <= N; i++) {
	printf("    ");
	for (j = 1; j <= N; j++) printf("%9.5f  ", p[i][j]);
	printf("    %9.2e\n", x[i]);
    }
}
