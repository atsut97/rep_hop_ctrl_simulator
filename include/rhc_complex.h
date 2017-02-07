#ifndef __RHC_COMPLEX_H__
#define __RHC_COMPLEX_H__

#include "rhc_misc.h"

typedef struct{
  double re;
  double im;
} complex_t;

complex_t *complex_init(complex_t *c, double re, double im);
complex_t *complex_polar(complex_t *c, double r, double theta);

#endif /* __RHC_COMPLEX_H__ */
