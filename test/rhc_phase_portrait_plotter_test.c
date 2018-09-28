#include "rhc_phase_portrait_plotter.h"
#include "rhc_ctrl_dummy.h"
#include "rhc_test.h"

cmd_t cmd;
ctrl_t ctrl;
model_t model;
logger_t logger;
ppp_t ppp;

void setup()
{
  cmd_default_init( &cmd );
  ctrl_dummy_create( &ctrl, &cmd, &model, 10 );
  model_init( &model, 1.0 );
  logger_init( &logger );
  ppp_init( &ppp, &cmd, &ctrl, &model, &logger );
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
  ASSERT_PTREQ( &logger, ppp_logger( &ppp ) );
  ASSERT_EQ( -HUGE_VAL, vec_elem( ppp_min( &ppp ), 0 ) );
  ASSERT_EQ( -HUGE_VAL, vec_elem( ppp_min( &ppp ), 1 ) );
  ASSERT_EQ(  HUGE_VAL, vec_elem( ppp_max( &ppp ), 0 ) );
  ASSERT_EQ(  HUGE_VAL, vec_elem( ppp_max( &ppp ), 1 ) );
  ASSERT_EQ( 10, ppp_n_sc( &ppp, 0 ) );
  ASSERT_EQ( 10, ppp_n_sc( &ppp, 1 ) );
  ASSERT_EQ( 0, vec_list_num( ppp_p0_list( &ppp ) ) );
}

TEST(test_phase_portrait_plotter_destroy)
{
  ppp_t ppp2;

  ppp_init( &ppp2, &cmd, &ctrl, &model, &logger );
  ppp_destroy( &ppp2 );
  ASSERT_PTREQ( NULL, ppp_cmd( &ppp2 ) );
  ASSERT_PTREQ( NULL, ppp_ctrl( &ppp2 ) );
  ASSERT_PTREQ( NULL, ppp_model( &ppp2 ) );
  ASSERT_PTREQ( NULL, ppp2.n_sc );
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
  vec_destroy( min );
  vec_destroy( max );
}

TEST(test_phase_portrait_plotter_set_lim_xy)
{
  struct case_t {
    double xmin, xmax;
    double ymin, ymax;
    bool end;
  } cases[] = {
    { -3, 3, -5, 5, false },
    { -0.8, 0.8, -0.5, 0.5, false },
    { 0.23, 0.29, -4, 6, false },
    { 0, 0, 0, 0, true },
  };
  struct case_t *c;

  for( c=cases; !c->end; c++ ){
    ppp_set_lim_xy( &ppp, c->xmin, c->xmax, c->ymin, c->ymax );
    ASSERT_EQ( c->xmin, vec_elem( ppp_min(&ppp), 0 ) );
    ASSERT_EQ( c->xmax, vec_elem( ppp_max(&ppp), 0 ) );
    ASSERT_EQ( c->ymin, vec_elem( ppp_min(&ppp), 1 ) );
    ASSERT_EQ( c->ymax, vec_elem( ppp_max(&ppp), 1 ) );
  }
}

TEST(test_phase_portrait_plotter_set_n_sc)
{
  int n_sc[] = { 10, 15 };
  ppp_set_n_sc( &ppp, n_sc );
  ASSERT_EQ( 10, ppp_n_sc(&ppp, 0) );
  ASSERT_EQ( 15, ppp_n_sc(&ppp, 1) );
}

TEST(test_phase_portrait_plotter_set_n_sc_xy)
{
  struct case_t {
    int n_x, n_y;
    bool end;
  } cases[] = {
    { 10, 10, false },
    { 10, 15, false },
    { 5, 8, false },
    { 0, 0, true },
  };
  struct case_t *c;

  for( c=cases; !c->end; c++ ){
    ppp_set_n_sc_xy( &ppp, c->n_x, c->n_y );
    ASSERT_EQ( c->n_x, ppp_n_sc( &ppp, 0 ) );
    ASSERT_EQ( c->n_y, ppp_n_sc( &ppp, 1 ) );
  }
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

TEST(test_phase_portrait_plotter_generate_edge_points)
{
  vec_t min = vec_create_list( 2, -4.0, -4.0 );
  vec_t max = vec_create_list( 2,  4.0,  4.0 );
  int n_sc[2] = { 4, 4 };
  vec_list_node_t *node;
  struct case_t {
    double x, y;
  } cases[] = {
    { -4, -4 }, { -2, -4 }, {  0, -4 }, {  2, -4 },
    {  4,  4 }, {  2,  4 }, {  0,  4 }, { -2,  4 },
    { -4,  4 }, { -4,  2 }, { -4,  0 }, { -4, -2 },
    {  4, -4 }, {  4, -2 }, {  4,  0 }, {  4,  2 },
  };
  struct case_t *c = cases;

  ppp_set_lim( &ppp, min, max );
  ppp_set_n_sc( &ppp, n_sc );
  ppp_generate_edge_points( &ppp );

  ASSERT_EQ( 16, vec_list_num( ppp_p0_list( &ppp ) ) );
  vec_list_for_each( ppp_p0_list( &ppp ), node ){
    ASSERT_DOUBLE_EQ( c->x, vec_elem( node->v, 0 ) );
    ASSERT_DOUBLE_EQ( c->y, vec_elem( node->v, 1 ) );
    c++;
  }

  vec_destroy( min );
  vec_destroy( max );
}

TEST(test_phase_portrait_plotter_simulator_update)
{
  vec_t p0 = vec_create_list( 2, -3.0, 3.0 );

  simulator_set_state( ppp_simulator(&ppp), p0 );
  ASSERT_EQ( 0, vec_ring_size( ppp_point_buf(&ppp) ) );
  ASSERT_GE( vec_ring_capacity( ppp_point_buf(&ppp) ), 10 );

  ppp_simulator_update( ppp_simulator(&ppp), 0, 0.001, &ppp );
  ASSERT_EQ( 1, vec_ring_size( ppp_point_buf(&ppp) ) );
  ASSERT_TRUE( vec_match( vec_ring_head( ppp_point_buf(&ppp) ), p0 ) );

  ppp_simulator_update( ppp_simulator(&ppp), 0, 0.001, &ppp );
  ASSERT_EQ( 2, vec_ring_size( ppp_point_buf(&ppp) ) );
  ASSERT_FALSE( vec_match( vec_ring_head( ppp_point_buf(&ppp) ), p0 ) );

  vec_destroy( p0 );
}

TEST_SUITE(test_phase_portrait_plotter)
{
  CONFIGURE_SUITE( setup, teardown );
  RUN_TEST(test_phase_portrait_plotter_init);
  RUN_TEST(test_phase_portrait_plotter_destroy);
  RUN_TEST(test_phase_portrait_plotter_set_lim);
  RUN_TEST(test_phase_portrait_plotter_set_lim_xy);
  RUN_TEST(test_phase_portrait_plotter_set_n_sc);
  RUN_TEST(test_phase_portrait_plotter_set_n_sc_xy);
  RUN_TEST(test_phase_portrait_plotter_push_p0);
  RUN_TEST(test_phase_portrait_plotter_generate_edge_points);
  RUN_TEST(test_phase_portrait_plotter_simulator_update);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_phase_portrait_plotter);
  TEST_REPORT();
  TEST_EXIT();
}
