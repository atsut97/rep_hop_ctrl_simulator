#include "rhc_ctrl_slip.h"
#include "rhc_test.h"

static cmd_t cmd;
static model_t model;
static ctrl_t ctrl;
static vec_t p;

void setup()
{
  cmd_default_init( &cmd );
  model_init( &model, 10 );
  ctrl_slip_create( &ctrl, &cmd, &model );
  p = vec_create( 2 );
}

void teardown()
{
  vec_destroy( p );
  ctrl_destroy( &ctrl );
  cmd_destroy( &cmd );
}

TEST(test_ctrl_slip_create)
{
  ASSERT_PTREQ( ctrl_slip_update, ctrl._update );
  ASSERT_PTREQ( ctrl_slip_destroy, ctrl._destroy );
  ASSERT_PTREQ( ctrl_slip_header, ctrl._header );
  ASSERT_PTREQ( ctrl_slip_writer, ctrl._writer );
  ASSERT_EQ( 0, ((ctrl_slip_prp*)ctrl.prp)->k );
}

TEST(test_ctrl_slip_k)
{
  ctrl_slip_prp *prp;

  prp = ctrl.prp;
  prp->k = 1000;
  ASSERT_EQ( 1000, ctrl_slip_k( &ctrl ) );
}

TEST(test_ctrl_slip_update)
{
  /* ctrl_update( &ctrl, 0, p ); */
  /* ASSERT_EQ( 10, ctrl_fz( &ctrl ) ); */
  /* FAIL( "intentionally" ); */
}

TEST(test_ctrl_slip_update_phase)
{
  struct case_t {
    double za, z0, zb;
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

TEST(test_ctrl_slip_update_flight_zero)
{
  struct case_t {
    double za, z0, zb;
    double z, v;
    double expct_fz;
  } cases[] = {
    { 0.28, 0.26, 0.24, 0.28, 0.0, 0.0 },          /* top */
    { 0.28, 0.26, 0.24, 0.26, -sqrt(0.04*G), 0 },  /* touchdown */
    { 0.28, 0.26, 0.24, 0.24, 0.0, 10 },           /* bottom */
    { 0.28, 0.26, 0.24, 0.26, sqrt(0.04*G), 0 },   /* lift-off */
    { 0.28, 0.26, 0.24, 0.28, 0.0, 0.0 },          /* top */
    { 0.28, 0.26, 0.24, 0.26, -sqrt(0.04*G), 0 },  /* touchdown */
    { 0.28, 0.26, 0.24, 0.24, 0.0, 10 },           /* bottom */
    { 0.28, 0.26, 0.24, 0.26, sqrt(0.04*G), 0 },   /* lift-off */
    { 0.28, 0.26, 0.24, 0.28, 0.0, 0.0 },          /* top */
    { 0, 0, 0, 0, 0, 0 },
  };
  struct case_t *c;
  double t;

  t = 0;
  for( c=cases; c->za>0; c++ ){
    vec_set_elem_list( p, 2, c->z, c->v );
    ctrl_update( &ctrl, t, p );
    if( c->expct_fz == 0 )
      ASSERT_DOUBLE_EQ( c->expct_fz, ctrl_fz( &ctrl ) );
    else
      ASSERT_LT( 0, ctrl_fz( &ctrl ) );
    t += 0.01;
  }
}

TEST(test_ctrl_slip_destroy)
{
  ASSERT_EQ( 0, ((ctrl_slip_prp*)ctrl.prp)->k );
  ctrl_slip_destroy( &ctrl );
  ASSERT_PTREQ( NULL, ctrl_cmd( &ctrl ) );
  ASSERT_PTREQ( NULL, ctrl.prp );
}

TEST(test_ctrl_slip_calc_stiffness)
{
  struct case_t{
    double m, z0, za, zb;
    double expected;
  } cases[] = {
    { 1, 0.26, 0.28, 0.24, 200*G },
    { 2, 0.26, 0.28, 0.24, 400*G },
    { 0.3, 0.27, 0.30, 0.24, 40*G },
    { 0, 0, 0, 0, 0 }
  };
  struct case_t *c;
  double (*f)(double,double,double,double);

  f = ctrl_slip_calc_stiffness;
  for( c=cases; c->m>0; c++ ){
    ASSERT_NEAR( c->expected, f( c->m, c->z0, c->za, c->zb ), 1e-10 );
  }
}

TEST(test_ctrl_slip_stiffness)
{
  struct case_t{
    double m, z0, za, zb;
    double expected;
  } cases[] = {
    { 1, 0.26, 0.28, 0.24, 200*G },
    { 2, 0.26, 0.28, 0.24, 400*G },
    { 0.3, 0.27, 0.30, 0.24, 40*G },
    { 0, 0, 0, 0, 0 }
  };
  struct case_t *c;

  for( c=cases; c->m>0; c++ ){
    model_set_mass( &model, c->m );
    cmd.z0 = c->z0;
    cmd.za = c->za;
    cmd.zb = c->zb;
    ASSERT_NEAR( c->expected, ctrl_slip_stiffness( &ctrl ), 1e-10 );
  }
}

TEST_SUITE(test_ctrl_slip)
{
  CONFIGURE_SUITE( setup, teardown );
  RUN_TEST(test_ctrl_slip_create);
  RUN_TEST(test_ctrl_slip_k);
  RUN_TEST(test_ctrl_slip_update);
  RUN_TEST(test_ctrl_slip_update_phase);
  RUN_TEST(test_ctrl_slip_update_flight_zero);
  RUN_TEST(test_ctrl_slip_destroy);
  RUN_TEST(test_ctrl_slip_calc_stiffness);
  RUN_TEST(test_ctrl_slip_stiffness);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_ctrl_slip);
  TEST_REPORT();
  TEST_EXIT();
}
