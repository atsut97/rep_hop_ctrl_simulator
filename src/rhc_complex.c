#include "rhc_complex.h"

complex_t *complex_init(complex_t *c, double re, double im)
{
  c->re = re;
  c->im = im;
  return c;
}

complex_t *complex_polar(complex_t *c, double r, double theta)
{
  return complex_init( c, r*cos(theta), r*sin(theta) );
}

double complex_sqr_abs(complex_t *c)
{
  return sqr(c->re) + sqr(c->im);
}

double complex_arg(complex_t *c)
{
  return atan2( c->im, c->re );
}

complex_t *complex_conj(complex_t *c, complex_t *cc)
{
  return complex_init( cc, c->re, -c->im );
}
