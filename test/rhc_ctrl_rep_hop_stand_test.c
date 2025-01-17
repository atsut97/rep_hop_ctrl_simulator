#include "rhc_ctrl.h"
#include "rhc_ctrl_rep_hop_stand.h"
#include "rhc_test.h"

static cmd_t cmd;
static model_t model;
static ctrl_t ctrl;
static vec_t p;

void setup()
{
  cmd_default_init( &cmd );
  model_init( &model, 10 );
  ctrl_rep_hop_stand_create( &ctrl, &cmd, &model );
  p = vec_create( 2 );
}

void teardown()
{
  vec_destroy( p );
  ctrl_destroy( &ctrl );
  cmd_destroy( &cmd );
}

TEST(test_ctrl_rep_hop_stand_cmd_init)
{
  ctrl_rep_hop_stand_cmd_init( &ctrl, &cmd );
  ASSERT_EQ( 0.0, ctrl_rep_hop_stand_rho( &ctrl ) );
  ASSERT_EQ( 4.0, ctrl_rep_hop_stand_k( &ctrl ) );
  ASSERT_TRUE( ctrl_rep_hop_stand_soft_landing( &ctrl ) );
}

TEST(test_ctrl_rep_hop_stand_create)
{
  ASSERT_PTREQ( ctrl_cmd( &ctrl ), &cmd );
  ASSERT_PTREQ( ctrl_model( &ctrl ), &model );
  ASSERT_PTREQ( ctrl_rep_hop_stand_update, ctrl._update );
  ASSERT_PTREQ( ctrl_rep_hop_stand_destroy, ctrl._destroy );
  ASSERT_PTREQ( ctrl_rep_hop_stand_header, ctrl._header );
  ASSERT_PTREQ( ctrl_rep_hop_stand_writer, ctrl._writer );
  ASSERT_PTRNE( NULL, ctrl.prp );

  ASSERT_EQ( rep_hop_stand_default, ctrl_rep_hop_stand_type(&ctrl) );
  ASSERT_PTREQ( ctrl_rep_hop_stand_update_params_default, ctrl_rep_hop_stand_get_prp(&ctrl)->_update_params );
  ASSERT_EQ( 0, ctrl_rep_hop_stand_q1(&ctrl) );
  ASSERT_EQ( 0, ctrl_rep_hop_stand_q2(&ctrl) );
  ASSERT_EQ( 0, ctrl_rep_hop_stand_vm(&ctrl) );
  ASSERT_FALSE( ctrl_rep_hop_stand_cushioning(&ctrl) );

  ASSERT_EQ( 0, ctrl_rep_hop_stand_rho(&ctrl) );
  ASSERT_EQ( 4.0, ctrl_rep_hop_stand_k(&ctrl) );
  ASSERT_TRUE( ctrl_rep_hop_stand_soft_landing(&ctrl) );

  ASSERT_EQ( 0.28, ctrl_rep_hop_stand_params_za(&ctrl) );
  ASSERT_EQ( 0.26, ctrl_rep_hop_stand_params_zh(&ctrl) );
  ASSERT_EQ( 0.255, ctrl_rep_hop_stand_params_zm(&ctrl) );
  ASSERT_EQ( 0.23, ctrl_rep_hop_stand_params_zb(&ctrl) );
  ASSERT_EQ( 0, ctrl_rep_hop_stand_params_rho(&ctrl) );
}

TEST(test_ctrl_rep_hop_stand_create_no_update)
{
  ctrl_t ctrl_no_update;
  ctrl_rep_hop_stand_create_with_type( &ctrl_no_update, &cmd, &model, no_update_params );

  ASSERT_PTREQ( ctrl_cmd( &ctrl_no_update ), &cmd );
  ASSERT_PTREQ( ctrl_model( &ctrl_no_update ), &model );
  ASSERT_PTREQ( ctrl_rep_hop_stand_update, ctrl._update );
  ASSERT_PTREQ( ctrl_rep_hop_stand_destroy, ctrl._destroy );
  ASSERT_PTREQ( ctrl_rep_hop_stand_header, ctrl._header );
  ASSERT_PTREQ( ctrl_rep_hop_stand_writer, ctrl._writer );
  ASSERT_PTRNE( NULL, ctrl.prp );

  ASSERT_EQ( no_update_params, ctrl_rep_hop_stand_type(&ctrl_no_update) );
  ASSERT_PTREQ( ctrl_rep_hop_stand_update_params_no_update, ctrl_rep_hop_stand_get_prp(&ctrl_no_update)->_update_params );
  ASSERT_EQ( 0, ctrl_rep_hop_stand_q1(&ctrl_no_update) );
  ASSERT_EQ( 0, ctrl_rep_hop_stand_q2(&ctrl_no_update) );
  ASSERT_EQ( 0, ctrl_rep_hop_stand_vm(&ctrl_no_update) );

  ASSERT_EQ( 0, ctrl_rep_hop_stand_rho(&ctrl_no_update) );
  ASSERT_EQ( 4.0, ctrl_rep_hop_stand_k(&ctrl_no_update) );
  ASSERT_TRUE( ctrl_rep_hop_stand_soft_landing(&ctrl_no_update) );

  ASSERT_EQ( 0.28, ctrl_rep_hop_stand_params_za(&ctrl_no_update) );
  ASSERT_EQ( 0.26, ctrl_rep_hop_stand_params_zh(&ctrl_no_update) );
  ASSERT_EQ( 0.255, ctrl_rep_hop_stand_params_zm(&ctrl_no_update) );
  ASSERT_EQ( 0.23, ctrl_rep_hop_stand_params_zb(&ctrl_no_update) );
  ASSERT_EQ( 0, ctrl_rep_hop_stand_params_rho(&ctrl_no_update) );

  ctrl_destroy( &ctrl_no_update );
}

TEST(test_ctrl_rep_hop_stand_destroy)
{
  ctrl_rep_hop_stand_destroy( &ctrl );
  ASSERT_PTREQ( NULL, ctrl_cmd( &ctrl ) );
  ASSERT_PTREQ( NULL, ctrl_model( &ctrl ) );
  ASSERT_PTREQ( NULL, ctrl.prp );
  ASSERT_EQ( 0, ctrl_n( &ctrl ) );
  ASSERT_EQ( 0, ctrl_phi( &ctrl ) );
}

TEST(test_ctrl_rep_hop_stand_set_rho)
{
  struct case_t {
    double rho;
  } cases[] = {
    {1.0}, {0.5}, {0.0}, {-1.0}
  };
  struct case_t *c;

  for( c=cases; c->rho>0; c++ ){
    ctrl_rep_hop_stand_set_rho( &ctrl, c->rho );
    ASSERT_EQ( c->rho, ctrl_rep_hop_stand_rho(&ctrl) );
  }
}

TEST(test_ctrl_rep_hop_stand_set_k)
{
  struct case_t {
    double k;
  } cases[] = {
    {1.0}, {0.5}, {0.0}, {-1.0}
  };
  struct case_t *c;

  for( c=cases; c->k>0; c++ ){
    ctrl_rep_hop_stand_set_k( &ctrl, c->k );
    ASSERT_EQ( c->k, ctrl_rep_hop_stand_k(&ctrl) );
  }
}

TEST(test_ctrl_rep_hop_stand_set_soft_landing)
{
  ctrl_rep_hop_stand_enable_soft_landing( &ctrl );
  ASSERT_TRUE( ctrl_rep_hop_stand_soft_landing(&ctrl) );
  ctrl_rep_hop_stand_disable_soft_landing( &ctrl );
  ASSERT_FALSE( ctrl_rep_hop_stand_soft_landing(&ctrl) );
  ctrl_rep_hop_stand_enable_soft_landing( &ctrl );
  ASSERT_TRUE( ctrl_rep_hop_stand_soft_landing(&ctrl) );
  ctrl_rep_hop_stand_disable_soft_landing( &ctrl );
  ASSERT_FALSE( ctrl_rep_hop_stand_soft_landing(&ctrl) );
  ctrl_rep_hop_stand_disable_soft_landing( &ctrl );
  ASSERT_FALSE( ctrl_rep_hop_stand_soft_landing(&ctrl) );
  ctrl_rep_hop_stand_enable_soft_landing( &ctrl );
  ASSERT_TRUE( ctrl_rep_hop_stand_soft_landing(&ctrl) );
}

TEST(test_ctrl_rep_hop_stand_calc_q1)
{
  struct case_t {
    double zh, zm;
    double expected;
  } cases[] = {
    { 1.0+G, 1.0, 1.0, },
    { 2.0, 1.0, sqrt(G), },
    { 1.5, 1.0, sqrt(2.0*G), },
    { 0.0, 0.0, 0.0, },
  };
  struct case_t *c;
  double q1;

  for( c=cases; c->expected>0.0; c++ ){
    q1=ctrl_rep_hop_stand_calc_q1( c->zh, c->zm, G );
    ASSERT_NEAR( c->expected, q1, 1e-10);
  }
}

TEST(test_ctrl_rep_hop_stand_calc_r)
{
  struct case_t {
    double zm, zb;
    double expected;
  } cases[] = {
    { 1.0, 0.5, 0.5, },
    { 2.0, 1.2, 0.8, },
    { 2.0, 1.0, 1.0, },
    { 0.0, 0.0, 0.0, },
  };
  struct case_t *c;
  double r;

  for( c=cases; c->expected>0; c++ ){
    r = ctrl_rep_hop_stand_calc_r( c->zm, c->zb );
    ASSERT_NEAR( c->expected, r, 1e-10 );
  }
}

TEST(test_ctrl_rep_hop_stand_calc_sqr_vm)
{
  struct case_t {
    double zh, zm, zb;
    double expected;
  } cases[] = {
    { 3.0, 2.0, 1.0, G, },
    { 2.0, 1.5, 0.5, 2.0*G, },
    { 2.0, 1.0, 0.5, 0.25*G, },
    { 0.0, 0.0, 0.0, 0.0, },
  };
  struct case_t *c;
  double sqr_vm, vm;

  for( c=cases; c->expected>0; c++ ){
    sqr_vm = ctrl_rep_hop_stand_calc_sqr_vm( c->zh, c->zm, c->zb, G );
    vm = ctrl_rep_hop_stand_calc_vm( c->zh, c->zm, c->zb, G );
    ASSERT_NEAR( c->expected, sqr_vm, 1e-10 );
    ASSERT_NEAR( sqrt( c->expected ), vm, 1e-10 );
  }
}

TEST(test_ctrl_rep_hop_stand_calc_sqr_gamma)
{
  struct case_t {
    double z, v, zh, zm, zb;
    double expected;
  } cases[] = {
    { 1.0, 0.0, 1.5+G, 1.5, 0.5, 0.25, },
    { 1.0, 0.5, 2.0, 1.0, 0.5, 1.0/G, },
    { 1.0, -G, 3.0, 2.0, 1.0, 1.0 + G, },
    { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, },
  };
  struct case_t *c;
  double sqr_gamma, gamma;

  for( c=cases; c->expected>0; c++ ){
    vec_set_elem_list( p, c->z, c->v );
    sqr_gamma = ctrl_rep_hop_stand_calc_sqr_gamma( p, c->zh, c->zm, c->zb, G );
    gamma = ctrl_rep_hop_stand_calc_gamma( p, c->zh, c->zm, c->zb, G );
    ASSERT_NEAR( c->expected, sqr_gamma, 1e-10 );
    ASSERT_NEAR( sqrt( c->expected ), gamma, 1e-10 );
  }
}

TEST(test_ctrl_rep_hop_stand_calc_za)
{
  struct case_t {
    double zh, zm, zb;
    double expected;
  } cases[] = {
    { 2.0, 1.5, 1.0, 2.0, },
    { 3.0, 2.0, 1.0, 3.0, },
    { 3.0, 2.0, 0.5, 3.625, },
    { 0.0, 0.0, 0.0, 0.0, },
  };
  struct case_t *c;
  double za;

  for( c=cases; c->expected>0; c++ ){
    za = ctrl_rep_hop_stand_calc_za( c->zh, c->zm, c->zb );
    ASSERT_NEAR( c->expected, za, 1e-10 );
  }
}

TEST(test_ctrl_rep_hop_stand_calc_zh)
{
  struct case_t {
    double za, zm, zb;
    double expected;
  } cases[] = {
    { 2.0, 1.5, 1.0, 2.0, },
    { 3.0, 1.5, 1.0, 3.0 - sqrt(2.0), },
    { 3.0, 1.5, 0.5, 3.0 - 0.5 * sqrt(5.0), },
    { 0.0, 0.0, 0.0, 0.0, },
  };
  struct case_t *c;
  double zh;

  for( c=cases; c->expected>0; c++ ){
    zh = ctrl_rep_hop_stand_calc_zh( c->za, c->zm, c->zb );
    ASSERT_NEAR( c->expected, zh, 1e-10 );
  }
}

TEST(test_ctrl_rep_hop_stand_calc_zm)
{
  struct case_t {
    double za, zh, zb;
    double expected;
  } cases[] = {
    { 2.0, 1.5, 1.0, 1.375, },
    { 3.0, 2.0, 1.0, 1.75, },
    { 3.0, 2.0, 0.5, 1.55, },
    { 0.0, 0.0, 0.0, 0.0, },
  };
  struct case_t *c;
  double zm;

  for( c=cases; c->expected>0; c++ ){
    zm = ctrl_rep_hop_stand_calc_zm( c->za, c->zh, c->zb );
    ASSERT_NEAR( c->expected, zm, 1e-10 );
  }
}

TEST(test_ctrl_rep_hop_stand_calc_zm_when_za_lower_than_zh)
{
  struct case_t {
    double za, zh, zb;
    double expected;
  } cases[] = {
    { 1.4, 1.5, 1.0, 1.2, },
    { 1.5, 2.0, 1.0, 1.25, },
    { 1.5, 2.0, 0.5, 1.0, },
    { 0.0, 0.0, 0.0, 0.0, },
  };
  struct case_t *c;
  double zm;

  for( c=cases; c->expected>0; c++ ){
    zm = ctrl_rep_hop_stand_calc_zm( c->za, c->zh, c->zb );
    ASSERT_NEAR( c->expected, zm, 1e-10 );
  }
}

TEST(test_ctrl_rep_hop_stand_calc_zb)
{
  struct case_t {
    double za, zh, zm;
    double expected;
  } cases[] = {
    { 2.0, 1.5, 1.0, 1.0 - 0.5 * sqrt(3.0), },
    { 3.0, 2.0, 1.0, 1.0 - sqrt(3.0), },
    { 3.0, 2.0, 0.5, 0.5 - sqrt(1.5*3.5), },
    { 0.0, 0.0, 0.0, 0.0, },
  };
  struct case_t *c;
  double zb;

  for( c=cases; c->za>0.0; c++ ){
    zb = ctrl_rep_hop_stand_calc_zb( c->za, c->zh, c->zm );
    ASSERT_NEAR( c->expected, zb, 1e-10 );
  }
}

TEST(test_ctrl_rep_hop_stand_update_params_hop_fix_zb)
{
  struct case_t {
    double z, v, za, zh, zm, zb, rho;
    double expected_zb;
  } cases[] = {
    /*  z,   v , za,  zh,  zm,  zb, rho, expected_zb */
    { 0.0, 0.0, 2.5, 2.0, 1.5, 0.5, 1.0, 1.5-0.5*sqrt(3), },
    { 0.0, 0.0, 1.5, 1.0, 0.8, 0.2, 1.0, 0.8-0.2*sqrt(6), },
    { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, },
  };
  struct case_t *c;

  for( c=cases; c->zb>0.0; c++ ){
    cmd_set( &cmd, c->za, c->zh, c->zm, c->zb );
    ctrl_rep_hop_stand_set_rho( &ctrl, c->rho );
    ctrl_rep_hop_stand_disable_soft_landing( &ctrl );
    vec_set_elem_list( p, c->z, c->v );
    ctrl_rep_hop_stand_update_params_default( &ctrl, p );
    ASSERT_EQ( c->za, ctrl_rep_hop_stand_params_za(&ctrl) );
    ASSERT_EQ( c->zh, ctrl_rep_hop_stand_params_zh(&ctrl) );
    ASSERT_EQ( c->zm, ctrl_rep_hop_stand_params_zm(&ctrl) );
    ASSERT_NEAR( c->expected_zb, ctrl_rep_hop_stand_params_zb(&ctrl), 1e-10 );
    ASSERT_EQ( c->rho, ctrl_rep_hop_stand_params_rho(&ctrl) );
  }
}

TEST(test_ctrl_rep_hop_stand_update_params_hop_fix_zm)
{
  struct case_t {
    double z, v, za, zh, zm, zb, rho;
    double expected_zm;
  } cases[] = {
    /*  z,   v,  za,  zh,  zm,  zb, rho, expected_zm */
    { 0.0, 0.0, 2.5, 2.0, 1.5, 1.0, 1.0, 1.75-1.0/12.0, },
    { 0.0, 0.0, 1.5, 1.0, 0.8, 0.5, 1.0, 0.875, },
    { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, },
  };
  struct case_t *c;

  for( c=cases; c->zb>0.0; c++ ){
    cmd_set( &cmd, c->za, c->zh, c->zm, c->zb );
    ctrl_rep_hop_stand_set_rho( &ctrl, c->rho );
    ctrl_rep_hop_stand_disable_soft_landing( &ctrl );
    vec_set_elem_list( p, c->z, c->v );
    ctrl_rep_hop_stand_update_params_default( &ctrl, p );
    ASSERT_EQ( c->za, ctrl_rep_hop_stand_params_za(&ctrl) );
    ASSERT_EQ( c->zh, ctrl_rep_hop_stand_params_zh(&ctrl) );
    ASSERT_NEAR( c->expected_zm, ctrl_rep_hop_stand_params_zm(&ctrl), 1e-10 );
    ASSERT_EQ( c->zb, ctrl_rep_hop_stand_params_zb(&ctrl) );
    ASSERT_EQ( c->rho, ctrl_rep_hop_stand_params_rho(&ctrl) );
  }
}

TEST(test_ctrl_rep_hop_stand_update_params_squat_fix_zm)
{
  struct case_t {
    double z, v, za, zh, zm, zb, rho;
    double expected_zm;
  } cases[] = {
    /*  z,   v,  za,  zh,  zm,  zb, rho, expected_zm */
    { 0.0, 0.0, 1.8, 2.0, 1.5, 1.0, 1.0, 1.4, },
    { 0.0, 0.0, 0.7, 1.0, 0.8, 0.5, 1.0, 0.6, },
    { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, },
  };
  struct case_t *c;

  for( c=cases; c->zb>0.0; c++ ){
    cmd_set( &cmd, c->za, c->zh, c->zm, c->zb );
    ctrl_rep_hop_stand_set_rho( &ctrl, c->rho );
    ctrl_rep_hop_stand_disable_soft_landing( &ctrl );
    vec_set_elem_list( p, c->z, c->v );
    ctrl_rep_hop_stand_update_params_default( &ctrl, p );
    ASSERT_EQ( c->za, ctrl_rep_hop_stand_params_za(&ctrl) );
    ASSERT_EQ( c->zh, ctrl_rep_hop_stand_params_zh(&ctrl) );
    ASSERT_NEAR( c->expected_zm, ctrl_rep_hop_stand_params_zm(&ctrl), 1e-10 );
    ASSERT_EQ( c->zb, ctrl_rep_hop_stand_params_zb(&ctrl) );
    ASSERT_EQ( c->rho, ctrl_rep_hop_stand_params_rho(&ctrl) );
  }
}

TEST(test_ctrl_rep_hop_stand_update_params_hop_soft_landing_fix_za)
{
  struct case_t {
    double z, v, za, zh, zm, zb, rho, z_apex;
    double expected_za;
  } cases[] = {
    /*   z,   v , za,  zh,  zm,  zb, rho, z_apex, expected_za */
    { 1.5, -0.5, 2.5, 2.0, 1.55, 0.5, 1.0, 3.0, 3.0, },
    { 0.8, -0.5, 1.5, 1.0, 0.8,  0.8-sqrt(5)/5, 1.0, 1.4, 1.4, },
    { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, },
  };
  struct case_t *c;

  for( c=cases; c->zb>0.0; c++ ){
    cmd_set( &cmd, c->za, c->zh, c->zm, c->zb );
    ctrl_rep_hop_stand_set_rho( &ctrl, c->rho );
    ctrl_rep_hop_stand_enable_soft_landing( &ctrl );

    ctrl_events_init( ctrl_events( &ctrl ) );
    vec_set_elem_list( p, c->z_apex, 0.0 ); /* apex */
    ctrl_events_update( ctrl_events(&ctrl), 1.0, p, &cmd, G );
    vec_set_elem_list( p, c->zh, -0.3 ); /* touchdown */
    ctrl_events_update( ctrl_events(&ctrl), 1.0, p, &cmd, G );
    vec_set_elem_list( p, c->z, c->v ); /* compression */

    ctrl_rep_hop_stand_update_params_default( &ctrl, p );
    ASSERT_NEAR( c->expected_za, ctrl_rep_hop_stand_params_za(&ctrl), 1e-10 );
    ASSERT_EQ( c->zh, ctrl_rep_hop_stand_params_zh(&ctrl) );
    ASSERT_EQ( c->zm, ctrl_rep_hop_stand_params_zm(&ctrl) );
    ASSERT_NEAR( c->zb, ctrl_rep_hop_stand_params_zb(&ctrl), 1e-10 );
    ASSERT_EQ( c->rho, ctrl_rep_hop_stand_params_rho(&ctrl) );
  }
}



TEST(test_ctrl_rep_hop_stand_update_params_no_update)
{
  struct case_t {
    double z, v, za, zh, zm, zb, rho;
    double expected_zm;
  } cases[] = {
    /*  z,   v,  za,  zh,  zm,  zb, rho, expected_zm */
    { 0.0, 0.0, 2.5, 2.0, 1.5, 1.0, 1.0, 1.75-1.0/12.0, },
    { 0.0, 0.0, 1.5, 1.0, 0.8, 0.5, 1.0, 0.875, },
    { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, },
  };
  struct case_t *c;

  for( c=cases; c->zb>0.0; c++ ){
    cmd_set( &cmd, c->za, c->zh, c->zm, c->zb );
    ctrl_rep_hop_stand_set_rho( &ctrl, c->rho );
    vec_set_elem_list( p, c->z, c->v );
    ctrl_rep_hop_stand_update_params_no_update( &ctrl, p );
    ASSERT_EQ( c->za, ctrl_rep_hop_stand_params_za(&ctrl) );
    ASSERT_EQ( c->zh, ctrl_rep_hop_stand_params_zh(&ctrl) );
    ASSERT_NEAR( c->expected_zm, ctrl_rep_hop_stand_params_zm(&ctrl), 1e-10 );
    ASSERT_EQ( c->zb, ctrl_rep_hop_stand_params_zb(&ctrl) );
    ASSERT_EQ( c->rho, ctrl_rep_hop_stand_params_rho(&ctrl) );
  }
}

TEST_SUITE(test_ctrl_rep_hop_stand)
{
  CONFIGURE_SUITE( setup, teardown );
  RUN_TEST(test_ctrl_rep_hop_stand_cmd_init);
  RUN_TEST(test_ctrl_rep_hop_stand_create);
  RUN_TEST(test_ctrl_rep_hop_stand_create_no_update);
  RUN_TEST(test_ctrl_rep_hop_stand_destroy);
  RUN_TEST(test_ctrl_rep_hop_stand_set_rho);
  RUN_TEST(test_ctrl_rep_hop_stand_set_k);
  RUN_TEST(test_ctrl_rep_hop_stand_set_soft_landing);
  RUN_TEST(test_ctrl_rep_hop_stand_calc_q1);
  RUN_TEST(test_ctrl_rep_hop_stand_calc_r);
  RUN_TEST(test_ctrl_rep_hop_stand_calc_sqr_vm);
  RUN_TEST(test_ctrl_rep_hop_stand_calc_sqr_gamma);
  RUN_TEST(test_ctrl_rep_hop_stand_calc_za);
  RUN_TEST(test_ctrl_rep_hop_stand_calc_zh);
  RUN_TEST(test_ctrl_rep_hop_stand_calc_zm);
  RUN_TEST(test_ctrl_rep_hop_stand_calc_zm_when_za_lower_than_zh);
  RUN_TEST(test_ctrl_rep_hop_stand_calc_zb);
  RUN_TEST(test_ctrl_rep_hop_stand_update_params_hop_fix_zb);
  RUN_TEST(test_ctrl_rep_hop_stand_update_params_hop_fix_zm);
  RUN_TEST(test_ctrl_rep_hop_stand_update_params_squat_fix_zm);
  RUN_TEST(test_ctrl_rep_hop_stand_update_params_hop_soft_landing_fix_za);
  RUN_TEST(test_ctrl_rep_hop_stand_update_params_no_update);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_ctrl_rep_hop_stand);
  TEST_REPORT();
  TEST_EXIT();
}
