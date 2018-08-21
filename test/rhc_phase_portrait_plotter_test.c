#include "rhc_phase_portrait_plotter.h"
#include "rhc_ctrl_dummy.h"
#include "rhc_test.h"

cmd_t cmd;
ctrl_t ctrl;
model_t model;
ppp_t ppp;

void setup()
{
  cmd_default_init( &cmd );
  ctrl_dummy_create( &ctrl, &cmd, &model, 10 );
  model_init( &model, 1.0 );
  ppp_init( &ppp, &cmd, &ctrl, &model );
}

void teardown()
{
  ppp_destroy( &ppp );
  model_destroy( &model );
  ctrl_destroy( &ctrl );
  cmd_destroy( &cmd );
}

TEST(test_phase_portrait_plotter_init)
{
  ASSERT_EQ( 2, ppp_dim( &ppp ) );
  ASSERT_PTREQ( &cmd, ppp_cmd( &ppp ) );
  ASSERT_PTREQ( &model, ppp_model( &ppp ) );
  ASSERT_PTREQ( &ctrl, ppp_ctrl( &ppp ) );
  ASSERT_EQ( 0, vec_elem( ppp_min( &ppp ), 0 ) );
  ASSERT_EQ( 0, vec_elem( ppp_min( &ppp ), 1 ) );
  ASSERT_EQ( 0, vec_elem( ppp_max( &ppp ), 0 ) );
  ASSERT_EQ( 0, vec_elem( ppp_max( &ppp ), 1 ) );
  ASSERT_EQ( 10, ppp_n_sc( &ppp, 0 ) );
  ASSERT_EQ( 10, ppp_n_sc( &ppp, 1 ) );
  ASSERT_EQ( 0, vec_list_num( ppp_p0_list( &ppp ) ) );
}

TEST(test_phase_portrait_plotter_destroy)
{
  ppp_destroy( &ppp );
  ASSERT_PTREQ( NULL, ppp_cmd( &ppp ) );
  ASSERT_PTREQ( NULL, ppp_ctrl( &ppp ) );
  ASSERT_PTREQ( NULL, ppp_model( &ppp ) );
  ASSERT_PTREQ( NULL, ppp.n_sc );
}

TEST(test_phase_portrait_plotter_set_lim)
{
  vec_t min, max;
  min = vec_create_list( 2, -3.0, -5.0 );
  max = vec_create_list( 2, 3.0, 5.0);

  ppp_set_lim( &ppp, min, max );
  ASSERT_EQ( -3.0, vec_elem( ppp_min(&ppp), 0 ) );
  ASSERT_EQ( -5.0, vec_elem( ppp_min(&ppp), 1 ) );
  ASSERT_EQ(  3.0, vec_elem( ppp_max(&ppp), 0 ) );
  ASSERT_EQ(  5.0, vec_elem( ppp_max(&ppp), 1 ) );
}

TEST(test_phase_portrait_plotter_push_p0)
{
  vec_t p0;
  vec_list_node_t *head;

  p0 = vec_create_list( 2, -3.0, 3.0 );
  ppp_push_p0( &ppp, p0 );
  head = vec_list_head( ppp_p0_list(&ppp) );
  ASSERT_EQ( -3.0, vec_elem( head->v, 0 ) );
  ASSERT_EQ(  3.0, vec_elem( head->v, 1 ) );
  ASSERT_PTRNE( p0, head->v );
  vec_destroy( p0 );
}

/* TEST(test_phase_portrait_plotter_) */
/* { */
/* } */

TEST_SUITE(test_phase_portrait_plotter)
{
  CONFIGURE_SUITE( setup, teardown );
  RUN_TEST(test_phase_portrait_plotter_init);
  RUN_TEST(test_phase_portrait_plotter_destroy);
  RUN_TEST(test_phase_portrait_plotter_set_lim);
  RUN_TEST(test_phase_portrait_plotter_push_p0);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_phase_portrait_plotter);
  TEST_REPORT();
  TEST_EXIT();
}
