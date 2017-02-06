#include "rhc_vec_list.h"
#include "rhc_test.h"

TEST(test_vec_list_node_init)
{
  vec_list_node_t node;

  vec_list_node_init( &node );
  ASSERT_PTREQ( &node, vec_list_node_next(&node) );
  ASSERT_PTREQ( NULL, vec_list_node_data(&node) );
  vec_list_node_destroy( &node );
}

TEST(test_vec_list_node_destroy)
{
  vec_list_node_t node;
  vec_t v;

  vec_list_node_init( &node );
  v = vec_create( 2 );
  /* ret = vec_list_node_set_data( &node, v ); */
  vec_list_node_destroy( &node );
  ASSERT_PTREQ( NULL, vec_list_node_next(&node) );
  ASSERT_PTREQ( NULL, vec_list_node_data(&node) );
}

TEST(test_vec_list_init)
{
  vec_list_t vl;

  vec_list_init( &vl );
  ASSERT_EQ( 0, vec_list_num( &vl ) );
  ASSERT_PTREQ( vec_list_root( &vl ),
                vec_list_node_next( vec_list_root(&vl) ) );
  vec_list_destroy( &vl );
}

TEST_SUITE(test_vec_list)
{
  RUN_TEST(test_vec_list_node_init);
  RUN_TEST(test_vec_list_node_destroy);
  RUN_TEST(test_vec_list_init);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_vec_list);
  TEST_REPORT();
  TEST_EXIT();
}
