#include "rhc_vec.h"
#include "rhc_test.h"
#include <malloc.h>

/* set random values to a vector */
void set_vec_rand(vec_t v)
{
  register size_t i;

  for( i=0; i<vec_size(v); i++ )
    vec_set_elem( v, i, rand() );
}

/* check if a vector created by 'vec_create' has correct size */
void check_vec_create(size_t n, size_t expected)
{
  vec_t v;

  v = vec_create( n );
  ASSERT_EQ( expected, vec_size( v ) );
  ASSERT_LE( sizeof(double) * expected, malloc_usable_size(v->elem) );
  vec_destroy( v );
}

TEST(test_vec_create)
{
  struct case_t {
    size_t n, expected;
  } cases[] = { {2, 2}, {5, 5}, {0, 0} };
  struct case_t *c;

  for( c=cases; (*c).n != 0; c++ )
    check_vec_create( (*c).n, (*c).expected );
}

TEST(test_vec_create_zero_size)
{
  vec_t v;

  v = vec_create( 0 );
  ASSERT_EQ( 0, vec_size( v ) );
  ASSERT_FALSE( vec_buf( v ) );
  vec_destroy( v );
}

void check_vec_elem(vec_t v, size_t size, ... )
{
  va_list args;
  double val;
  register size_t i;

  va_start( args, size );
  for( i=0; i<size; i++ ){
    val = (double)va_arg( args, double );
    ASSERT_EQ( val, vec_elem( v, i ) );
  }
  va_end( args );
}

void check_vec_set_elem(size_t size, double *elem)
{
  vec_t v;
  register size_t i;

  v = vec_create( size );
  for( i=0; i<size; i++ )
    vec_set_elem( v, i, elem[i] );
  for( i=0; i<size; i++ )
    ASSERT_EQ( elem[i], vec_elem( v, i ) );
  vec_destroy( v );
}

TEST(test_vec_set_elem)
{
  struct case_t {
    size_t n;
    double elem[10];
  } cases[] = {
    { 1, { 1.0 } },
    { 3, { 2.0, 3.0, 5.5 } },
    { 0, {} }
  };
  struct case_t *c;

  for( c=cases; (*c).n>0; c++ )
    check_vec_set_elem( (*c).n, (*c).elem );
}

TEST(test_vec_set_elem_list)
{
  vec_t v;

  v = vec_create( 3 );
  vec_set_elem_list( v, 1.0, 2.0, 3.0 );
  check_vec_elem( v, 3, 1.0, 2.0, 3.0 );
  vec_destroy( v );

  v = vec_create( 6 );
  vec_set_elem_list( v, -3.0, -2.0, -1.0, 1.0, 2.0, 3.0 );
  check_vec_elem( v, 6, -3.0, -2.0, -1.0, 1.0, 2.0, 3.0 );
  vec_destroy( v );
}

void check_vec_clear(size_t n)
{
  vec_t v;
  register size_t i;

  v = vec_create( n );
  set_vec_rand( v );
  vec_clear( v );
  for( i=0; i<vec_size(v); i++ )
    ASSERT_EQ( 0.0, vec_elem(v,i) );
  vec_destroy( v );
}

TEST(test_vec_clear)
{
  int cases[] = { 3, 5, 0 };

  for( int *c=cases; (*c) > 0; c++ )
    check_vec_clear( *c );
}

TEST(test_vec_create_list)
{
  vec_t v;

  v = vec_create_list( 3, 1.0, 2.0, 3.0 );
  ASSERT_EQ( 3, vec_size( v ) );
  check_vec_elem( v, 3, 1.0, 2.0, 3.0 );
  vec_destroy( v );

  v = vec_create_list( 6, 1.0, 2.0, 3.0, -3.0, -2.0, -1.0 );
  ASSERT_EQ( 6, vec_size( v ) );
  check_vec_elem( v, 6, 1.0, 2.0, 3.0, -3.0, -2.0, -1.0 );
  vec_destroy( v );
}

void check_vec_elem_with_array(double *expected, vec_t v)
{
  register size_t i;

  for( i=0; i<vec_size(v); i++ )
    ASSERT_EQ( expected[i], vec_elem( v, i ) );
}

void check_vec_create_array(size_t n, double *elem)
{
  vec_t v;

  v = vec_create_array( n, elem );
  ASSERT_EQ( n, vec_size( v ) );
  check_vec_elem_with_array( elem, v );
  vec_destroy( v );
}

TEST(test_vec_create_array)
{
  struct case_t {
    size_t n;
    double elem[10];
  } cases[] = {
    { 3, { 1.0, 2.0, 3.0 } },
    { 6, { 1.0, 2.0, 3.0, -3.0, -2.0, -1.0 } },
    { 0, {} }
  };
  struct case_t *c;

  for( c=cases; (*c).n > 0; c++ )
    check_vec_create_array( (*c).n, (*c).elem );
}

void check_vec_op(size_t n, double *val1, double *val2, double *expected, vec_t (*method)(vec_t,vec_t,vec_t))
{
  vec_t v1, v2, v;

  v1 = vec_create_array( n, val1 );
  v2 = vec_create_array( n, val2 );
  v  = vec_create( n );

  method( v1, v2, v );

  check_vec_elem_with_array( expected, v );
  vec_destroy( v1 );
  vec_destroy( v2 );
  vec_destroy( v );
}

TEST(test_vec_add)
{
  struct case_t {
    size_t n;
    double val1[10];
    double val2[10];
    double expected[10];
  } cases[] = {
    { 2, { 1.0, 1.0 }, { 3.0, 4.0 }, { 4.0, 5.0 } },
    { 4, { -2.0, 3.0, -4.0, 5.0 }, { -3.0, 4.0, 5.0, -6.0 }, { -5.0, 7.0, 1.0, -1.0 } },
    { 0, {}, {}, {} },
  };
  struct case_t *c;

  for( c=cases; (*c).n > 0; c++ )
    check_vec_op( (*c).n, (*c).val1, (*c).val2, (*c).expected, vec_add );
}

void check_vec_size_3(size_t v1_s, size_t v2_s, size_t v3_s, bool expected, vec_t (*method)(vec_t,vec_t,vec_t))
{
  vec_t v1, v2, v3, ret;
  char msg[BUFSIZ];
  bool cond;

  v1 = vec_create( v1_s ); vec_clear( v1 );
  v2 = vec_create( v2_s ); vec_clear( v2 );
  v3 = vec_create( v3_s ); vec_clear( v3 );
  ret = method( v1, v2, v3 );
  cond = ( vec_size(v1) == vec_size(v2) ) && ( vec_size(v2) == vec_size(v3) );
  if( expected ){
    sprintf( msg, "Expected all sizes are equal, but was %lu vs %lu vs %lu",
             vec_size(v1), vec_size(v2), vec_size(v3) );
    ASSERT( cond, msg );
    ASSERT_PTREQ( vec_buf(v3), vec_buf(ret) );
  } else {
    sprintf( msg, "Expected some sizes are not equal, but was %lu vs %lu vs %lu",
             vec_size(v1), vec_size(v2), vec_size(v3) );
    ASSERT( !cond, msg );
    ASSERT_STREQ( VEC_ERR_SIZMIS, __err_last_msg );
    ASSERT_PTREQ( NULL, ret );
    RESET_ERR_MSG();
  }
}

TEST(test_vec_add_size_mismatch)
{
  struct case_t {
    size_t v1_s, v2_s, v3_s;
    bool expected;
  } cases[] = {
    { 2, 2, 2, true },
    { 2, 3, 3, false },
    { 3, 3, 1, false },
    { 2, 3, 1, false },
    { 0, 0, 0, false }
  };
  struct case_t *c;

  ECHO_OFF();
  for( c=cases; (*c).v1_s>0; c++ )
    check_vec_size_3( (*c).v1_s, (*c).v2_s, (*c).v3_s,
                      (*c).expected, vec_add );
  ECHO_ON();
}

TEST(test_vec_sub)
{
  struct case_t {
    size_t n;
    double val1[10];
    double val2[10];
    double expected[10];
  } cases[] = {
    { 2, { 1.0, 1.0 }, { 3.0, 4.0 }, { -2.0, -3.0 } },
    { 4, { -2.0, 3.0, -4.0, 5.0 }, { -3.0, 4.0, 5.0, -6.0 }, { 1.0, -1.0, -9.0, 11.0 } },
    { 0, {}, {}, {} },
  };
  struct case_t *c;

  for( c=cases; (*c).n > 0; c++ )
    check_vec_op( (*c).n, (*c).val1, (*c).val2, (*c).expected, vec_sub );
}

TEST_SUITE(test_vec)
{
  RUN_TEST(test_vec_create);
  RUN_TEST(test_vec_create_zero_size);
  RUN_TEST(test_vec_set_elem);
  RUN_TEST(test_vec_set_elem_list);
  RUN_TEST(test_vec_create_list);
  RUN_TEST(test_vec_create_array);
  RUN_TEST(test_vec_clear);
  RUN_TEST(test_vec_add);
  RUN_TEST(test_vec_add_size_mismatch);
  RUN_TEST(test_vec_sub);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_vec);
  TEST_REPORT();
  TEST_EXIT();
}
