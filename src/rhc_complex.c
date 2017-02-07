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
