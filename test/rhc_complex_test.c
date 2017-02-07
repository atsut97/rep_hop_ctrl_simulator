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

TEST_SUITE(test_complex)
{
  RUN_TEST(test_complex_init);
  RUN_TEST(test_complex_polar);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_complex);
  TEST_REPORT();
  TEST_EXIT();
}
