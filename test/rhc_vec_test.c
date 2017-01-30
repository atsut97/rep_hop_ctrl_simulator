#include "rhc_test.h"
#include "rhc_vec.h"
#include <malloc.h>

void _test_vec_create(uint size)
{
  rhcVec v;

  v = rhcVecCreate( size );
  RHC_ASSERT_EQ( size, rhcVecSize( v ) );
  RHC_ASSERT_LE( sizeof(double) * size, malloc_usable_size(v->elem) );
  rhcVecDestroy( v );
}

RHC_TEST(test_vec_create)
{
  _test_vec_create( 2 );
  _test_vec_create( 5 );
}

RHC_TEST_SUITE(test_vec)
{
  RHC_RUN_TEST(test_vec_create);
}

int main(int argc, char *argv[])
{
  RHC_RUN_SUITE(test_vec);
  RHC_TEST_REPORT();
  RHC_TEST_EXIT();
}
