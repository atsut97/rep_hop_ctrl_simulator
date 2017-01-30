#include "rhc_vec.h"
#include "rhc_test.h"
#include <malloc.h>

void check_if_vec_create(uint size)
{
  rhcVec v;

  v = rhcVecCreate( size );
  RHC_ASSERT_EQ( size, rhcVecSize( v ) );
  RHC_ASSERT_LE( sizeof(double) * size, malloc_usable_size(v->elem) );
  rhcVecDestroy( v );
}

RHC_TEST(test_vec_create)
{
  check_if_vec_create( 2 );
  check_if_vec_create( 5 );
}

void check_if_vec_set_elem(rhcVec v, uint size, ... )
{
  va_list args;
  double val;
  register uint i;

  va_start( args, size );
  for( i=0; i<size; i++ ){
    val = (double)va_arg( args, double );
    RHC_ASSERT_EQ( val, rhcVecElem( v, i ) );
  }
  va_end( args );
}

RHC_TEST(test_vec_set_elem)
{
  rhcVec v;

  v = rhcVecCreate( 1 );
  rhcVecSetElem( v, 0, 1.0 );
  check_if_vec_set_elem( v, 1, 1.0 );
  rhcVecDestroy( v );

  v = rhcVecCreate( 3 );
  rhcVecSetElem( v, 0, 2 );
  rhcVecSetElem( v, 1, 3.0 );
  rhcVecSetElem( v, 2, 5.5 );
  check_if_vec_set_elem( v, 3, 2.0, 3.0, 5.5 );
  rhcVecDestroy( v );
}

RHC_TEST_SUITE(test_vec)
{
  RHC_RUN_TEST(test_vec_create);
  RHC_RUN_TEST(test_vec_set_elem);
}

int main(int argc, char *argv[])
{
  RHC_RUN_SUITE(test_vec);
  RHC_TEST_REPORT();
  RHC_TEST_EXIT();
}
