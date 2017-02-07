#ifndef __RHC_COMPLEX_H__
#define __RHC_COMPLEX_H__

#include "rhc_misc.h"

typedef struct{
  double re;
  double im;
} complex_t;

complex_t *complex_init(complex_t *c, double re, double im);
complex_t *complex_polar(complex_t *c, double r, double theta);
complex_t *complex_clear(complex_t *c);

double complex_sqr_abs(complex_t *c);
#define complex_abs(c) sqrt( complex_sqr_abs(c) )
double complex_arg(complex_t *c);
complex_t *complex_conj(complex_t *c, complex_t *cc);

#endif /* __RHC_COMPLEX_H__ */
