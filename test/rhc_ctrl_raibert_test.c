#include "rhc_cmd.h"
#include "rhc_ctrl.h"
#include "rhc_ctrl_raibert.h"
#include "rhc_model.h"
#include "rhc_test.h"
#include "rhc_vec.h"

static cmd_t cmd;
static model_t model;
static ctrl_t ctrl;
static vec_t p;

void setup()
{
  cmd_default_init( &cmd );
  model_init( &model, 10 );
  p = vec_create( 2 );
}

void teardown()
{
  vec_destroy( p );
  ctrl_destroy( &ctrl );
  cmd_destroy( &cmd );
}

TEST(test_ctrl_raibert_create)
{
  ctrl_raibert_create( &ctrl, &cmd, &model, none );
  ASSERT_PTREQ( ctrl_cmd( &ctrl ), &cmd );
  ASSERT_PTREQ( ctrl_model( &ctrl ), &model );
  ASSERT_PTREQ( ctrl_raibert_update, ctrl._update );
  ASSERT_PTREQ( ctrl_raibert_destroy, ctrl._destroy );
  ASSERT_PTREQ( ctrl_raibert_header, ctrl._header );
  ASSERT_PTREQ( ctrl_raibert_writer, ctrl._writer );
  ASSERT_EQ( 0, ctrl_raibert_delta(&ctrl) );
  ASSERT_EQ( 0, ctrl_raibert_tau(&ctrl) );
  ASSERT_EQ( 0, ctrl_raibert_gamma(&ctrl) );
  ASSERT_EQ( 0, ctrl_raibert_yeta1(&ctrl) );
  ASSERT_EQ( 0, ctrl_raibert_zr(&ctrl) );
  ASSERT_EQ( 0, ctrl_raibert_mu(&ctrl) );
  ASSERT_PTRNE( NULL, ctrl.prp );
  ASSERT_EQ( none, ((ctrl_raibert_prp*)ctrl.prp)->type );
  ASSERT_EQ( 0, ((ctrl_raibert_prp*)ctrl.prp)->end_of_thrust.t );
  ASSERT_EQ( 0, ((ctrl_raibert_prp*)ctrl.prp)->end_of_thrust.z );
  ASSERT_EQ( 0, ((ctrl_raibert_prp*)ctrl.prp)->end_of_thrust.v );
  ASSERT_FALSE( ((ctrl_raibert_prp*)ctrl.prp)->is_in_thrust );
}

TEST(test_ctrl_raibert_destroy)
{
  ctrl_raibert_create( &ctrl, &cmd, &model, none );
  ctrl_raibert_destroy( &ctrl );
  ASSERT_PTREQ( NULL, ctrl_cmd( &ctrl ) );
  ASSERT_PTREQ( NULL, ctrl_model( &ctrl ) );
  ASSERT_PTREQ( NULL, ctrl.prp );
  ASSERT_EQ( 0, ctrl_n( &ctrl ) );
  ASSERT_EQ( 0, ctrl_phi( &ctrl ) );
}

TEST(test_ctrl_raibert_set_delta)
{
  double delta = 14.0;
  ctrl_raibert_create( &ctrl, &cmd, &model, none );
  ctrl_raibert_set_delta( &ctrl, delta );
  ASSERT_EQ( delta, ctrl_raibert_delta(&ctrl) );
}

TEST(test_ctrl_raibert_set_tau)
{
  double tau = 96.1;
  ctrl_raibert_create( &ctrl, &cmd, &model, none );
  ctrl_raibert_set_tau( &ctrl, tau );
  ASSERT_EQ( tau, ctrl_raibert_tau(&ctrl) );
}

TEST(test_ctrl_raibert_set_gamma)
{
  double gamma = 44.2;
  ctrl_raibert_create( &ctrl, &cmd, &model, none );
  ctrl_raibert_set_gamma( &ctrl, gamma );
  ASSERT_EQ( gamma, ctrl_raibert_gamma(&ctrl) );
}

TEST(test_ctrl_raibert_set_yeta1)
{
  double yeta1 = 43.6;
  ctrl_raibert_create( &ctrl, &cmd, &model, none );
  ctrl_raibert_set_yeta1( &ctrl, yeta1 );
  ASSERT_EQ( yeta1, ctrl_raibert_yeta1(&ctrl) );
}

TEST(test_ctrl_raibert_set_zr)
{
  double zr = 2.2;
  ctrl_raibert_create( &ctrl, &cmd, &model, none );
  ctrl_raibert_set_zr( &ctrl, zr );
  ASSERT_EQ( zr, ctrl_raibert_zr(&ctrl) );
}

TEST(test_ctrl_raibert_set_mu)
{
  double mu = 0.3;
  ctrl_raibert_create( &ctrl, &cmd, &model, none );
  ctrl_raibert_set_mu( &ctrl, mu );
  ASSERT_EQ( mu, ctrl_raibert_mu(&ctrl) );
}

TEST(test_ctrl_raibert_set_params)
{
  struct case_t {
    double delta, tau, gamma, yeta1, zr, mu;
  } cases[] = {
    { 69.6, 84.6, 62.8, 69.9, 17.9, 41.3 },
    { 63.9, 57.7, 83.6, 15.4, 79.8, 92.0 },
    { 24.4, 94.2, 68.0, 4.8, 39.6, 11.8 },
    { 0, 0, 0, 0, 0, 0 },
  };
  struct case_t *c;

  for ( c=cases; c->delta>0; c++ ){
    ctrl_raibert_create( &ctrl, &cmd, &model, none );
    ctrl_raibert_set_params( &ctrl, c->delta, c->tau, c->gamma, c->yeta1, c->zr, c->mu );
    ASSERT_EQ( c->delta, ctrl_raibert_delta(&ctrl) );
    ASSERT_EQ( c->tau, ctrl_raibert_tau(&ctrl) );
    ASSERT_EQ( c->gamma, ctrl_raibert_gamma(&ctrl) );
    ASSERT_EQ( c->yeta1, ctrl_raibert_yeta1(&ctrl) );
    ASSERT_EQ( c->zr, ctrl_raibert_zr(&ctrl) );
    ASSERT_EQ( c->mu, ctrl_raibert_mu(&ctrl) );
    ctrl_raibert_destroy( &ctrl );
  }
}

TEST(test_ctrl_raibert_set_params_full_nonlinear)
{
  struct case_t {
    double delta, tau, gamma, yeta1, zr, mu;
  } cases[] = {
    { 91.5, 22.7, 30.3, 7.2, 12.0, 76.8 },
    { 94.2, 99.2, 53.4, 79.4, 70.6, 47.0 },
    { 84.8, 72.3, 94.1, 68.7, 96.4, 48.8 },
    { 0, 0, 0, 0, 0, 0 },
  };
  struct case_t *c;

  for ( c=cases; c->delta>0; c++ ){
    ctrl_raibert_create( &ctrl, &cmd, &model, none );
    ctrl_raibert_set_params_full_nonlinear( &ctrl, c->delta, c->tau, c->gamma, c->yeta1, c->mu );
    ASSERT_EQ( c->delta, ctrl_raibert_delta(&ctrl) );
    ASSERT_EQ( c->tau, ctrl_raibert_tau(&ctrl) );
    ASSERT_EQ( c->gamma, ctrl_raibert_gamma(&ctrl) );
    ASSERT_EQ( c->yeta1, ctrl_raibert_yeta1(&ctrl) );
    ASSERT_EQ( 0, ctrl_raibert_zr(&ctrl) );
    ASSERT_EQ( c->mu, ctrl_raibert_mu(&ctrl) );
    ctrl_raibert_destroy( &ctrl );
  }
}

TEST(test_ctrl_raibert_set_params_simplified_nonlinear)
{
  struct case_t {
    double delta, tau, gamma, yeta1, zr, mu;
  } cases[] = {
    { 43.3, 93.4, 28.3, 12.2, 78.4, 73.0 },
    { 18.9, 10.2, 96.5, 19.9, 26.1, 88.7 },
    { 70.9, 92.2, 15.5, 76.7, 96.6, 52.3 },
    { 0, 0, 0, 0, 0, 0 },
  };
  struct case_t *c;

  for ( c=cases; c->delta>0; c++ ){
    ctrl_raibert_create( &ctrl, &cmd, &model, none );
    ctrl_raibert_set_params_simplified_nonlinear( &ctrl, c->tau, c->yeta1 );
    ASSERT_EQ( 0, ctrl_raibert_delta(&ctrl) );
    ASSERT_EQ( c->tau, ctrl_raibert_tau(&ctrl) );
    ASSERT_EQ( 0, ctrl_raibert_gamma(&ctrl) );
    ASSERT_EQ( c->yeta1, ctrl_raibert_yeta1(&ctrl) );
    ASSERT_EQ( 0, ctrl_raibert_zr(&ctrl) );
    ASSERT_EQ( 0, ctrl_raibert_mu(&ctrl) );
    ctrl_raibert_destroy( &ctrl );
  }
}

TEST(test_ctrl_raibert_set_params_full_linear)
{
  struct case_t {
    double delta, tau, gamma, yeta1, zr, mu;
  } cases[] = {
    { 75.4, 60.0, 32.7, 27.7, 45.4, 24.5 },
    { 17.0, 7.3, 24.7, 35.6, 15.8, 11.7 },
    { 58.2, 66.7, 43.5, 38.5, 65.4, 81.1 },
    { 0, 0, 0, 0, 0, 0 },
  };
  struct case_t *c;

  for ( c=cases; c->delta>0; c++ ){
    ctrl_raibert_create( &ctrl, &cmd, &model, none );
    ctrl_raibert_set_params_full_linear( &ctrl, c->delta, c->tau, c->gamma, c->yeta1, c->zr, c->mu );
    ASSERT_EQ( c->delta, ctrl_raibert_delta(&ctrl) );
    ASSERT_EQ( c->tau, ctrl_raibert_tau(&ctrl) );
    ASSERT_EQ( c->gamma, ctrl_raibert_gamma(&ctrl) );
    ASSERT_EQ( c->yeta1, ctrl_raibert_yeta1(&ctrl) );
    ASSERT_EQ( c->zr, ctrl_raibert_zr(&ctrl) );
    ASSERT_EQ( c->mu, ctrl_raibert_mu(&ctrl) );
    ctrl_raibert_destroy( &ctrl );
  }
}

TEST(test_ctrl_raibert_set_params_simplified_linear)
{
  struct case_t {
    double delta, tau, gamma, yeta1, zr, mu;
  } cases[] = {
    { 52.7, 38.0, 84.0, 72.6, 17.2, 55.5 },
    { 10.8, 69.9, 0.3, 18.6, 56.2, 7.8 },
    { 4.8, 26.9, 2.6, 91.2, 45.9, 17.4 },
    { 0, 0, 0, 0, 0, 0 },
  };
  struct case_t *c;

  for ( c=cases; c->delta>0; c++ ){
    ctrl_raibert_create( &ctrl, &cmd, &model, none );
    ctrl_raibert_set_params_simplified_linear( &ctrl, c->delta, c->tau, c->gamma, c->yeta1, c->zr );
    ASSERT_EQ( c->delta, ctrl_raibert_delta(&ctrl) );
    ASSERT_EQ( c->tau, ctrl_raibert_tau(&ctrl) );
    ASSERT_EQ( c->gamma, ctrl_raibert_gamma(&ctrl) );
    ASSERT_EQ( c->yeta1, ctrl_raibert_yeta1(&ctrl) );
    ASSERT_EQ( c->zr, ctrl_raibert_zr(&ctrl) );
    ASSERT_EQ( 0, ctrl_raibert_mu(&ctrl) );
    ctrl_raibert_destroy( &ctrl );
  }
}

TEST(test_ctrl_raibert_create_full_nonlinear)
{
  struct case_t {
    double delta, tau, gamma, yeta1, zr, mu;
  } cases[] = {
    { 73.6, 90.5, 61.4, 8.7, 35.2, 3.4 },
    { 14.6, 43.4, 47.5, 62.2, 40.1, 47.8 },
    { 20.6, 95.4, 64.2, 61.4, 88.4, 69.3 },
    { 0, 0, 0, 0, 0, 0 },
  };
  struct case_t *c;

  for ( c=cases; c->tau>0; c++ ){
    ctrl_raibert_create_full_nonlinear( &ctrl, &cmd, &model, c->delta, c->tau, c->gamma, c->yeta1, c->mu );
    ASSERT_EQ( full_nonlinear, ctrl_raibert_type( &ctrl ) );
    ASSERT_EQ( c->delta, ctrl_raibert_delta(&ctrl) );
    ASSERT_EQ( c->tau, ctrl_raibert_tau(&ctrl) );
    ASSERT_EQ( c->gamma, ctrl_raibert_gamma(&ctrl) );
    ASSERT_EQ( c->yeta1, ctrl_raibert_yeta1(&ctrl) );
    ASSERT_EQ( 0, ctrl_raibert_zr(&ctrl) );
    ASSERT_EQ( c->mu, ctrl_raibert_mu(&ctrl) );
    ctrl_raibert_destroy( &ctrl );
  }
}

TEST(test_ctrl_raibert_create_simplified_nonlinear)
{
  struct case_t {
    double delta, tau, gamma, yeta1, zr, mu;
  } cases[] = {
    { 54.5, 15.7, 52.3, 52.6, 66.9, 62.3 },
    { 66.9, 42.4, 59.2, 24.1, 45.4, 62.8 },
    { 40.3, 70.5, 10.0, 63.7, 89.9, 86.4 },
    { 0, 0, 0, 0, 0, 0 },
  };
  struct case_t *c;

  for ( c=cases; c->tau>0; c++ ){
    ctrl_raibert_create_simplified_nonlinear( &ctrl, &cmd, &model, c->tau, c->yeta1 );
    ASSERT_EQ( simplified_nonlinear, ctrl_raibert_type( &ctrl ) );
    ASSERT_EQ( 0, ctrl_raibert_delta(&ctrl) );
    ASSERT_EQ( c->tau, ctrl_raibert_tau(&ctrl) );
    ASSERT_EQ( 0, ctrl_raibert_gamma(&ctrl) );
    ASSERT_EQ( c->yeta1, ctrl_raibert_yeta1(&ctrl) );
    ASSERT_EQ( 0, ctrl_raibert_zr(&ctrl) );
    ASSERT_EQ( 0, ctrl_raibert_mu(&ctrl) );
    ctrl_raibert_destroy( &ctrl );
  }
}

TEST(test_ctrl_raibert_create_full_linear)
{
  struct case_t {
    double delta, tau, gamma, yeta1, zr, mu;
  } cases[] = {
    { 4.2, 26.7, 65.1, 16.7, 12.7, 20.9 },
    { 47.5, 80.1, 36.0, 71.3, 27.9, 36.3 },
    { 28.3, 84.1, 77.1, 41.3, 57.6, 90.0 },
    { 0, 0, 0, 0, 0, 0 },
  };
  struct case_t *c;

  for ( c=cases; c->tau>0; c++ ){
    ctrl_raibert_create_full_linear( &ctrl, &cmd, &model, c->delta, c->tau, c->gamma, c->yeta1, c->zr, c->mu );
    ASSERT_EQ( full_linear, ctrl_raibert_type( &ctrl ) );
    ASSERT_EQ( c->delta, ctrl_raibert_delta(&ctrl) );
    ASSERT_EQ( c->tau, ctrl_raibert_tau(&ctrl) );
    ASSERT_EQ( c->gamma, ctrl_raibert_gamma(&ctrl) );
    ASSERT_EQ( c->yeta1, ctrl_raibert_yeta1(&ctrl) );
    ASSERT_EQ( c->zr, ctrl_raibert_zr(&ctrl) );
    ASSERT_EQ( c->mu, ctrl_raibert_mu(&ctrl) );
    ctrl_raibert_destroy( &ctrl );
  }
}

TEST(test_ctrl_raibert_create_simplified_linear)
{
  struct case_t {
    double delta, tau, gamma, yeta1, zr, mu;
  } cases[] = {
    { 29.6, 68.0, 42.1, 44.6, 48.7, 80.6 },
    { 64.3, 51.1, 66.6, 22.2, 80.0, 86.4 },
    { 85.8, 48.9, 5.4, 24.1, 48.1, 34.3 },
    { 0, 0, 0, 0, 0, 0 },
  };
  struct case_t *c;

  for ( c=cases; c->tau>0; c++ ){
    ctrl_raibert_create_simplified_linear( &ctrl, &cmd, &model, c->delta, c->tau, c->gamma, c->yeta1, c->zr );
    ASSERT_EQ( simplified_linear, ctrl_raibert_type( &ctrl ) );
    ASSERT_EQ( c->delta, ctrl_raibert_delta(&ctrl) );
    ASSERT_EQ( c->tau, ctrl_raibert_tau(&ctrl) );
    ASSERT_EQ( c->gamma, ctrl_raibert_gamma(&ctrl) );
    ASSERT_EQ( c->yeta1, ctrl_raibert_yeta1(&ctrl) );
    ASSERT_EQ( c->zr, ctrl_raibert_zr(&ctrl) );
    ASSERT_EQ( 0, ctrl_raibert_mu(&ctrl) );
    ctrl_raibert_destroy( &ctrl );
  }
}

TEST(test_ctrl_raibert_is_in_thrust)
{
  struct case_t {
    double delta;
    double t, z, v;
    bool expected;
  } cases[] = {
    /* delta,  t,     z,     v,   exp */
    { 0.05, 0.00, 0.800,  0.00, false },  /* initial pos. */
    { 0.05, 0.20, 0.600, -1.98, false },  /* falling */
    { 0.05, 0.25, 0.500, -2.43, false },  /* touchdown */
    { 0.05, 0.40, 0.190,  0.00, true  },  /* bottom */
    { 0.05, 0.42, 0.194,  0.30, true  },  /* thrust */
    { 0.05, 0.44, 0.198,  0.60, true  },  /* thrust */
    { 0.05, 0.45, 0.200,  1.00, false },  /* end of thrust */
    { 0.05, 0.50, 0.400,  3.00, false },  /* extension */
    { 0.05, 0.55, 0.500,  4.00, false },  /* lift off */
    { 0, 0, 0, 0, false },
  };
  struct case_t *c;
  vec_t p;

  p = vec_create( 2 );
  cmd.zd = 0.8;
  cmd.z0 = 0.5;
  cmd.zb = 0.19;
  ctrl_raibert_create( &ctrl, &cmd, &model, none );
  for( c=cases; c->delta>0; c++ ){
    vec_set_elem_list( p, c->z, c->v );
    ctrl_raibert_set_delta( &ctrl, c->delta );
    ctrl_raibert_update_events( &ctrl, c->t, p );
    if( c->expected )
      ASSERT_TRUE( ctrl_raibert_is_in_thrust( &ctrl ) );
    else
      ASSERT_FALSE( ctrl_raibert_is_in_thrust( &ctrl ) );
  }
  ctrl_raibert_destroy( &ctrl );
  vec_destroy( p );
}

TEST(test_ctrl_raibert_is_in_thrust_2)
{
  struct case_t {
    double delta;
    double t, z, v;
    bool expected;
  } cases[] = {
    /* delta,  t,     z,     v,   exp */
    { 0.00, 0.00, 0.800,  0.00, false },  /* initial pos. */
    { 0.00, 0.20, 0.600, -1.98, false },  /* falling */
    { 0.00, 0.25, 0.500, -2.43, false },  /* touchdown */
    { 0.00, 0.40, 0.190,  0.00, false },  /* bottom */
    { 0.00, 0.42, 0.194,  0.30, false },  /* extension */
    { 0.00, 0.44, 0.198,  0.60, false },  /* extension */
    { 0.00, 0.45, 0.200,  1.00, false },  /* extension */
    { 0.00, 0.50, 0.400,  3.00, false },  /* extension */
    { 0.00, 0.55, 0.500,  4.00, false },  /* lift off */
    { -1, 0, 0, 0, false },
  };
  struct case_t *c;
  vec_t p;

  p = vec_create( 2 );
  cmd.zd = 0.8;
  cmd.z0 = 0.5;
  cmd.zb = 0.19;
  ctrl_raibert_create( &ctrl, &cmd, &model, none );
  for( c=cases; c->delta>=0.0; c++ ){
    vec_set_elem_list( p, c->z, c->v );
    ctrl_raibert_set_delta( &ctrl, c->delta );
    ctrl_raibert_update_events( &ctrl, c->t, p );
    if( c->expected )
      ASSERT_TRUE( ctrl_raibert_is_in_thrust( &ctrl ) );
    else
      ASSERT_FALSE( ctrl_raibert_is_in_thrust( &ctrl ) );
  }
  ctrl_raibert_destroy( &ctrl );
  vec_destroy( p );
}

TEST(test_ctrl_raibert_end_of_thrust)
{
  struct case_t {
    double delta;
    double t, z, v;
    /* expected recorded event */
    struct _ctrl_events_tuple_t et;
  } cases[] = {
    /* delta,  t,     z,     v,   exp */
    { 0.05, 0.00, 0.800,  0.00, { 0.00, 0.000,  0.00 } },  /* initial pos. */
    { 0.05, 0.20, 0.600, -1.98, { 0.00, 0.000,  0.00 } },  /* falling */
    { 0.05, 0.25, 0.500, -2.43, { 0.00, 0.000,  0.00 } },  /* touchdown */
    { 0.05, 0.40, 0.190,  0.00, { 0.40, 0.190,  0.00 } },  /* bottom */
    { 0.05, 0.42, 0.194,  0.30, { 0.42, 0.194,  0.30 } },  /* thrust */
    { 0.05, 0.44, 0.198,  0.60, { 0.44, 0.198,  0.60 } },  /* thrust */
    { 0.05, 0.45, 0.200,  1.00, { 0.45, 0.200,  1.00 } },  /* end of thrust */
    { 0.05, 0.50, 0.400,  3.00, { 0.45, 0.200,  1.00 } },  /* extension */
    { 0.05, 0.55, 0.500,  4.00, { 0.45, 0.200,  1.00 } },  /* lift off */
    { 0.05, 0.70, 0.600,  1.50, { 0.45, 0.200,  1.00 } },  /* rising */
    { 0.05, 0.80, 0.800,  0.00, { 0.45, 0.200,  1.00 } },  /* apex */

    { 0.05, 1.00, 0.600, -1.98, { 0.45, 0.200,  1.00 } },  /* falling */
    { 0.05, 1.05, 0.500, -2.43, { 0.45, 0.200,  1.00 } },  /* touchdown */
    { 0.05, 1.20, 0.190,  0.00, { 1.20, 0.190,  0.00 } },  /* bottom */
    { 0.05, 1.22, 0.194,  0.30, { 1.22, 0.194,  0.30 } },  /* thrust */
    { 0.05, 1.24, 0.198,  0.60, { 1.24, 0.198,  0.60 } },  /* thrust */
    { 0.05, 1.25, 0.200,  1.00, { 1.25, 0.200,  1.00 } },  /* end of thrust */
    { 0.05, 1.30, 0.400,  3.00, { 1.25, 0.200,  1.00 } },  /* extension */
    { 0.05, 1.35, 0.500,  4.00, { 1.25, 0.200,  1.00 } },  /* lift off */

    { 0, 0, 0, 0, { 0, 0, 0 } },
  };
  struct case_t *c;
  vec_t p;

  p = vec_create( 2 );
  cmd.zd = 0.8;
  cmd.z0 = 0.5;
  cmd.zb = 0.19;
  ctrl_raibert_create( &ctrl, &cmd, &model, none );
  for( c=cases; c->delta>0; c++ ){
    vec_set_elem_list( p, c->z, c->v );
    ctrl_raibert_set_delta( &ctrl, c->delta );
    ctrl_raibert_update_events( &ctrl, c->t, p );
    ASSERT_EQ( c->et.t, ctrl_raibert_end_of_thrust(&ctrl).t );
    ASSERT_EQ( c->et.z, ctrl_raibert_end_of_thrust(&ctrl).z );
    ASSERT_EQ( c->et.v, ctrl_raibert_end_of_thrust(&ctrl).v );
  }
  ctrl_raibert_destroy( &ctrl );
  vec_destroy( p );
}

TEST(test_ctrl_raibert_end_of_thrust_edge_case)
{
  struct case_t {
    double delta;
    double t, z, v;
    /* expected recorded event */
    struct _ctrl_events_tuple_t et;
  } cases[] = {
    /* delta,  t,     z,     v,   exp */
    { 0.03, 0.00, 0.800,  0.00, { 0.00, 0.000,  0.00 } },  /* initial pos. */
    { 0.03, 0.25, 0.500, -2.43, { 0.00, 0.000,  0.00 } },  /* touchdown */
    { 0.03, 0.40, 0.190,  0.00, { 0.40, 0.190,  0.00 } },  /* bottom */
    { 0.03, 0.41, 0.192,  0.30, { 0.41, 0.192,  0.30 } },  /* thrust */
    { 0.03, 0.42, 0.194,  0.32, { 0.42, 0.194,  0.32 } },  /* thrust */
    { 0.03, 0.43, 0.196,  0.34, { 0.43, 0.196,  0.34 } },  /* end of thrust */
    { 0.03, 0.44, 0.198,  0.40, { 0.43, 0.196,  0.34 } },  /* extension */
    { 0.03, 0.45, 0.200,  0.44, { 0.43, 0.196,  0.34 } },  /* extension */
    { 0, 0, 0, 0, { 0, 0, 0 } },
  };
  struct case_t *c;
  vec_t p;

  p = vec_create( 2 );
  cmd.zd = 0.8;
  cmd.z0 = 0.5;
  cmd.zb = 0.19;
  ctrl_raibert_create( &ctrl, &cmd, &model, none );
  for( c=cases; c->delta>0; c++ ){
    vec_set_elem_list( p, c->z, c->v );
    ctrl_raibert_set_delta( &ctrl, c->delta );
    ctrl_raibert_update_events( &ctrl, c->t, p );
    ASSERT_EQ( c->et.t, ctrl_raibert_end_of_thrust(&ctrl).t );
    ASSERT_EQ( c->et.z, ctrl_raibert_end_of_thrust(&ctrl).z );
    ASSERT_EQ( c->et.v, ctrl_raibert_end_of_thrust(&ctrl).v );
  }
  ctrl_raibert_destroy( &ctrl );
  vec_destroy( p );
}

TEST(test_ctrl_raibert_calc_fz_full_nonlinear)
{
  struct case_t{
    double t, z, v;
    double expected;
  } cases[] = {
    { 0.00, 0.800,  0.00, 0.0    },  /* initial pos. */
    { 0.20, 0.600, -1.98, 0.0    },  /* falling */
    { 0.25, 0.500, -2.43, 17.2819},  /* touchdown */
    { 0.40, 0.190,  0.00, 41.86  },  /* bottom */
    { 0.45, 0.195,  0.50, 40.695 },  /* thrust */
    { 0.50, 0.200,  1.00, 39.53  },  /* end of thrust */
    { 0.70, 0.400,  3.00, 13.94  },  /* extension */
    { 0.80, 0.500,  4.00, 0.0    },  /* lift off */
    { -1, 0, 0, 0 },
  };
  struct case_t *c;
  vec_t p;

  p = vec_create( 2 );
  model_set_mass( &model, 1 );
  cmd.zd = 0.8;
  cmd.z0 = 0.5;
  cmd.zb = 0.19;
  ctrl_raibert_create_full_nonlinear( &ctrl, &cmd, &model, 0.1, 41.86, 2.33, 5.81, 1 );
  for ( c=cases; c->t>=0; c++ ){
    vec_set_elem_list( p, c->z, c->v );
    ctrl_raibert_update_events( &ctrl, c->t, p );
    ASSERT_NEAR( c->expected, ctrl_raibert_calc_fz( &ctrl, c->t, p ), 1e-10 );
  }
  ctrl_raibert_destroy( &ctrl );
  vec_destroy( p );
}

TEST(test_ctrl_raibert_calc_fz_simplified_nonlinear)
{
  struct case_t{
    double t, z, v;
    double expected;
  } cases[] = {
    { 0.00, 1.150,  0.00, 0.0     },  /* initial pos. */
    { 0.27, 0.800, -2.62, 0.0     },  /* falling */
    { 0.36, 0.500, -3.57, 21.4267 },  /* touchdown */
    { 0.45, 0.300, -2.60, 29.1733 },  /* compress */
    { 0.60, 0.190,  0.00, 51.6667 },  /* bottom */
    { 0.70, 0.300,  2.60, 36.3180 },  /* extension */
    { 0.80, 0.500,  3.57, 0.0     },  /* lift off */
    { 0.90, 0.800,  2.62, 0.0     },  /* rising */
    { -1, 0, 0, 0 },
  };
  struct case_t *c;
  vec_t p;

  p = vec_create( 2 );
  model_set_mass( &model, 1 );
  cmd.zd = 1.15;
  cmd.z0 = 0.5;
  cmd.zb = 0.19;
  ctrl_raibert_create_simplified_nonlinear( &ctrl, &cmd, &model, 41.86, 5.81 );
  for ( c=cases; c->t>=0; c++ ){
    vec_set_elem_list( p, c->z, c->v );
    ctrl_raibert_update_events( &ctrl, c->t, p );
    ASSERT_NEAR( c->expected, ctrl_raibert_calc_fz( &ctrl, c->t, p ), 1e-4 );
  }
  ctrl_raibert_destroy( &ctrl );
  vec_destroy( p );
}

TEST(test_ctrl_raibert_calc_fz_full_linear)
{
  struct case_t{
    double t, z, v;
    double expected;
  } cases[] = {
    { 0.00, 1.000,  0.00, 0.0     },  /* initial pos. */
    { 0.20, 0.800, -1.98, 0.0     },  /* falling */
    { 0.32, 0.500, -3.13, 30.5429 },  /* touchdown */
    { 0.40, 0.300, -2.00, 37.21   },  /* compress */
    { 0.55, 0.190,  0.00, 41.86   },  /* bottom */
    { 0.57, 0.210,  0.50, 40.695  },  /* thrust */
    { 0.60, 0.220,  0.90, 39.763  },  /* end of thrust */
    { 0.70, 0.300,  2.00, 32.9067 },  /* extension */
    { 0.80, 0.500,  3.13, 0.0     },  /* lift off */
    { 0.90, 0.800,  1.98, 0.0     },  /* rising */
    { -1, 0, 0, 0 },
  };
  struct case_t *c;
  vec_t p;

  p = vec_create( 2 );
  model_set_mass( &model, 1 );
  cmd.zd = 0.8;
  cmd.z0 = 0.5;
  cmd.zb = 0.19;
  ctrl_raibert_create_full_linear( &ctrl, &cmd, &model, 0.05, 41.86, 2.33, 46.5, 1.0, 1 );
  for ( c=cases; c->t>=0; c++ ){
    vec_set_elem_list( p, c->z, c->v );
    ctrl_raibert_update_events( &ctrl, c->t, p );
    ASSERT_NEAR( c->expected, ctrl_raibert_calc_fz( &ctrl, c->t, p ), 1e-4 );
  }
  ctrl_raibert_destroy( &ctrl );
  vec_destroy( p );
}

TEST_SUITE(test_ctrl_raibert)
{
  CONFIGURE_SUITE( setup, teardown );
  RUN_TEST(test_ctrl_raibert_create);
  RUN_TEST(test_ctrl_raibert_destroy);
  RUN_TEST(test_ctrl_raibert_set_delta);
  RUN_TEST(test_ctrl_raibert_set_tau);
  RUN_TEST(test_ctrl_raibert_set_gamma);
  RUN_TEST(test_ctrl_raibert_set_yeta1);
  RUN_TEST(test_ctrl_raibert_set_zr);
  RUN_TEST(test_ctrl_raibert_set_mu);
  RUN_TEST(test_ctrl_raibert_set_params);
  RUN_TEST(test_ctrl_raibert_set_params_full_nonlinear);
  RUN_TEST(test_ctrl_raibert_set_params_simplified_nonlinear);
  RUN_TEST(test_ctrl_raibert_set_params_full_linear);
  RUN_TEST(test_ctrl_raibert_set_params_simplified_linear);
  RUN_TEST(test_ctrl_raibert_create_full_nonlinear);
  RUN_TEST(test_ctrl_raibert_create_simplified_nonlinear);
  RUN_TEST(test_ctrl_raibert_create_full_linear);
  RUN_TEST(test_ctrl_raibert_create_simplified_linear);
  RUN_TEST(test_ctrl_raibert_is_in_thrust);
  RUN_TEST(test_ctrl_raibert_is_in_thrust_2);
  RUN_TEST(test_ctrl_raibert_end_of_thrust);
  RUN_TEST(test_ctrl_raibert_end_of_thrust_edge_case);
  RUN_TEST(test_ctrl_raibert_calc_fz_full_nonlinear);
  RUN_TEST(test_ctrl_raibert_calc_fz_simplified_nonlinear);
  RUN_TEST(test_ctrl_raibert_calc_fz_full_linear);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_ctrl_raibert);
  TEST_REPORT();
  TEST_EXIT();
}
