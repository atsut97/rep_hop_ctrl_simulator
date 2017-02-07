#include "rhc_complex.h"
#include "rhc_test.h"

void check_eq_complex(complex_t *expected, complex_t *actual)
{
  ASSERT_DOUBLE_EQ( expected->re, actual->re );
  ASSERT_DOUBLE_EQ( expected->im, actual->im );
}

void check_method_dd_c(double arg1, double arg2, complex_t *expected, complex_t* (*method)(complex_t*,double,double))
{
  complex_t actual;

  method( &actual, arg1, arg2 );
  check_eq_complex( expected, &actual );
}

void check_method_c_c(complex_t *arg, complex_t *expected, complex_t* (*method)(complex_t*,complex_t*))
{
  complex_t actual;

  method( arg, &actual );
  check_eq_complex( expected, &actual );
}

void check_method_c_d(complex_t *arg, double expected, double (*method)(complex_t*))
{
  double actual;

  actual = method( arg );
  ASSERT_DOUBLE_EQ( expected, actual );
}

void check_method_cc_c(complex_t *arg1, complex_t *arg2, complex_t *expected, complex_t* (*method)(complex_t*,complex_t*,complex_t*))
{
  complex_t actual;

  method( arg1, arg2, &actual );
  check_eq_complex( expected, &actual );
}

TEST(test_complex_init)
{
  struct case_t {
    double re;
    double im;
    complex_t expected;
    bool end;
  } cases[] = {
    { 0, 0, { 0, 0 }, false },
    { 1, 3, { 1, 3 }, false },
    { -3, 2, { -3, 2 }, false },
    { 1, -2, { 1, -2 }, false },
    { -8, -6, { -8, -6 }, false },
    { 0, 0, {}, true } };
  struct case_t *c;

  for( c=cases; !c->end; c++ )
    check_method_dd_c( c->re, c->im, &c->expected, complex_init );
}

TEST(test_complex_polar)
{
  struct case_t {
    double r;
    double theta;
    complex_t expected;
    bool end;
  } cases[] = {
    { 0, 0,    { 0, 0 }, false },
    { 1, 0,    { 1, 0 }, false },
    { 2, PI_2, { 0, 2 }, false },
    { 1, PI,   {-1, 0 }, false },
    { 0, 0,    { 0, 0 }, true } };
  struct case_t *c;

  for( c=cases; !c->end; c++ )
    check_method_dd_c( c->r, c->theta, &c->expected, complex_polar);
}

TEST(test_complex_clear)
{
  complex_t c;

  complex_init( &c, 1, 2 );
  complex_clear( &c );
  ASSERT_DOUBLE_EQ( 0, c.re );
  ASSERT_DOUBLE_EQ( 0, c.im );
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
    complex_t arg;
    double expected;
    bool end;
  } cases[] = {
    { { 0, 0 },    0, false },
    { { 1, 1 },    2, false },
    { {-3, 4 },   25, false },
    { {-5, 12 }, 169, false },
    { { 0, 0 },    0, true },
  };
  struct case_t *c;

  for( c=cases; !c->end; c++ )
    check_method_c_d( &c->arg, c->expected, complex_sqr_abs );
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
    complex_t arg;
    double expected;
    bool end;
  } cases[] = {
    { { 0, 0 },       0, false },
    { { 1, 1 }, sqrt(2), false },
    { {-3, 4 },       5, false },
    { {-5, 12 },     13, false },
    { { 0, 0 },       0, true },
  };
  struct case_t *c;

  for( c=cases; !c->end; c++ )
    check_complex_abs( c->arg.re, c->arg.im, c->expected );
}

TEST(test_complex_arg)
{
  struct case_t {
    complex_t arg;
    double expected;
    bool end;
  } cases[] = {
    { { 0, 0 }, 0, false },
    { { 1, 1 }, PI/4, false },
    { { 1, sqrt(3) }, PI/3, false },
    { {-1, 1 }, 3*PI/4, false },
    { {-sqrt(3), -1 }, -5*PI/6, false },
    { {0, 0 }, 0, true },
  };
  struct case_t *c;

  for( c=cases; !c->end; c++ )
    check_method_c_d( &c->arg, c->expected, complex_arg );
}

TEST(test_complex_conj)
{
  struct case_t {
    complex_t arg;
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
    check_method_c_c( &c->arg, &c->expected, complex_conj );
}

TEST_SUITE(test_complex)
{
  RUN_TEST(test_complex_init);
  RUN_TEST(test_complex_polar);
  RUN_TEST(test_complex_clear);
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
