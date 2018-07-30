#include "rhc_ctrl_regulator.h"
#include "rhc_test.h"

static cmd_t cmd;
static model_t model;
static ctrl_t ctrl;
static vec_t p;

void setup()
{
  model_init( &model, 1 );
  ctrl_regulator_create( &ctrl, &cmd, &model );
  ctrl_regulator_cmd_init( &ctrl, &cmd );
  p = vec_create( 2 );
}

void teardown()
{
  vec_destroy( p );
  ctrl_destroy( &ctrl );
  cmd_destroy( &cmd );
}

TEST(test_ctrl_regulator_cmd_init)
{
  ctrl_regulator_cmd_init( &ctrl, &cmd );
  ASSERT_EQ( 1.0, ctrl_regulator_q1( &ctrl ) );
  ASSERT_EQ( 1.0, ctrl_regulator_q2( &ctrl ) );
}

TEST(test_ctrl_regulator_create)
{
  ASSERT_PTREQ( ctrl_regulator_update, ctrl._update );
  ASSERT_PTREQ( ctrl_regulator_destroy, ctrl._destroy );
  ASSERT_PTREQ( ctrl_regulator_header, ctrl._header );
  ASSERT_PTREQ( ctrl_regulator_writer, ctrl._writer );
  ASSERT_EQ( 0, ((ctrl_regulator_prp*)ctrl.prp)->xi );
  ASSERT_EQ( 1, ctrl_regulator_q1(&ctrl) );
  ASSERT_EQ( 1, ctrl_regulator_q2(&ctrl) );
}

TEST(test_ctrl_regulator_destroy)
{
  ctrl_regulator_destroy( &ctrl );
  ASSERT_PTREQ( NULL, ctrl_cmd( &ctrl ) );
  ASSERT_PTREQ( NULL, ctrl.prp );
}

TEST(test_ctrl_regulator_calc_sqr_xi)
{
  struct case_t{
    double z0;
    double expected;
  } cases[] = {
    { 1, G }, { G, 1 }, { 0.5*G, 2 }, { 0, 0 }
  };
  struct case_t *c;
  for( c=cases; c->z0>0; c++ ){
    ASSERT_NEAR( c->expected, ctrl_regulator_calc_sqr_xi(c->z0), 1e-10 );
  }
}

TEST(test_ctrl_regulator_calc_xi)
{
  struct case_t{
    double z0;
    double expected;
  } cases[] = {
    { 1, sqrt(G) }, { G, 1 }, { 0.5*G, sqrt(2) }, { 0, 0 }
  };
  struct case_t *c;
  for( c=cases; c->z0>0; c++ ){
    ASSERT_NEAR( c->expected, ctrl_regulator_calc_xi(c->z0), 1e-10 );
  }
}

TEST(test_ctrl_regulator_calc_fz)
{
  struct case_t{
    double m, z0, q1, q2;
    double z, v;
    double expected;
  } cases[] = {
    { 1, 0.26, 1, 1, 0.24, 0, G/13 + G },
    { 1, 0.26, 1, 2, 0.24, 0, 2*G/13 + G },
    { 1, 0.26, 1, 1, 0.26, -0.2*sqrt(G), 4*G/sqrt(26) + G },
    { 2, 0.26, 1, 1, 0.26, -0.2*sqrt(G), 8*G/sqrt(26) + 2*G },
    { 0, 0, 0, 0, 0, 0, 0 }
  };
  struct case_t *c;

  for( c=cases; c->m>0; c++ ){
    model_set_mass( &model, c->m );
    cmd.z0 = c->z0;
    cmd.regulator.q1 = c->q1;
    cmd.regulator.q2 = c->q2;
    vec_set_elem_list( p, c->z, c->v );
    ASSERT_NEAR( c->expected, ctrl_regulator_calc_fz( &ctrl, p ), 1e-10 );
  }
}

TEST(test_ctrl_regulator_update)
{
  struct case_t{
    double z, v;
    double expected_fz;
  } cases[] = {
    { 0.26, -0.2*sqrt(G), 4*G/sqrt(26) + G },  /* touchdown */
    { 0.24, 0, G/13 + G },                     /* bottom */
    { 0.28, 0, 0     },                        /* apex */
    { 0, 0, 0 }
  };
  struct case_t *c;
  vec_t p;

  p = vec_create( 2 );
  ctrl_regulator_q1( &ctrl ) = 1;
  ctrl_regulator_q2( &ctrl ) = 1;
  for( c=cases; c->z>0; c++ ) {
    vec_set_elem_list( p, c->z, c->v );
    ctrl_update( &ctrl, 0, p );
    ASSERT_NEAR( c->expected_fz, ctrl_fz(&ctrl), 1e-10 );
  }
}

TEST_SUITE(test_ctrl_regulator)
{
  CONFIGURE_SUITE( setup, teardown );
  RUN_TEST(test_ctrl_regulator_cmd_init);
  RUN_TEST(test_ctrl_regulator_create);
  RUN_TEST(test_ctrl_regulator_destroy);
  RUN_TEST(test_ctrl_regulator_calc_sqr_xi);
  RUN_TEST(test_ctrl_regulator_calc_xi);
  RUN_TEST(test_ctrl_regulator_calc_fz);
  RUN_TEST(test_ctrl_regulator_update);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_ctrl_regulator);
  TEST_REPORT();
  TEST_EXIT();
}
