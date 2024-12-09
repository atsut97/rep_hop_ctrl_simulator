#include "rhc_ctrl.h"
#include "rhc_ctrl_arl.h"
#include "rhc_test.h"

static cmd_t cmd;
static model_t model;
static ctrl_t ctrl;
static vec_t p;

void setup() {
  cmd_default_init( &cmd );
  model_init( &model, 10 );
  ctrl_arl_create( &ctrl, &cmd, &model, none );
  p = vec_create( 2 );
}

void teardown() {
  vec_destroy( p );
  ctrl_destroy( &ctrl );
  cmd_destroy( &cmd );
}

TEST(test_ctrl_arl_create)
{
  ASSERT_PTREQ( ctrl_cmd( &ctrl ), &cmd );
  ASSERT_PTREQ( ctrl_model( &ctrl ), &model );
  ASSERT_PTREQ( ctrl_arl_update, ctrl._update );
  ASSERT_PTREQ( ctrl_arl_destroy, ctrl._destroy );
  ASSERT_PTREQ( ctrl_arl_header, ctrl._header );
  ASSERT_PTREQ( ctrl_arl_writer, ctrl._writer );
  ASSERT_EQ( 0, ctrl_arl_k(&ctrl) );
  ASSERT_EQ( 0, ctrl_arl_beta(&ctrl) );
  ASSERT_PTRNE( NULL, ctrl.prp );
  ASSERT_EQ( none, ((ctrl_arl_prp*)ctrl.prp)->type );
  ASSERT_EQ( 0, ((ctrl_arl_prp*)ctrl.prp)->sqr_R_des );
  ASSERT_EQ( 0, ((ctrl_arl_prp*)ctrl.prp)->sqr_R );
  ASSERT_EQ( 0, ((ctrl_arl_prp*)ctrl.prp)->delta );
}

TEST(test_ctrl_arl_destroy)
{
  ctrl_arl_destroy( &ctrl );
  ASSERT_PTREQ( NULL, ctrl_cmd( &ctrl ) );
  ASSERT_PTREQ( NULL, ctrl_model( &ctrl ) );
  ASSERT_PTREQ( NULL, ctrl.prp );
  ASSERT_EQ( 0, ctrl_n( &ctrl ) );
  ASSERT_EQ( 0, ctrl_phi( &ctrl ) );
}

TEST(test_ctrl_arl_set_k)
{
  double k = 10.0;
  ctrl_arl_set_k( &ctrl, k );
  ASSERT_EQ( k, ctrl_arl_k(&ctrl) );
}

TEST(test_ctrl_arl_set_beta)
{
  double beta = 500.0;
  ctrl_arl_set_beta( &ctrl, beta );
  ASSERT_EQ( beta, ctrl_arl_beta(&ctrl) );
}

TEST(test_ctrl_arl_set_params)
{
  struct case_t {
    double k, beta;
  } cases[] = {
    { 82.8, 73.8 },
    { 31.0, 15.1 },
    { 30.2, 89.9 },
    { 0, 0 },
  };
  struct case_t *c;
  ctrl_t local_ctrl;

  for ( c=cases; c->k>0; c++ ){
    ctrl_arl_create( &local_ctrl, &cmd, &model, none );
    ctrl_arl_set_params( &local_ctrl, c->k, c->beta );
    ASSERT_EQ( c->k, ctrl_arl_k(&local_ctrl) );
    ASSERT_EQ( c->beta, ctrl_arl_beta(&local_ctrl) );
    ctrl_arl_destroy( &local_ctrl );
  }
}

TEST(test_ctrl_arl_calc_sqr_R_des)
{
  struct case_t {
    double m, k, zh, za;
    double expected_sqr_R_des;
  } cases[] = {
    { 1.0, G, 0.26, 0.28, 1.0804 },
    { 1.0, G, 0.25, 0.28, 1.1209 },
    { 1.0, G, 0.25, 0.29, 1.1616 },
    { 0.0, 0.0, 0.0, 0.0, 0.0 },
  };
  struct case_t *c;
  double sqr_R_des;

  for( c=cases; c->m>0; c++ ){
    sqr_R_des = ctrl_arl_calc_sqr_R_des( c->m, c->k, c->zh, c->za );
    ASSERT_NEAR( c->expected_sqr_R_des, sqr_R_des, 1e-10 );
  }
}

TEST(test_ctrl_arl_calc_sqr_R)
{
  struct case_t {
    double m, k, z, v, zh;
    double expected_sqr_R;
  } cases[] = {
    { 1.0, G, 0.24,     0.0, 0.26, 0.9604 }, /* bottom */
    { 1.0, G, 0.26, sqrt(G), 0.26, 2.0 },    /* lift-off */
    { 1.0, G, 0.26,-sqrt(G), 0.26, 2.0 },    /* touch-down */
    { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
  };
  struct case_t *c;
  double sqr_R;

  for( c=cases; c->m>0; c++ ){
    vec_set_elem_list( p, c->z, c->v );
    sqr_R = ctrl_arl_calc_sqr_R( p, c->m, c->k, c->zh );
    ASSERT_NEAR( c->expected_sqr_R, sqr_R, 1e-10 );
  }
}

TEST(test_ctrl_arl_calc_delta)
{
  struct case_t {
    double m, k, z, v, beta, zh, za;
    double expected_delta;
  } cases[] = {
    { 1.0, G, 0.24,     0.0, 1.0, 0.26, 0.28, 0.0 }, /* bottom */
    { 1.0, G, 0.26, sqrt(G), 1.5, 0.26, 0.28, 0.0 },    /* lift-off */
    { 1.0, G, 0.26,-sqrt(G), 2.0, 0.26, 0.28, 0.0 },    /* touch-down */
    { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
  };
  struct case_t *c;
  double delta;

  for( c=cases; c->m>0; c++ ){
    vec_set_elem_list( p, c->z, c->v );
    delta = ctrl_arl_calc_delta( p, c->m, c->k, c->beta, c->zh, c->za );
    ASSERT_NEAR( c->expected_delta, delta, 1e-10 );
  }
}

TEST_SUITE(test_ctrl_arl)
{
  CONFIGURE_SUITE(setup, teardown);
  RUN_TEST(test_ctrl_arl_create);
  RUN_TEST(test_ctrl_arl_destroy);
  RUN_TEST(test_ctrl_arl_set_k);
  RUN_TEST(test_ctrl_arl_set_beta);
  RUN_TEST(test_ctrl_arl_set_params);
  RUN_TEST(test_ctrl_arl_calc_sqr_R_des);
  RUN_TEST(test_ctrl_arl_calc_sqr_R);
  RUN_TEST(test_ctrl_arl_calc_delta);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_ctrl_arl);
  TEST_REPORT();
  TEST_EXIT();
}
