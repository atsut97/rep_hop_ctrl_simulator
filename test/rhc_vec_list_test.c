#include "rhc_vec_list.h"
#include "rhc_test.h"

vec_list_node_t *node1, *node2, *node3;

void vec_list_node_setup()
{
  if( ( node1 = nalloc( vec_list_node_t, 1 ) ) == NULL )
    goto ERROR;
  if( ( node2 = nalloc( vec_list_node_t, 1 ) ) == NULL )
    goto ERROR;
  if( ( node3 = nalloc( vec_list_node_t, 1 ) ) == NULL )
    goto ERROR;
  vec_list_node_init( node1 );
  vec_list_node_init( node2 );
  vec_list_node_init( node3 );
  return;

ERROR:
  ALLOC_ERR();
  return;
}

void vec_list_node_teardown()
{
  vec_list_node_destroy( node3 );
  vec_list_node_destroy( node2 );
  vec_list_node_destroy( node1 );
  sfree( node3 );
  sfree( node2 );
  sfree( node1 );
}

TEST(test_vec_list_node_init)
{
  ASSERT_PTREQ( node1, vec_list_node_prev( node1 ) );
  ASSERT_PTREQ( node1, vec_list_node_next( node1 ) );
  ASSERT_PTREQ( NULL, vec_list_node_data( node1 ) );
}

TEST(test_vec_list_node_insert_prev)
{
  vec_list_node_insert_prev( node1, node2 );
  ASSERT_PTREQ( node2, vec_list_node_prev( node1 ) );
  ASSERT_PTREQ( node1, vec_list_node_prev( node2 ) );
  ASSERT_PTREQ( node2, vec_list_node_next( node1 ) );
  ASSERT_PTREQ( node1, vec_list_node_next( node2 ) );
}

TEST(test_vec_list_node_insert_next)
{
  vec_list_node_insert_next( node1, node2 );
  ASSERT_PTREQ( node2, vec_list_node_next( node1 ) );
  ASSERT_PTREQ( node1, vec_list_node_next( node2 ) );
  ASSERT_PTREQ( node2, vec_list_node_prev( node1 ) );
  ASSERT_PTREQ( node1, vec_list_node_prev( node2 ) );
}

TEST(test_vec_list_node_insert_prev_prepend)
{
  vec_list_node_insert_prev( node1, node2 );
  vec_list_node_insert_prev( node2, node3 );
  ASSERT_PTREQ( node2, vec_list_node_prev( node1 ) );
  ASSERT_PTREQ( node3, vec_list_node_prev( node2 ) );
  ASSERT_PTREQ( node1, vec_list_node_prev( node3 ) );
  ASSERT_PTREQ( node3, vec_list_node_next( node1 ) );
  ASSERT_PTREQ( node2, vec_list_node_next( node3 ) );
  ASSERT_PTREQ( node1, vec_list_node_next( node2 ) );
}

TEST(test_vec_list_node_insert_next_append)
{
  vec_list_node_insert_next( node1, node2 );
  vec_list_node_insert_next( node2, node3 );
  ASSERT_PTREQ( node2, vec_list_node_next( node1 ) );
  ASSERT_PTREQ( node3, vec_list_node_next( node2 ) );
  ASSERT_PTREQ( node1, vec_list_node_next( node3 ) );
  ASSERT_PTREQ( node3, vec_list_node_prev( node1 ) );
  ASSERT_PTREQ( node2, vec_list_node_prev( node3 ) );
  ASSERT_PTREQ( node1, vec_list_node_prev( node2 ) );
}

TEST(test_vec_list_node_insert_prev_insert)
{
  vec_list_node_insert_prev( node1, node2 );
  vec_list_node_insert_prev( node1, node3 );
  ASSERT_PTREQ( node3, vec_list_node_prev( node1 ) );
  ASSERT_PTREQ( node2, vec_list_node_prev( node3 ) );
  ASSERT_PTREQ( node1, vec_list_node_prev( node2 ) );
  ASSERT_PTREQ( node2, vec_list_node_next( node1 ) );
  ASSERT_PTREQ( node3, vec_list_node_next( node2 ) );
  ASSERT_PTREQ( node1, vec_list_node_next( node3 ) );
}

TEST(test_vec_list_node_insert_next_insert)
{
  vec_list_node_insert_next( node1, node2 );
  vec_list_node_insert_next( node1, node3 );
  ASSERT_PTREQ( node3, vec_list_node_next( node1 ) );
  ASSERT_PTREQ( node2, vec_list_node_next( node3 ) );
  ASSERT_PTREQ( node1, vec_list_node_next( node2 ) );
  ASSERT_PTREQ( node2, vec_list_node_prev( node1 ) );
  ASSERT_PTREQ( node3, vec_list_node_prev( node2 ) );
  ASSERT_PTREQ( node1, vec_list_node_prev( node3 ) );
}

TEST(test_vec_list_node_delete_prev)
{
  vec_list_node_t *ret;

  vec_list_node_insert_prev( node1, node2 );
  ret = vec_list_node_delete_prev( node1 );
  ASSERT_PTREQ( node1, vec_list_node_prev( node1 ) );
  ASSERT_PTREQ( node1, vec_list_node_next( node1 ) );
  ASSERT_PTREQ( node2, vec_list_node_prev( node2 ) );
  ASSERT_PTREQ( node2, vec_list_node_next( node2 ) );
  ASSERT_PTREQ( node2, ret );
}

TEST(test_vec_list_node_delete_prev_last)
{
  vec_list_node_t *ret;

  vec_list_node_insert_prev( node1, node2 );
  vec_list_node_insert_prev( node2, node3 );
  ret = vec_list_node_delete_prev( node2 );
  ASSERT_PTREQ( node2, vec_list_node_prev( node1 ) );
  ASSERT_PTREQ( node1, vec_list_node_prev( node2 ) );
  ASSERT_PTREQ( node2, vec_list_node_next( node1 ) );
  ASSERT_PTREQ( node1, vec_list_node_next( node2 ) );
  ASSERT_PTREQ( node3, vec_list_node_prev( node3 ) );
  ASSERT_PTREQ( node3, vec_list_node_next( node3 ) );
  ASSERT_PTREQ( node3, ret );
}

TEST(test_vec_list_node_delete_prev_between)
{
  vec_list_node_t *ret;

  vec_list_node_insert_prev( node1, node2 );
  vec_list_node_insert_prev( node2, node3 );
  ret = vec_list_node_delete_prev( node1 );
  ASSERT_PTREQ( node3, vec_list_node_prev( node1 ) );
  ASSERT_PTREQ( node1, vec_list_node_prev( node3 ) );
  ASSERT_PTREQ( node3, vec_list_node_next( node1 ) );
  ASSERT_PTREQ( node1, vec_list_node_next( node3 ) );
  ASSERT_PTREQ( node2, vec_list_node_prev( node2 ) );
  ASSERT_PTREQ( node2, vec_list_node_next( node2 ) );
  ASSERT_PTREQ( node2, ret );
}

TEST(test_vec_list_node_delete_next)
{
  vec_list_node_t *ret;

  vec_list_node_insert_next( node1, node2 );
  ret = vec_list_node_delete_next( node1 );
  ASSERT_PTREQ( node1, vec_list_node_next( node1 ) );
  ASSERT_PTREQ( node1, vec_list_node_prev( node1 ) );
  ASSERT_PTREQ( node2, vec_list_node_next( node2 ) );
  ASSERT_PTREQ( node2, vec_list_node_prev( node2 ) );
  ASSERT_PTREQ( node2, ret );
}

TEST(test_vec_list_node_delete_next_last)
{
  vec_list_node_t *ret;

  vec_list_node_insert_next( node1, node2 );
  vec_list_node_insert_next( node2, node3 );
  ret = vec_list_node_delete_next( node2 );
  ASSERT_PTREQ( node2, vec_list_node_next( node1 ) );
  ASSERT_PTREQ( node1, vec_list_node_next( node2 ) );
  ASSERT_PTREQ( node2, vec_list_node_prev( node1 ) );
  ASSERT_PTREQ( node1, vec_list_node_prev( node2 ) );
  ASSERT_PTREQ( node3, vec_list_node_next( node3 ) );
  ASSERT_PTREQ( node3, vec_list_node_prev( node3 ) );
  ASSERT_PTREQ( node3, ret );
}

TEST(test_vec_list_node_delete_next_between)
{
  vec_list_node_t *ret;

  vec_list_node_insert_next( node1, node2 );
  vec_list_node_insert_next( node2, node3 );
  ret = vec_list_node_delete_next( node1 );
  ASSERT_PTREQ( node3, vec_list_node_next( node1 ) );
  ASSERT_PTREQ( node1, vec_list_node_next( node3 ) );
  ASSERT_PTREQ( node3, vec_list_node_prev( node1 ) );
  ASSERT_PTREQ( node1, vec_list_node_prev( node3 ) );
  ASSERT_PTREQ( node2, vec_list_node_next( node2 ) );
  ASSERT_PTREQ( node2, vec_list_node_prev( node2 ) );
  ASSERT_PTREQ( node2, ret );
}

TEST(test_vec_list_node_set_data)
{
  vec_t v, ret;

  v = vec_create( 2 );
  ret = vec_list_node_set_data( node1, v );
  ASSERT_PTREQ( v, vec_list_node_data(node1) );
  ASSERT_PTREQ( v, ret );
}

TEST(test_vec_list_node_destroy)
{
  vec_t v;

  vec_list_node_init( node1 );
  v = vec_create( 2 );
  vec_list_node_set_data( node1, v );
  vec_list_node_destroy( node1 );
  ASSERT_PTREQ( NULL, vec_list_node_prev(node1) );
  ASSERT_PTREQ( NULL, vec_list_node_next(node1) );
  ASSERT_PTREQ( NULL, vec_list_node_data(node1) );
}

TEST(test_vec_list_node_destroy_null)
{
  vec_t v;

  vec_list_node_init( node1 );
  v = NULL;
  vec_list_node_set_data( node1, v );
  vec_list_node_destroy( node1 );
  ASSERT_PTREQ( NULL, vec_list_node_prev(node1) );
  ASSERT_PTREQ( NULL, vec_list_node_next(node1) );
  ASSERT_PTREQ( NULL, vec_list_node_data(node1) );
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
                vec_list_node_prev( vec_list_root(&vl) ) );
  ASSERT_PTREQ( vec_list_root( &vl ),
                vec_list_node_next( vec_list_root(&vl) ) );
  sfree( node1 );
  sfree( node2 );
  sfree( node3 );
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
  sfree( node1 );
  sfree( node2 );
  sfree( node3 );
}

TEST(test_vec_list_insert_prev)
{
  vec_list_insert_prev( &vl, vec_list_root(&vl), node1 );
  ASSERT_EQ( 1, vec_list_num( &vl ) );
  ASSERT_PTREQ( node1, vec_list_node_prev( vec_list_root(&vl) ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_prev( node1 ) );
  ASSERT_PTREQ( node1, vec_list_node_next( vec_list_root(&vl) ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_next( node1 ) );

  vec_list_insert_prev( &vl, node1, node2 );
  ASSERT_EQ( 2, vec_list_num( &vl ) );
  ASSERT_PTREQ( node1, vec_list_node_prev( vec_list_root(&vl) ) );
  ASSERT_PTREQ( node2, vec_list_node_prev( node1 ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_prev( node2 ) );
  ASSERT_PTREQ( node2, vec_list_node_next( vec_list_root(&vl) ) );
  ASSERT_PTREQ( node1, vec_list_node_next( node2 ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_next( node1 ) );
  sfree( node3 );
}

TEST(test_vec_list_insert_prev_append)
{
  vec_list_insert_head( &vl, node1 );
  vec_list_insert_prev( &vl, node1, node2 );
  vec_list_insert_prev( &vl, node2, node3 );
  ASSERT_EQ( 3, vec_list_num(&vl) );
  ASSERT_PTREQ( node1, vec_list_head( &vl ) );
  ASSERT_PTREQ( node3, vec_list_tail( &vl ) );
  ASSERT_PTREQ( node1, vec_list_node_prev( vec_list_root(&vl) ) );
  ASSERT_PTREQ( node2, vec_list_node_prev( node1 ) );
  ASSERT_PTREQ( node3, vec_list_node_prev( node2 ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_prev( node3 ) );
  ASSERT_PTREQ( node3, vec_list_node_next( vec_list_root(&vl) ) );
  ASSERT_PTREQ( node2, vec_list_node_next( node3 ) );
  ASSERT_PTREQ( node1, vec_list_node_next( node2 ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_next( node1 ) );
}

TEST(test_vec_list_insert_prev_insert)
{
  vec_list_insert_head( &vl, node1 );
  vec_list_insert_prev( &vl, node1, node2 );
  vec_list_insert_prev( &vl, node1, node3 );
  ASSERT_EQ( 3, vec_list_num(&vl) );
  ASSERT_PTREQ( node1, vec_list_head( &vl ) );
  ASSERT_PTREQ( node2, vec_list_tail( &vl ) );
  ASSERT_PTREQ( node1, vec_list_node_prev( vec_list_root(&vl) ) );
  ASSERT_PTREQ( node3, vec_list_node_prev( node1 ) );
  ASSERT_PTREQ( node2, vec_list_node_prev( node3 ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_prev( node2 ) );
  ASSERT_PTREQ( node2, vec_list_node_next( vec_list_root(&vl) ) );
  ASSERT_PTREQ( node3, vec_list_node_next( node2 ) );
  ASSERT_PTREQ( node1, vec_list_node_next( node3 ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_next( node1 ) );
}

TEST(test_vec_list_insert_head)
{
  vec_list_insert_head( &vl, node1 );
  ASSERT_EQ( 1, vec_list_num( &vl ) );
  ASSERT_PTREQ( node1, vec_list_head( &vl ) );
  ASSERT_PTREQ( node1, vec_list_tail( &vl ) );
  ASSERT_PTREQ( node1, vec_list_node_prev( vec_list_root(&vl) ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_prev( node1 ) );
  ASSERT_PTREQ( node1, vec_list_node_next( vec_list_root(&vl) ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_next( node1 ) );

  vec_list_insert_head( &vl, node2 );
  ASSERT_EQ( 2, vec_list_num( &vl ) );
  ASSERT_PTREQ( node2, vec_list_head( &vl ) );
  ASSERT_PTREQ( node1, vec_list_tail( &vl ) );

  ASSERT_PTREQ( node2, vec_list_node_prev( vec_list_root(&vl) ) );
  ASSERT_PTREQ( node1, vec_list_node_prev( node2 ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_prev( node1 ) );

  ASSERT_PTREQ( node1, vec_list_node_next( vec_list_root(&vl) ) );
  ASSERT_PTREQ( node2, vec_list_node_next( node1 ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_next( node2 ) );
  sfree( node3 );
}

TEST(test_vec_list_insert_next)
{
  vec_list_insert_next( &vl, vec_list_root(&vl), node1 );
  ASSERT_EQ( 1, vec_list_num( &vl ) );
  ASSERT_PTREQ( node1, vec_list_node_next( vec_list_root(&vl) ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_next( node1 ) );
  ASSERT_PTREQ( node1, vec_list_node_prev( vec_list_root(&vl) ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_prev( node1 ) );

  vec_list_insert_next( &vl, node1, node2 );
  ASSERT_EQ( 2, vec_list_num( &vl ) );
  ASSERT_PTREQ( node1, vec_list_node_next( vec_list_root(&vl) ) );
  ASSERT_PTREQ( node2, vec_list_node_next( node1 ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_next( node2 ) );
  ASSERT_PTREQ( node2, vec_list_node_prev( vec_list_root(&vl) ) );
  ASSERT_PTREQ( node1, vec_list_node_prev( node2 ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_prev( node1 ) );
  sfree( node3 );
}

TEST(test_vec_list_insert_next_append)
{
  vec_list_insert_tail( &vl, node1 );
  vec_list_insert_next( &vl, node1, node2 );
  vec_list_insert_next( &vl, node2, node3 );
  ASSERT_EQ( 3, vec_list_num(&vl) );
  ASSERT_PTREQ( node1, vec_list_tail( &vl ) );
  ASSERT_PTREQ( node3, vec_list_head( &vl ) );
  ASSERT_PTREQ( node1, vec_list_node_next( vec_list_root(&vl) ) );
  ASSERT_PTREQ( node2, vec_list_node_next( node1 ) );
  ASSERT_PTREQ( node3, vec_list_node_next( node2 ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_next( node3 ) );
  ASSERT_PTREQ( node3, vec_list_node_prev( vec_list_root(&vl) ) );
  ASSERT_PTREQ( node2, vec_list_node_prev( node3 ) );
  ASSERT_PTREQ( node1, vec_list_node_prev( node2 ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_prev( node1 ) );
}

TEST(test_vec_list_insert_next_insert)
{
  vec_list_insert_tail( &vl, node1 );
  vec_list_insert_next( &vl, node1, node2 );
  vec_list_insert_next( &vl, node1, node3 );
  ASSERT_EQ( 3, vec_list_num(&vl) );
  ASSERT_PTREQ( node1, vec_list_tail( &vl ) );
  ASSERT_PTREQ( node2, vec_list_head( &vl ) );
  ASSERT_PTREQ( node1, vec_list_node_next( vec_list_root(&vl) ) );
  ASSERT_PTREQ( node3, vec_list_node_next( node1 ) );
  ASSERT_PTREQ( node2, vec_list_node_next( node3 ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_next( node2 ) );
  ASSERT_PTREQ( node2, vec_list_node_prev( vec_list_root(&vl) ) );
  ASSERT_PTREQ( node3, vec_list_node_prev( node2 ) );
  ASSERT_PTREQ( node1, vec_list_node_prev( node3 ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_prev( node1 ) );
}

TEST(test_vec_list_insert_tail)
{
  vec_list_insert_tail( &vl, node1 );
  ASSERT_EQ( 1, vec_list_num( &vl ) );
  ASSERT_PTREQ( node1, vec_list_tail( &vl ) );
  ASSERT_PTREQ( node1, vec_list_head( &vl ) );
  ASSERT_PTREQ( node1, vec_list_node_next( vec_list_root(&vl) ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_next( node1 ) );
  ASSERT_PTREQ( node1, vec_list_node_prev( vec_list_root(&vl) ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_prev( node1 ) );

  vec_list_insert_tail( &vl, node2 );
  ASSERT_EQ( 2, vec_list_num( &vl ) );
  ASSERT_PTREQ( node2, vec_list_tail( &vl ) );
  ASSERT_PTREQ( node1, vec_list_head( &vl ) );
  ASSERT_PTREQ( node2, vec_list_node_next( vec_list_root(&vl) ) );
  ASSERT_PTREQ( node1, vec_list_node_next( node2 ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_next( node1 ) );
  ASSERT_PTREQ( node1, vec_list_node_prev( vec_list_root(&vl) ) );
  ASSERT_PTREQ( node2, vec_list_node_prev( node1 ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_prev( node2 ) );
  sfree( node3 );
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
  sfree( node1 );
  sfree( node2 );
  sfree( node3 );
}

TEST(test_vec_list_delete_prev)
{
  vec_list_node_t *ret;

  vec_list_insert_prev( &vl, vec_list_root(&vl), node1 );
  vec_list_insert_prev( &vl, node1, node2 );
  ret = vec_list_delete_prev( &vl, node1 );
  ASSERT_EQ( 1, vec_list_num( &vl ) );
  ASSERT_PTREQ( node1, vec_list_node_prev( vec_list_root(&vl) ) );
  ASSERT_PTREQ( vec_list_node_prev( vec_list_root(&vl) ), node1 );
  ASSERT_PTREQ( node1, vec_list_node_next( vec_list_root(&vl) ) );
  ASSERT_PTREQ( vec_list_node_next( vec_list_root(&vl) ), node1 );
  ASSERT_PTREQ( node2, vec_list_node_next( node2 ) );
  ASSERT_PTREQ( node2, vec_list_node_prev( node2 ) );
  ASSERT_PTREQ( node2, ret );
  sfree( node2 );
  sfree( node3 );
}

TEST(test_vec_list_delete_prev_last)
{
  vec_list_node_t *ret;

  vec_list_insert_head( &vl, node1 );
  vec_list_insert_prev( &vl, node1, node2 );
  vec_list_insert_prev( &vl, node2, node3 );
  ret = vec_list_delete_prev( &vl, node2 );
  ASSERT_EQ( 2, vec_list_num( &vl ) );
  ASSERT_PTREQ( node1, vec_list_node_prev( vec_list_root(&vl) ) );
  ASSERT_PTREQ( node2, vec_list_node_prev( node1 ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_prev( node2 ) );
  ASSERT_PTREQ( node2, vec_list_node_next( vec_list_root(&vl) ) );
  ASSERT_PTREQ( node1, vec_list_node_next( node2 ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_next( node1 ) );
  ASSERT_PTREQ( node3, vec_list_node_next( node3 ) );
  ASSERT_PTREQ( node3, vec_list_node_prev( node3 ) );
  ASSERT_PTREQ( node3, ret );
  sfree( node3 );
}

TEST(test_vec_list_delete_prev_between)
{
  vec_list_node_t *ret;

  vec_list_insert_head( &vl, node1 );
  vec_list_insert_prev( &vl, node1, node2 );
  vec_list_insert_prev( &vl, node2, node3 );
  ret = vec_list_delete_prev( &vl, node1 );
  ASSERT_EQ( 2, vec_list_num( &vl ) );
  ASSERT_PTREQ( node1, vec_list_node_prev( vec_list_root(&vl) ) );
  ASSERT_PTREQ( node3, vec_list_node_prev( node1 ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_prev( node3 ) );
  ASSERT_PTREQ( node3, vec_list_node_next( vec_list_root(&vl) ) );
  ASSERT_PTREQ( node1, vec_list_node_next( node3 ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_next( node1 ) );
  ASSERT_PTREQ( node2, vec_list_node_next( node2 ) );
  ASSERT_PTREQ( node2, vec_list_node_prev( node2 ) );
  ASSERT_PTREQ( node2, ret );
  sfree( node2 );
}

TEST(test_vec_list_delete_head)
{
  vec_list_node_t *ret;

  vec_list_insert_head( &vl, node1 );
  vec_list_insert_head( &vl, node2 );
  ret = vec_list_delete_head( &vl );
  ASSERT_EQ( 1, vec_list_num( &vl ) );
  ASSERT_PTREQ( node1, vec_list_node_prev( vec_list_root(&vl) ) );
  ASSERT_PTREQ( vec_list_node_prev( vec_list_root(&vl) ), node1 );
  ASSERT_PTREQ( node1, vec_list_node_next( vec_list_root(&vl) ) );
  ASSERT_PTREQ( vec_list_node_next( vec_list_root(&vl) ), node1 );
  ASSERT_PTREQ( node2, vec_list_node_next( node2 ) );
  ASSERT_PTREQ( node2, vec_list_node_prev( node2 ) );
  ASSERT_PTREQ( node2, ret );
  sfree( node2 );
  sfree( node3 );
}

TEST(test_vec_list_delete_head2)
{
  vec_list_node_t *ret;

  vec_list_insert_head( &vl, node1 );
  vec_list_insert_head( &vl, node2 );
  vec_list_insert_head( &vl, node3 );

  ret = vec_list_delete_head( &vl );
  ASSERT_EQ( 2, vec_list_num( &vl ) );
  ASSERT_PTREQ( node2, vec_list_node_prev( vec_list_root(&vl) ) );
  ASSERT_PTREQ( node1, vec_list_node_prev( node2 ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_prev( node1 ) );
  ASSERT_PTREQ( node1, vec_list_node_next( vec_list_root(&vl) ) );
  ASSERT_PTREQ( node2, vec_list_node_next( node1 ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_next( node2 ) );
  ASSERT_PTREQ( node3, vec_list_node_next( node3 ) );
  ASSERT_PTREQ( node3, vec_list_node_prev( node3 ) );
  ASSERT_PTREQ( node3, ret );

  ret = vec_list_delete_head( &vl );
  ASSERT_EQ( 1, vec_list_num( &vl ) );
  ASSERT_PTREQ( node1, vec_list_node_prev( vec_list_root(&vl) ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_prev( node1 ) );
  ASSERT_PTREQ( node1, vec_list_node_next( vec_list_root(&vl) ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_next( node1 ) );
  ASSERT_PTREQ( node2, vec_list_node_next( node2 ) );
  ASSERT_PTREQ( node2, vec_list_node_prev( node2 ) );
  ASSERT_PTREQ( node2, ret );

  ret = vec_list_delete_head( &vl );
  ASSERT_EQ( 0, vec_list_num( &vl ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_prev( vec_list_root(&vl) ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_next( vec_list_root(&vl) ) );
  ASSERT_PTREQ( node1, vec_list_node_prev( node1 ) );
  ASSERT_PTREQ( node1, vec_list_node_next( node1 ) );
  ASSERT_PTREQ( node1, ret );
  sfree( node1 );
  sfree( node2 );
  sfree( node3 );
}

TEST(test_vec_list_delete_next)
{
  vec_list_node_t *ret;

  vec_list_insert_next( &vl, vec_list_root(&vl), node1 );
  vec_list_insert_next( &vl, node1, node2 );
  ret = vec_list_delete_next( &vl, node1 );
  ASSERT_EQ( 1, vec_list_num( &vl ) );
  ASSERT_PTREQ( node1, vec_list_node_next( vec_list_root(&vl) ) );
  ASSERT_PTREQ( vec_list_node_next( vec_list_root(&vl) ), node1 );
  ASSERT_PTREQ( node1, vec_list_node_prev( vec_list_root(&vl) ) );
  ASSERT_PTREQ( vec_list_node_prev( vec_list_root(&vl) ), node1 );
  ASSERT_PTREQ( node2, vec_list_node_next( node2 ) );
  ASSERT_PTREQ( node2, vec_list_node_prev( node2 ) );
  ASSERT_PTREQ( node2, ret );
  sfree( node2 );
  sfree( node3 );
}

TEST(test_vec_list_delete_next_last)
{
  vec_list_node_t *ret;

  vec_list_insert_tail( &vl, node1 );
  vec_list_insert_next( &vl, node1, node2 );
  vec_list_insert_next( &vl, node2, node3 );
  ret = vec_list_delete_next( &vl, node2 );
  ASSERT_EQ( 2, vec_list_num( &vl ) );
  ASSERT_PTREQ( node1, vec_list_node_next( vec_list_root(&vl) ) );
  ASSERT_PTREQ( node2, vec_list_node_next( node1 ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_next( node2 ) );
  ASSERT_PTREQ( node2, vec_list_node_prev( vec_list_root(&vl) ) );
  ASSERT_PTREQ( node1, vec_list_node_prev( node2 ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_prev( node1 ) );
  ASSERT_PTREQ( node3, vec_list_node_next( node3 ) );
  ASSERT_PTREQ( node3, vec_list_node_prev( node3 ) );
  ASSERT_PTREQ( node3, ret );
  sfree( node3 );
}

TEST(test_vec_list_delete_next_between)
{
  vec_list_node_t *ret;

  vec_list_insert_tail( &vl, node1 );
  vec_list_insert_next( &vl, node1, node2 );
  vec_list_insert_next( &vl, node2, node3 );
  ret = vec_list_delete_next( &vl, node1 );
  ASSERT_EQ( 2, vec_list_num( &vl ) );
  ASSERT_PTREQ( node1, vec_list_node_next( vec_list_root(&vl) ) );
  ASSERT_PTREQ( node3, vec_list_node_next( node1 ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_next( node3 ) );
  ASSERT_PTREQ( node3, vec_list_node_prev( vec_list_root(&vl) ) );
  ASSERT_PTREQ( node1, vec_list_node_prev( node3 ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_prev( node1 ) );
  ASSERT_PTREQ( node2, vec_list_node_next( node2 ) );
  ASSERT_PTREQ( node2, vec_list_node_prev( node2 ) );
  ASSERT_PTREQ( node2, ret );
  sfree( node2 );
}

TEST(test_vec_list_delete_tail)
{
  vec_list_node_t *ret;

  vec_list_insert_tail( &vl, node1 );
  vec_list_insert_tail( &vl, node2 );
  ret = vec_list_delete_tail( &vl );
  ASSERT_EQ( 1, vec_list_num( &vl ) );
  ASSERT_PTREQ( node1, vec_list_node_next( vec_list_root(&vl) ) );
  ASSERT_PTREQ( vec_list_node_next( vec_list_root(&vl) ), node1 );
  ASSERT_PTREQ( node1, vec_list_node_prev( vec_list_root(&vl) ) );
  ASSERT_PTREQ( vec_list_node_prev( vec_list_root(&vl) ), node1 );
  ASSERT_PTREQ( node2, vec_list_node_next( node2 ) );
  ASSERT_PTREQ( node2, vec_list_node_prev( node2 ) );
  ASSERT_PTREQ( node2, ret );
  sfree( node2 );
  sfree( node3 );
}

TEST(test_vec_list_delete_tail2)
{
  vec_list_node_t *ret;

  vec_list_insert_tail( &vl, node1 );
  vec_list_insert_tail( &vl, node2 );
  vec_list_insert_tail( &vl, node3 );

  ret = vec_list_delete_tail( &vl );
  ASSERT_EQ( 2, vec_list_num( &vl ) );
  ASSERT_PTREQ( node2, vec_list_node_next( vec_list_root(&vl) ) );
  ASSERT_PTREQ( node1, vec_list_node_next( node2 ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_next( node1 ) );
  ASSERT_PTREQ( node1, vec_list_node_prev( vec_list_root(&vl) ) );
  ASSERT_PTREQ( node2, vec_list_node_prev( node1 ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_prev( node2 ) );
  ASSERT_PTREQ( node3, vec_list_node_next( node3 ) );
  ASSERT_PTREQ( node3, vec_list_node_prev( node3 ) );
  ASSERT_PTREQ( node3, ret );

  ret = vec_list_delete_tail( &vl );
  ASSERT_EQ( 1, vec_list_num( &vl ) );
  ASSERT_PTREQ( node1, vec_list_node_next( vec_list_root(&vl) ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_next( node1 ) );
  ASSERT_PTREQ( node1, vec_list_node_prev( vec_list_root(&vl) ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_prev( node1 ) );
  ASSERT_PTREQ( node2, vec_list_node_next( node2 ) );
  ASSERT_PTREQ( node2, vec_list_node_prev( node2 ) );
  ASSERT_PTREQ( node2, ret );

  ret = vec_list_delete_tail( &vl );
  ASSERT_EQ( 0, vec_list_num( &vl ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_next( vec_list_root(&vl) ) );
  ASSERT_PTREQ( vec_list_root(&vl), vec_list_node_prev( vec_list_root(&vl) ) );
  ASSERT_PTREQ( node1, vec_list_node_next( node1 ) );
  ASSERT_PTREQ( node1, vec_list_node_prev( node1 ) );
  ASSERT_PTREQ( node1, ret );
  sfree( node1 );
  sfree( node2 );
  sfree( node3 );
}

TEST(test_vec_list_is_empty)
{
  vec_list_init( &vl );
  ASSERT_TRUE( vec_list_is_empty( &vl ) );
  vec_list_insert_tail( &vl, node1 );
  ASSERT_FALSE( vec_list_is_empty( &vl ) );
  vec_list_delete_tail( &vl );
  ASSERT_TRUE( vec_list_is_empty( &vl ) );
  sfree( node1 );
  sfree( node2 );
  sfree( node3 );
}

TEST(test_vec_list_destroy)
{
  vec_t v;

  v = vec_create( 2 );
  vec_list_node_set_data( node1, v );
  vec_list_insert_tail( &vl, node1 );
  vec_list_destroy( &vl );
  ASSERT_EQ( 0, vec_list_num( &vl ) );
  ASSERT_PTREQ( vec_list_root( &vl ),
                vec_list_node_next( vec_list_root(&vl) ) );
  sfree( node2 );
  sfree( node3 );
}

TEST(test_vec_list_push_pop)
{
  vec_list_node_t *ret;

  ASSERT_EQ( 0, vec_list_num( &vl ) );

  vec_list_push( &vl, node1 );
  ASSERT_EQ( 1, vec_list_num( &vl ) );
  ASSERT_PTREQ( node1, vec_list_head( &vl ) );
  ASSERT_PTREQ( node1, vec_list_tail( &vl ) );

  vec_list_push( &vl, node2 );
  ASSERT_EQ( 2, vec_list_num( &vl ) );
  ASSERT_PTREQ( node2, vec_list_head( &vl ) );
  ASSERT_PTREQ( node1, vec_list_tail( &vl ) );

  vec_list_push( &vl, node3 );
  ASSERT_EQ( 3, vec_list_num( &vl ) );
  ASSERT_PTREQ( node3, vec_list_head( &vl ) );
  ASSERT_PTREQ( node1, vec_list_tail( &vl ) );

  ret = vec_list_pop( &vl );
  ASSERT_EQ( 2, vec_list_num( &vl ) );
  ASSERT_PTREQ( node2, vec_list_head( &vl ) );
  ASSERT_PTREQ( node1, vec_list_tail( &vl ) );
  ASSERT_PTREQ( node3, ret );

  ret = vec_list_pop( &vl );
  ASSERT_EQ( 1, vec_list_num( &vl ) );
  ASSERT_PTREQ( node1, vec_list_head( &vl ) );
  ASSERT_PTREQ( node1, vec_list_tail( &vl ) );
  ASSERT_PTREQ( node2, ret );

  ret = vec_list_pop( &vl );
  ASSERT_EQ( 0, vec_list_num( &vl ) );
  ASSERT_PTREQ( node1, ret );
  sfree( node1 );
  sfree( node2 );
  sfree( node3 );
}

TEST(test_vec_list_enqueue_dequeue)
{
  vec_list_node_t *ret;

  ASSERT_EQ( 0, vec_list_num( &vl ) );

  vec_list_enqueue( &vl, node1 );
  ASSERT_EQ( 1, vec_list_num( &vl ) );
  ASSERT_PTREQ( node1, vec_list_head( &vl ) );
  ASSERT_PTREQ( node1, vec_list_tail( &vl ) );

  vec_list_enqueue( &vl, node2 );
  ASSERT_EQ( 2, vec_list_num( &vl ) );
  ASSERT_PTREQ( node1, vec_list_head( &vl ) );
  ASSERT_PTREQ( node2, vec_list_tail( &vl ) );

  vec_list_enqueue( &vl, node3 );
  ASSERT_EQ( 3, vec_list_num( &vl ) );
  ASSERT_PTREQ( node1, vec_list_head( &vl ) );
  ASSERT_PTREQ( node3, vec_list_tail( &vl ) );

  ret = vec_list_dequeue( &vl );
  ASSERT_EQ( 2, vec_list_num( &vl ) );
  ASSERT_PTREQ( node2, vec_list_head( &vl ) );
  ASSERT_PTREQ( node3, vec_list_tail( &vl ) );
  ASSERT_PTREQ( node1, ret );

  ret = vec_list_dequeue( &vl );
  ASSERT_EQ( 1, vec_list_num( &vl ) );
  ASSERT_PTREQ( node3, vec_list_head( &vl ) );
  ASSERT_PTREQ( node3, vec_list_tail( &vl ) );
  ASSERT_PTREQ( node2, ret );

  ret = vec_list_dequeue( &vl );
  ASSERT_EQ( 0, vec_list_num( &vl ) );
  ASSERT_PTREQ( node3, ret );
  sfree( node1 );
  sfree( node2 );
  sfree( node3 );
}

TEST(test_vec_list_for_each)
{
  vec_list_node_t *node;
  int i = 0;

  vec_list_push( &vl, node1 );
  vec_list_push( &vl, node2 );
  vec_list_push( &vl, node3 );

  vec_list_for_each( &vl, node ){
    if( i == 0 ) ASSERT_PTREQ( node1, node );
    if( i == 1 ) ASSERT_PTREQ( node2, node );
    if( i == 2 ) ASSERT_PTREQ( node3, node );
    i++;
  }
}

TEST_SUITE(test_vec_list_node)
{
  CONFIGURE_SUITE(vec_list_node_setup, vec_list_node_teardown);
  RUN_TEST(test_vec_list_node_init);
  RUN_TEST(test_vec_list_node_insert_prev);
  RUN_TEST(test_vec_list_node_insert_next);
  RUN_TEST(test_vec_list_node_insert_prev_prepend);
  RUN_TEST(test_vec_list_node_insert_next_append);
  RUN_TEST(test_vec_list_node_insert_prev_insert);
  RUN_TEST(test_vec_list_node_insert_next_insert);
  RUN_TEST(test_vec_list_node_delete_prev);
  RUN_TEST(test_vec_list_node_delete_prev_last);
  RUN_TEST(test_vec_list_node_delete_prev_between);
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
  RUN_TEST(test_vec_list_insert_prev);
  RUN_TEST(test_vec_list_insert_prev_append);
  RUN_TEST(test_vec_list_insert_prev_insert);
  RUN_TEST(test_vec_list_insert_head);
  RUN_TEST(test_vec_list_insert_next);
  RUN_TEST(test_vec_list_insert_next_append);
  RUN_TEST(test_vec_list_insert_next_insert);
  RUN_TEST(test_vec_list_insert_tail);
  RUN_TEST(test_vec_list_dec);
  RUN_TEST(test_vec_list_delete_prev);
  RUN_TEST(test_vec_list_delete_prev_last);
  RUN_TEST(test_vec_list_delete_prev_between);
  RUN_TEST(test_vec_list_delete_head);
  RUN_TEST(test_vec_list_delete_head2);
  RUN_TEST(test_vec_list_delete_next);
  RUN_TEST(test_vec_list_delete_next_last);
  RUN_TEST(test_vec_list_delete_next_between);
  RUN_TEST(test_vec_list_delete_tail);
  RUN_TEST(test_vec_list_delete_tail2);
  RUN_TEST(test_vec_list_is_empty);
  RUN_TEST(test_vec_list_destroy);
  RUN_TEST(test_vec_list_push_pop);
  RUN_TEST(test_vec_list_enqueue_dequeue);
  RUN_TEST(test_vec_list_for_each);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_vec_list_node);
  RUN_SUITE(test_vec_list);
  TEST_REPORT();
  TEST_EXIT();
}
