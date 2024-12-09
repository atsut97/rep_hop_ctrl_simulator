#include "rhc_ctrl_dummy.h"
#include "rhc_test.h"

static cmd_t cmd;
static model_t model;
static ctrl_t ctrl;
static vec_t p;
static double k = 10;

void setup()
{
  cmd_default_init( &cmd );
  model_init( &model, 10 );
  ctrl_dummy_create( &ctrl, &cmd, &model, k );
  p = vec_create( 2 );
}

void teardown()
{
  vec_destroy( p );
  ctrl_destroy( &ctrl );
  cmd_destroy( &cmd );
}

TEST(test_ctrl_dummy_create)
{
  ASSERT_EQ( 10, ((ctrl_dummy_prp*)ctrl.prp)->k );
}

TEST(test_ctrl_dummy_update)
{
  ctrl_update( &ctrl, 0, p );
  ASSERT_EQ( 10, ctrl_fz( &ctrl ) );
}

TEST(test_ctrl_dummy_update_phase)
{
  struct case_t {
    double za, zh, zb;
    double z, v;
    int expct_n;
    double expct_phi;
  } cases[] = {
    { 0.28, 0.26, 0.24, 0.28, 0.0, 0, 0.0 },            /* top */
    { 0.28, 0.26, 0.24, 0.26, -sqrt(0.04*G), 0, PI_2 }, /* touchdown */
    { 0.28, 0.26, 0.24, 0.24, -0.0, 0, PI },            /* bottom */
    { 0.28, 0.26, 0.24, 0.24, 0.0, 0, -PI },            /* bottom */
    { 0.28, 0.26, 0.24, 0.26, sqrt(0.04*G), 0, -PI_2 }, /* lift-off */
    { 0.28, 0.26, 0.24, 0.28, 0.0, 1, 0.0 },            /* top */
    { 0.28, 0.26, 0.24, 0.26, -sqrt(0.04*G), 1, PI_2 }, /* touchdown */
    { 0.28, 0.26, 0.24, 0.24, -0.0, 1, PI },            /* bottom */
    { 0.28, 0.26, 0.24, 0.24, 0.0, 1, -PI },            /* bottom */
    { 0.28, 0.26, 0.24, 0.26, sqrt(0.04*G), 1, -PI_2 }, /* lift-off */
    { 0.28, 0.26, 0.24, 0.28, 0.0, 2, 0.0 },            /* top */
    { 0, 0, 0, 0, 0, 0, 0 },
  };
  struct case_t *c;
  double t;

  t = 0;
  for( c=cases; c->za>0; c++ ){
    vec_set_elem_list( p, 2, c->z, c->v );
    ctrl_update( &ctrl, t, p );
    ASSERT_DOUBLE_EQ( c->expct_n, ctrl_n( &ctrl ) );
    ASSERT_DOUBLE_EQ( c->expct_phi, ctrl_phi( &ctrl ) );
    t += 0.01;
  }
}

TEST(test_ctrl_dummy_update_flight_zero)
{
  struct case_t {
    double za, zh, zb;
    double z, v;
    double expct_fz;
  } cases[] = {
    { 0.28, 0.26, 0.24, 0.28, 0.0, 0.0 },          /* top */
    { 0.28, 0.26, 0.24, 0.26, -sqrt(0.04*G), 10 }, /* touchdown */
    { 0.28, 0.26, 0.24, 0.24, 0.0, 10 },           /* bottom */
    { 0.28, 0.26, 0.24, 0.26, sqrt(0.04*G), 0.0 }, /* lift-off */
    { 0.28, 0.26, 0.24, 0.28, 0.0, 0.0 },          /* top */
    { 0.28, 0.26, 0.24, 0.26, -sqrt(0.04*G), 10 }, /* touchdown */
    { 0.28, 0.26, 0.24, 0.24, 0.0, 10 },           /* bottom */
    { 0.28, 0.26, 0.24, 0.26, sqrt(0.04*G), 0.0 }, /* lift-off */
    { 0.28, 0.26, 0.24, 0.28, 0.0, 0.0 },          /* top */
    { 0, 0, 0, 0, 0, 0 },
  };
  struct case_t *c;
  double t;

  t = 0;
  for( c=cases; c->za>0; c++ ){
    vec_set_elem_list( p, 2, c->z, c->v );
    ctrl_update( &ctrl, t, p );
    ASSERT_DOUBLE_EQ( c->expct_fz, ctrl_fz( &ctrl ) );
    t += 0.01;
  }
}

TEST(test_ctrl_dummy_destroy)
{
  ASSERT_EQ( 10, ((ctrl_dummy_prp*)ctrl.prp)->k );
  ctrl_dummy_destroy( &ctrl );
  ASSERT_PTREQ( NULL, ctrl_cmd( &ctrl ) );
  ASSERT_PTREQ( NULL, ctrl.prp );
}

TEST_SUITE(test_ctrl_dummy)
{
  CONFIGURE_SUITE( setup, teardown );
  RUN_TEST(test_ctrl_dummy_create);
  RUN_TEST(test_ctrl_dummy_update);
  RUN_TEST(test_ctrl_dummy_update_phase);
  RUN_TEST(test_ctrl_dummy_update_flight_zero);
  RUN_TEST(test_ctrl_dummy_destroy);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_ctrl_dummy);
  TEST_REPORT();
  TEST_EXIT();
}
