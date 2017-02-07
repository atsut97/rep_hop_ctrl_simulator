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

complex_t *complex_clear(complex_t *c)
{
  return complex_init( c, 0, 0 );
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

complex_t *complex_add(complex_t *c1, complex_t *c2, complex_t *c)
{
  return complex_init( c, c1->re + c2->re, c1->im + c2->im );
}

complex_t *complex_sub(complex_t *c1, complex_t *c2, complex_t *c)
{
  return complex_init( c, c1->re - c2->re, c1->im - c2->im );
}

complex_t *complex_mul(complex_t *c1, double k, complex_t *c)
{
  return complex_init( c, c1->re * k, c1->im * k );
}
