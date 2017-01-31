#include "rhc_vec.h"
#include "rhc_test.h"
#include <malloc.h>

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

TEST(test_vec_set_elem)
{
  vec_t v;

  v = vec_create( 1 );
  vec_set_elem( v, 0, 1.0 );
  check_vec_elem( v, 1, 1.0 );
  vec_destroy( v );

  v = vec_create( 3 );
  vec_set_elem( v, 0, 2 );
  vec_set_elem( v, 1, 3.0 );
  vec_set_elem( v, 2, 5.5 );
  check_vec_elem( v, 3, 2.0, 3.0, 5.5 );
  vec_destroy( v );
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

TEST(test_vec_add)
{
  vec_t v1, v2, v;

  v1 = vec_create_list( 2, 1.0, 1.0 );
  v2 = vec_create_list( 2, 3.0, 4.0 );
  v  = vec_create( 2 );
  vec_add( v1, v2, v );
  check_vec_elem( v, 2, 4.0, 5.0 );
  vec_destroy( v1 );
  vec_destroy( v2 );
  vec_destroy( v );

  v1 = vec_create_list( 4, -2.0, 3.0, -4.0,  5.0 );
  v2 = vec_create_list( 4, -3.0, 4.0,  5.0, -6.0 );
  v  = vec_create( 4 );
  vec_add( v1, v2, v );
  check_vec_elem( v, 4, -5.0, 7.0, 1.0, -1.0 );
  vec_destroy( v1 );
  vec_destroy( v2 );
  vec_destroy( v );
}

TEST_SUITE(test_vec)
{
  RUN_TEST(test_vec_create);
  RUN_TEST(test_vec_set_elem);
  RUN_TEST(test_vec_set_elem_list);
  RUN_TEST(test_vec_create_list);
  RUN_TEST(test_vec_add);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_vec);
  TEST_REPORT();
  TEST_EXIT();
}
