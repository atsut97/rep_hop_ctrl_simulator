#include "rhc_complex.h"
#include "rhc_test.h"

void check_complex_init(double re, double im)
{
  complex_t c;

  complex_init( &c, re, im );
  ASSERT_EQ( re, c.re );
  ASSERT_EQ( im, c.im );
}

TEST(test_complex_init)
{
  struct case_t {
    double re;
    double im;
    bool end;
  } cases[] = {
    { 0, 0, false },
    { 1, 3, false },
    { -3, 2, false },
    { 1, -2, false },
    { -8, -6, false },
    { 0, 0, true } };
  struct case_t *c;

  for( c=cases; !c->end; c++ )
    check_complex_init( c->re, c->im );
}

void check_complex_polar(double r, double theta, double re, double im)
{
  complex_t c;

  complex_polar( &c, r, theta );
  ASSERT_DOUBLE_EQ( re, c.re );
  ASSERT_DOUBLE_EQ( im, c.im );
}

TEST(test_complex_polar)
{
  struct case_t {
    double r;
    double theta;
    double re;
    double im;
    bool end;
  } cases[] = {
    { 0, 0,    0, 0, false },
    { 1, 0,    1, 0, false },
    { 2, PI_2, 0, 2, false },
    { 1, PI,  -1, 0, false },
    { 0, 0,    0, 0, true } };
  struct case_t *c;

  for( c=cases; !c->end; c++ )
    check_complex_polar( c->r, c->theta, c->re, c->im );
}

void check_complex_sqr_abs(double re, double im, double expected)
{
  complex_t c;

  complex_init( &c, re, im );
  ASSERT_DOUBLE_EQ( expected, complex_sqr_abs( &c ) );
}

TEST(test_complex_sqr_abs)
{
  struct case_t {
    double re, im;
    double expected;
    bool end;
  } cases[] = {
    {  0, 0,    0, false },
    {  1, 1,    2, false },
    { -3, 4,   25, false },
    { -5, 12, 169, false },
    { 0, 0, 0, true },
  };
  struct case_t *c;

  for( c=cases; !c->end; c++ )
    check_complex_sqr_abs( c->re, c->im, c->expected );
}

void check_complex_abs(double re, double im, double expected)
{
  complex_t c;

  complex_init( &c, re, im );
  ASSERT_DOUBLE_EQ( expected, complex_abs( &c ) );
}

TEST(test_complex_abs)
{
  struct case_t {
    double re, im;
    double expected;
    bool end;
  } cases[] = {
    {  0, 0,       0, false },
    {  1, 1, sqrt(2), false },
    { -3, 4,       5, false },
    { -5, 12,     13, false },
    { 0, 0, 0, true },
  };
  struct case_t *c;

  for( c=cases; !c->end; c++ )
    check_complex_abs( c->re, c->im, c->expected );
}

void check_complex_arg(double re, double im, double expected)
{
  complex_t c;

  complex_init( &c, re, im );
  ASSERT_DOUBLE_EQ( expected, complex_arg( &c ) );
}

TEST(test_complex_arg)
{
  struct case_t {
    double re, im;
    double expected;
    bool end;
  } cases[] = {
    {  0, 0, 0, false },
    {  1, 1, PI/4, false },
    {  1, sqrt(3), PI/3, false },
    { -1, 1, 3*PI/4, false },
    { -sqrt(3), -1, -5*PI/6, false },
    { 0, 0, 0, true },
  };
  struct case_t *c;

  for( c=cases; !c->end; c++ )
    check_complex_arg( c->re, c->im, c->expected );
}

void check_complex_conj(complex_t *c1, complex_t *expected)
{
  complex_t c2;

  complex_conj( c1, &c2 );
  ASSERT_DOUBLE_EQ( expected->re, c2.re );
  ASSERT_DOUBLE_EQ( expected->im, c2.im );
}

TEST(test_complex_conj)
{
  struct case_t {
    complex_t c1;
    complex_t expected;
    bool end;
  } cases[] = {
    { { 0, 0 }, { 0, 0 }, false },
    { { 1, 2 }, { 1, -2 }, false },
    { { -3, 2 }, { -3, -2 }, false },
    { { 2, -5 }, { 2, 5 }, false },
    { { -1, -4 }, { -1, 4 }, false },
    { { 0, 0 }, { 0, 0 }, true }
  };
  struct case_t *c;

  for( c=cases; !c->end; c++ )
    check_complex_conj( &c->c1, &c->expected );
}

TEST_SUITE(test_complex)
{
  RUN_TEST(test_complex_init);
  RUN_TEST(test_complex_polar);
  RUN_TEST(test_complex_sqr_abs);
  RUN_TEST(test_complex_abs);
  RUN_TEST(test_complex_arg);
  RUN_TEST(test_complex_conj);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_complex);
  TEST_REPORT();
  TEST_EXIT();
}
