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

  ppp_set_lim( &ppp, min, max );
  ppp_set_n_sc( &ppp, n_sc );
  ppp_generate_edge_points( &ppp );

  ASSERT_EQ( 16, vec_list_num( ppp_p0_list( &ppp ) ) );

  /* y = -4 */
  node = ppp_p0_list_tail( &ppp );
  ASSERT_DOUBLE_EQ( -4, vec_elem( node->v, 0 ) );
  ASSERT_DOUBLE_EQ( -4, vec_elem( node->v, 1 ) );
  node = vec_list_node_next( node );
  ASSERT_DOUBLE_EQ( -2, vec_elem( node->v, 0 ) );
  ASSERT_DOUBLE_EQ( -4, vec_elem( node->v, 1 ) );
  node = vec_list_node_next( node );
  ASSERT_DOUBLE_EQ(  0, vec_elem( node->v, 0 ) );
  ASSERT_DOUBLE_EQ( -4, vec_elem( node->v, 1 ) );
  node = vec_list_node_next( node );
  ASSERT_DOUBLE_EQ(  2, vec_elem( node->v, 0 ) );
  ASSERT_DOUBLE_EQ( -4, vec_elem( node->v, 1 ) );

  /* y = 4 */
  node = vec_list_node_next( node );
  ASSERT_DOUBLE_EQ( 4, vec_elem( node->v, 0 ) );
  ASSERT_DOUBLE_EQ( 4, vec_elem( node->v, 1 ) );
  node = vec_list_node_next( node );
  ASSERT_DOUBLE_EQ( 2, vec_elem( node->v, 0 ) );
  ASSERT_DOUBLE_EQ( 4, vec_elem( node->v, 1 ) );
  node = vec_list_node_next( node );
  ASSERT_DOUBLE_EQ( 0, vec_elem( node->v, 0 ) );
  ASSERT_DOUBLE_EQ( 4, vec_elem( node->v, 1 ) );
  node = vec_list_node_next( node );
  ASSERT_DOUBLE_EQ( -2, vec_elem( node->v, 0 ) );
  ASSERT_DOUBLE_EQ(  4, vec_elem( node->v, 1 ) );

  /* x = -4 */
  node = vec_list_node_next( node );
  ASSERT_DOUBLE_EQ( -4, vec_elem( node->v, 0 ) );
  ASSERT_DOUBLE_EQ(  4, vec_elem( node->v, 1 ) );
  node = vec_list_node_next( node );
  ASSERT_DOUBLE_EQ( -4, vec_elem( node->v, 0 ) );
  ASSERT_DOUBLE_EQ(  2, vec_elem( node->v, 1 ) );
  node = vec_list_node_next( node );
  ASSERT_DOUBLE_EQ( -4, vec_elem( node->v, 0 ) );
  ASSERT_DOUBLE_EQ(  0, vec_elem( node->v, 1 ) );
  node = vec_list_node_next( node );
  ASSERT_DOUBLE_EQ( -4, vec_elem( node->v, 0 ) );
  ASSERT_DOUBLE_EQ( -2, vec_elem( node->v, 1 ) );

  /* x = 4 */
  node = vec_list_node_next( node );
  ASSERT_DOUBLE_EQ(  4, vec_elem( node->v, 0 ) );
  ASSERT_DOUBLE_EQ( -4, vec_elem( node->v, 1 ) );
  node = vec_list_node_next( node );
  ASSERT_DOUBLE_EQ(  4, vec_elem( node->v, 0 ) );
  ASSERT_DOUBLE_EQ( -2, vec_elem( node->v, 1 ) );
  node = vec_list_node_next( node );
  ASSERT_DOUBLE_EQ( 4, vec_elem( node->v, 0 ) );
  ASSERT_DOUBLE_EQ( 0, vec_elem( node->v, 1 ) );
  node = vec_list_node_next( node );
  ASSERT_DOUBLE_EQ( 4, vec_elem( node->v, 0 ) );
  ASSERT_DOUBLE_EQ( 2, vec_elem( node->v, 1 ) );

  vec_destroy( min );
  vec_destroy( max );
}

/* TEST(test_phase_portrait_plotter_generate_edge_points2) */
/* { */
/*   vec_t min = vec_create_list( 3, -4.0, -4.0, -4.0 ); */
/*   vec_t max = vec_create_list( 3,  4.0,  4.0,  4.0 ); */
/*   int n_sc[3] = { 4, 4, 4 }; */
/*   vec_list_node_t *node; */

/*   ppp_set_lim( &ppp, min, max ); */
/*   ppp_set_n_sc( &ppp, n_sc ); */
/*   ppp_generate_edge_points( &ppp ); */

/*   node = ppp_p0_list_tail(&ppp); */
/*   printf( "num: %ld\n", vec_list_num( ppp_p0_list(&ppp) ) ); */
/*   for( ; node != vec_list_root( ppp_p0_list(&ppp) ); node = vec_list_node_next(node) ){ */
/*     vec_write( node->v ); */
/*   } */

/*   vec_destroy( min ); */
/*   vec_destroy( max ); */
/* } */

/* TEST(test_phase_portrait_plotter_) */
/* { */
/* } */

TEST_SUITE(test_phase_portrait_plotter)
{
  CONFIGURE_SUITE( setup, teardown );
  RUN_TEST(test_phase_portrait_plotter_init);
  RUN_TEST(test_phase_portrait_plotter_destroy);
  RUN_TEST(test_phase_portrait_plotter_set_lim);
  RUN_TEST(test_phase_portrait_plotter_set_lim_xy);
  RUN_TEST(test_phase_portrait_plotter_set_n_sc);
  RUN_TEST(test_phase_portrait_plotter_push_p0);
  RUN_TEST(test_phase_portrait_plotter_generate_edge_points);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_phase_portrait_plotter);
  TEST_REPORT();
  TEST_EXIT();
}
