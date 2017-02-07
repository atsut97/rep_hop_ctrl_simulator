#include "rhc_vec_list.h"
#include "rhc_test.h"

vec_list_node_t node1, node2, node3;

void vec_list_node_setup()
{
  vec_list_node_init( &node1 );
  vec_list_node_init( &node2 );
  vec_list_node_init( &node3 );
}

void vec_list_node_teardown()
{
  vec_list_node_destroy( &node3 );
  vec_list_node_destroy( &node2 );
  vec_list_node_destroy( &node1 );
}

TEST(test_vec_list_node_init)
{
  ASSERT_PTREQ( &node1, vec_list_node_next(&node1) );
  ASSERT_PTREQ( NULL, vec_list_node_data(&node1) );
}

TEST(test_vec_list_node_insert_next)
{
  vec_list_node_insert_next( &node1, &node2 );
  ASSERT_PTREQ( &node2, vec_list_node_next( &node1 ) );
  ASSERT_PTREQ( &node1, vec_list_node_next( &node2 ) );
}

TEST(test_vec_list_node_insert_next_append)
{
  vec_list_node_insert_next( &node1, &node2 );
  vec_list_node_insert_next( &node2, &node3 );
  ASSERT_PTREQ( &node2, vec_list_node_next( &node1 ) );
  ASSERT_PTREQ( &node3, vec_list_node_next( &node2 ) );
  ASSERT_PTREQ( &node1, vec_list_node_next( &node3 ) );
}

TEST(test_vec_list_node_insert_next_insert)
{
  vec_list_node_insert_next( &node1, &node2 );
  vec_list_node_insert_next( &node1, &node3 );
  ASSERT_PTREQ( &node3, vec_list_node_next( &node1 ) );
  ASSERT_PTREQ( &node2, vec_list_node_next( &node3 ) );
  ASSERT_PTREQ( &node1, vec_list_node_next( &node2 ) );
}

TEST(test_vec_list_node_delete_next)
{
  vec_list_node_t *ret;

  vec_list_node_insert_next( &node1, &node2 );
  ret = vec_list_node_delete_next( &node1 );
  ASSERT_PTREQ( &node1, vec_list_node_next( &node1 ) );
  ASSERT_PTREQ( &node2, vec_list_node_next( &node2 ) );
  ASSERT_PTREQ( &node2, ret );
}

TEST(test_vec_list_node_delete_next_last)
{
  vec_list_node_t *ret;

  vec_list_node_insert_next( &node1, &node2 );
  vec_list_node_insert_next( &node2, &node3 );
  ret = vec_list_node_delete_next( &node2 );
  ASSERT_PTREQ( &node2, vec_list_node_next( &node1 ) );
  ASSERT_PTREQ( &node1, vec_list_node_next( &node2 ) );
  ASSERT_PTREQ( &node3, vec_list_node_next( &node3 ) );
  ASSERT_PTREQ( &node3, ret );
}

TEST(test_vec_list_node_delete_next_between)
{
  vec_list_node_t *ret;

  vec_list_node_insert_next( &node1, &node2 );
  vec_list_node_insert_next( &node2, &node3 );
  ret = vec_list_node_delete_next( &node1 );
  ASSERT_PTREQ( &node3, vec_list_node_next( &node1 ) );
  ASSERT_PTREQ( &node1, vec_list_node_next( &node3 ) );
  ASSERT_PTREQ( &node2, vec_list_node_next( &node2 ) );
  ASSERT_PTREQ( &node2, ret );
}

TEST(test_vec_list_node_set_data)
{
  vec_t v, ret;

  v = vec_create( 2 );
  ret = vec_list_node_set_data( &node1, v );
  ASSERT_PTREQ( v, vec_list_node_data(&node1) );
  ASSERT_PTREQ( v, ret );
}

TEST(test_vec_list_node_destroy)
{
  vec_t v;

  vec_list_node_init( &node1 );
  v = vec_create( 2 );
  vec_list_node_set_data( &node1, v );
  vec_list_node_destroy( &node1 );
  ASSERT_PTREQ( NULL, vec_list_node_next(&node1) );
  ASSERT_PTREQ( NULL, vec_list_node_data(&node1) );
}

TEST(test_vec_list_node_destroy_null)
{
  vec_t v;

  vec_list_node_init( &node1 );
  v = NULL;
  vec_list_node_set_data( &node1, v );
  vec_list_node_destroy( &node1 );
  ASSERT_PTREQ( NULL, vec_list_node_next(&node1) );
  ASSERT_PTREQ( NULL, vec_list_node_data(&node1) );
}


vec_list_t vl;

void vec_list_setup()
{
  vec_list_node_setup();
  vec_list_init( &vl );
}

void vec_list_teardown()
{
  vec_list_destroy( &vl );
}

TEST(test_vec_list_init)
{
  ASSERT_EQ( 0, vec_list_num( &vl ) );
  ASSERT_PTREQ( vec_list_root( &vl ),
                vec_list_node_next( vec_list_root(&vl) ) );
}

TEST(test_vec_list_inc)
{
  ASSERT_EQ( 0, vec_list_num( &vl ) );
  vec_list_inc( &vl );
  ASSERT_EQ( 1, vec_list_num( &vl ) );
  vec_list_inc( &vl );
  ASSERT_EQ( 2, vec_list_num( &vl ) );
  vec_list_inc( &vl );
  vec_list_inc( &vl );
  vec_list_inc( &vl );
  ASSERT_EQ( 5, vec_list_num( &vl ) );
  vec_list_num( &vl ) = 0;
}

TEST(test_vec_list_insert_tail)
{
  vec_list_insert_tail( &vl, &node1 );
  ASSERT_EQ( 1, vec_list_num( &vl ) );
  ASSERT_PTREQ( &node1, vec_list_node_next( vec_list_root(&vl) ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_next( &node1 ) );

  vec_list_insert_tail( &vl, &node2 );
  ASSERT_EQ( 2, vec_list_num( &vl ) );
  ASSERT_PTREQ( &node2, vec_list_node_next( vec_list_root(&vl) ) );
  ASSERT_PTREQ( &node1, vec_list_node_next( &node2 ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_next( &node1 ) );
}

TEST(test_vec_list_insert_next)
{
  vec_list_insert_next( &vl, vec_list_root(&vl), &node1 );
  ASSERT_EQ( 1, vec_list_num( &vl ) );
  ASSERT_PTREQ( &node1, vec_list_node_next( vec_list_root(&vl) ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_next( &node1 ) );

  vec_list_insert_next( &vl, &node1, &node2 );
  ASSERT_EQ( 2, vec_list_num( &vl ) );
  ASSERT_PTREQ( &node1, vec_list_node_next( vec_list_root(&vl) ) );
  ASSERT_PTREQ( &node2, vec_list_node_next( &node1 ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_next( &node2 ) );
}

TEST(test_vec_list_insert_next_append)
{
  vec_list_insert_tail( &vl, &node1 );
  vec_list_insert_next( &vl, &node1, &node2 );
  vec_list_insert_next( &vl, &node2, &node3 );
  ASSERT_EQ( 3, vec_list_num(&vl) );
  ASSERT_PTREQ( &node1, vec_list_tail( &vl ) );
  ASSERT_PTREQ( &node2, vec_list_node_next( &node1 ) );
  ASSERT_PTREQ( &node3, vec_list_node_next( &node2 ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_next( &node3 ) );
}

TEST(test_vec_list_insert_next_insert)
{
  vec_list_insert_tail( &vl, &node1 );
  vec_list_insert_next( &vl, &node1, &node2 );
  vec_list_insert_next( &vl, &node1, &node3 );
  ASSERT_EQ( 3, vec_list_num(&vl) );
  ASSERT_PTREQ( &node1, vec_list_tail( &vl ) );
  ASSERT_PTREQ( &node3, vec_list_node_next( &node1 ) );
  ASSERT_PTREQ( &node2, vec_list_node_next( &node3 ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_next( &node2 ) );
}

TEST(test_vec_list_dec)
{
  vl.num = 5;
  ASSERT_EQ( 5, vec_list_num( &vl ) );
  vec_list_dec( &vl );
  ASSERT_EQ( 4, vec_list_num( &vl ) );
  vec_list_dec( &vl );
  ASSERT_EQ( 3, vec_list_num( &vl ) );
  vec_list_dec( &vl );
  vec_list_dec( &vl );
  vec_list_dec( &vl );
  ASSERT_EQ( 0, vec_list_num( &vl ) );
}

TEST(test_vec_list_delete_tail)
{
  vec_list_node_t *ret;

  vec_list_insert_tail( &vl, &node1 );
  vec_list_insert_tail( &vl, &node2 );
  ret = vec_list_delete_tail( &vl );
  ASSERT_EQ( 1, vec_list_num( &vl ) );
  ASSERT_PTREQ( &node1, vec_list_node_next( vec_list_root(&vl) ) );
  ASSERT_PTREQ( vec_list_node_next( vec_list_root(&vl) ), &node1 );
  ASSERT_PTREQ( &node2, vec_list_node_next( &node2 ) );
  ASSERT_PTREQ( &node2, ret );
}

TEST(test_vec_list_delete_tail2)
{
  vec_list_node_t *ret;

  vec_list_insert_tail( &vl, &node1 );
  vec_list_insert_tail( &vl, &node2 );
  vec_list_insert_tail( &vl, &node3 );

  ret = vec_list_delete_tail( &vl );
  ASSERT_EQ( 2, vec_list_num( &vl ) );
  ASSERT_PTREQ( &node2, vec_list_node_next( vec_list_root(&vl) ) );
  ASSERT_PTREQ( &node1, vec_list_node_next( &node2 ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_next( &node1 ) );
  ASSERT_PTREQ( &node3, vec_list_node_next( &node3 ) );
  ASSERT_PTREQ( &node3, ret );

  ret = vec_list_delete_tail( &vl );
  ASSERT_EQ( 1, vec_list_num( &vl ) );
  ASSERT_PTREQ( &node1, vec_list_node_next( vec_list_root(&vl) ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_next( &node1 ) );
  ASSERT_PTREQ( &node2, vec_list_node_next( &node2 ) );
  ASSERT_PTREQ( &node2, ret );

  ret = vec_list_delete_tail( &vl );
  ASSERT_EQ( 0, vec_list_num( &vl ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_next( vec_list_root(&vl) ) );
  ASSERT_PTREQ( &node1, vec_list_node_next( &node1 ) );
  ASSERT_PTREQ( &node1, ret );
}

TEST(test_vec_list_delete_next)
{
  vec_list_node_t *ret;

  vec_list_insert_next( &vl, vec_list_root(&vl), &node1 );
  vec_list_insert_next( &vl, &node1, &node2 );
  ret = vec_list_delete_next( &vl, &node1 );
  ASSERT_EQ( 1, vec_list_num( &vl ) );
  ASSERT_PTREQ( &node1, vec_list_node_next( vec_list_root(&vl) ) );
  ASSERT_PTREQ( vec_list_node_next( vec_list_root(&vl) ), &node1 );
  ASSERT_PTREQ( &node2, vec_list_node_next( &node2 ) );
  ASSERT_PTREQ( &node2, ret );
}

TEST(test_vec_list_delete_next_last)
{
  vec_list_node_t *ret;

  vec_list_insert_tail( &vl, &node1 );
  vec_list_insert_next( &vl, &node1, &node2 );
  vec_list_insert_next( &vl, &node2, &node3 );
  ret = vec_list_delete_next( &vl, &node2 );
  ASSERT_EQ( 2, vec_list_num( &vl ) );
  ASSERT_PTREQ( &node1, vec_list_node_next( vec_list_root(&vl) ) );
  ASSERT_PTREQ( &node2, vec_list_node_next( &node1 ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_next( &node2 ) );
  ASSERT_PTREQ( &node3, vec_list_node_next( &node3 ) );
  ASSERT_PTREQ( &node3, ret );
}

TEST(test_vec_list_delete_next_between)
{
  vec_list_node_t *ret;

  vec_list_insert_tail( &vl, &node1 );
  vec_list_insert_next( &vl, &node1, &node2 );
  vec_list_insert_next( &vl, &node2, &node3 );
  ret = vec_list_delete_next( &vl, &node1 );
  ASSERT_EQ( 2, vec_list_num( &vl ) );
  ASSERT_PTREQ( &node1, vec_list_node_next( vec_list_root(&vl) ) );
  ASSERT_PTREQ( &node3, vec_list_node_next( &node1 ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_next( &node3 ) );
  ASSERT_PTREQ( &node2, vec_list_node_next( &node2 ) );
  ASSERT_PTREQ( &node2, ret );
}

TEST(test_vec_list_is_empty)
{
  vec_list_init( &vl );
  ASSERT_TRUE( vec_list_is_empty( &vl ) );
  vec_list_insert_tail( &vl, &node1 );
  ASSERT_FALSE( vec_list_is_empty( &vl ) );
  vec_list_delete_tail( &vl );
  ASSERT_TRUE( vec_list_is_empty( &vl ) );
}

TEST(test_vec_list_destroy)
{
  vec_t v;

  v = vec_create( 2 );
  vec_list_node_set_data( &node1, v );
  vec_list_insert_tail( &vl, &node1 );
  vec_list_destroy( &vl );
  ASSERT_EQ( 0, vec_list_num( &vl ) );
  ASSERT_PTREQ( vec_list_root( &vl ),
                vec_list_node_next( vec_list_root(&vl) ) );
  ASSERT_PTREQ( NULL, vec_list_node_next(&node1) );
  ASSERT_PTREQ( NULL, vec_list_node_data(&node1) );
}

TEST_SUITE(test_vec_list_node)
{
  CONFIGURE_SUITE(vec_list_node_setup, vec_list_node_teardown);
  RUN_TEST(test_vec_list_node_init);
  RUN_TEST(test_vec_list_node_insert_next);
  RUN_TEST(test_vec_list_node_insert_next_append);
  RUN_TEST(test_vec_list_node_insert_next_insert);
  RUN_TEST(test_vec_list_node_delete_next);
  RUN_TEST(test_vec_list_node_delete_next_last);
  RUN_TEST(test_vec_list_node_delete_next_between);
  RUN_TEST(test_vec_list_node_set_data);
  RUN_TEST(test_vec_list_node_destroy);
  RUN_TEST(test_vec_list_node_destroy_null);
}

TEST_SUITE(test_vec_list)
{
  CONFIGURE_SUITE(vec_list_setup, vec_list_teardown);
  RUN_TEST(test_vec_list_init);
  RUN_TEST(test_vec_list_inc);
  RUN_TEST(test_vec_list_insert_tail);
  RUN_TEST(test_vec_list_insert_next);
  RUN_TEST(test_vec_list_insert_next_append);
  RUN_TEST(test_vec_list_insert_next_insert);
  RUN_TEST(test_vec_list_dec);
  RUN_TEST(test_vec_list_delete_tail);
  RUN_TEST(test_vec_list_delete_tail2);
  RUN_TEST(test_vec_list_delete_next);
  RUN_TEST(test_vec_list_delete_next_last);
  RUN_TEST(test_vec_list_delete_next_between);
  RUN_TEST(test_vec_list_is_empty);
  RUN_TEST(test_vec_list_destroy);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_vec_list_node);
  RUN_SUITE(test_vec_list);
  TEST_REPORT();
  TEST_EXIT();
}
