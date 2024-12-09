#include "rhc_ctrl_slip_var_stiff.h"
#include "rhc_test.h"

static cmd_t cmd;
static model_t model;
static ctrl_t ctrl;
static vec_t p;

void setup()
{
  cmd_default_init( &cmd );
  model_init( &model, 1 );
  ctrl_slip_var_stiff_create( &ctrl, &cmd, &model );
  p = vec_create( 2 );
}

void teardown()
{
  vec_destroy( p );
  ctrl_destroy( &ctrl );
  cmd_destroy( &cmd );
}

TEST(test_ctrl_slip_var_stiff_create)
{
  ASSERT_PTREQ( ctrl_slip_var_stiff_update, ctrl._update );
  ASSERT_PTREQ( ctrl_slip_var_stiff_destroy, ctrl._destroy );
  ASSERT_PTREQ( ctrl_slip_var_stiff_header, ctrl._header );
  ASSERT_PTREQ( ctrl_slip_var_stiff_writer, ctrl._writer );
  ASSERT_EQ( 0, ((ctrl_slip_var_stiff_prp*)ctrl.prp)->k );
}

TEST(test_ctrl_slip_var_stiff_destroy)
{
  ctrl_t c;
  ctrl_slip_var_stiff_create( &c, &cmd, &model );
  ASSERT_PTRNE( NULL, ctrl_cmd( &c ) );
  ASSERT_PTRNE( NULL, c.prp );
  ctrl_destroy( &c );
  ASSERT_PTREQ( NULL, ctrl_cmd( &c ) );
  ASSERT_PTREQ( NULL, c.prp );
}

TEST(test_ctrl_slip_var_stiff_calc_stiffness_decomp)
{
  struct case_t{
    double m, z, v, zh, za, zb;
    double expected_k;
  } cases[] = {
    { 1, 0.24, 0, 0.26, 0.28, 0.24, 200*G },
    { 1, 0.25, 0.1, 0.26, 0.28, 0.24, (6*G-1)*100 },
    { 2, 0.24, 0, 0.26, 0.28, 0.24, 400*G },
    { 1, 0.26, 0.2*sqrt(G), 0.26, 0.28, 0.24, 0 },
    { 0, 0, 0, 0, 0, 0, 0 }
  };
  struct case_t *c;
  double (*f)(vec_t,double,double,double,double);
  vec_t p;

  f = ctrl_slip_var_stiff_calc_stiffness_decomp;
  p = vec_create( 2 );
  for( c=cases; c->m>0; c++ ) {
    vec_set_elem_list( p, c->z, c->v );
    ASSERT_NEAR( c->expected_k,
                 f( p, c->m, c->zh, c->za, c->zb ), 1e-10 );
  }
  vec_destroy( p );
}

TEST(test_ctrl_slip_var_stiff_calc_stiffness_comp)
{
  struct case_t{
    double m, z, v, zh, za, zb;
    double expected_k;
  } cases[] = {
    { 1, 0.26, -0.2*sqrt(G), 0.26, 0.28, 0.24, 200*G },
    { 1, 0.25, -0.1, 0.26, 0.28, 0.24, (2*G+1)/0.03 },
    { 2, 0.26, -0.2*sqrt(G), 0.26, 0.28, 0.24, 400*G },
    { 0, 0, 0, 0, 0, 0, 0 }
  };
  struct case_t *c;
  double (*f)(vec_t,double,double,double,double);
  vec_t p;

  f = ctrl_slip_var_stiff_calc_stiffness_comp;
  p = vec_create( 2 );
  for( c=cases; c->m>0; c++ ) {
    vec_set_elem_list( p, c->z, c->v );
    ASSERT_NEAR( c->expected_k,
                 f( p, c->m, c->zh, c->za, c->zb ), 1e-10 );
  }
  vec_destroy( p );
}

TEST(test_ctrl_slip_var_stiff_calc_stiffness)
{
  struct case_t{
    double m, z, v, zh, za, zb;
    double expected_k;
  } cases[] = {
    /* extension phases */
    { 1, 0.24, 0, 0.26, 0.28, 0.24, 200*G },
    { 1, 0.25, 0.1, 0.26, 0.28, 0.24, (6*G-1)*100 },
    { 2, 0.24, 0, 0.26, 0.28, 0.24, 400*G },
    /* compression phases */
    { 1, 0.26, -0.2*sqrt(G), 0.26, 0.28, 0.24, 200*G },
    { 1, 0.25, -0.1, 0.26, 0.28, 0.24, (2*G+1)/0.03 },
    { 2, 0.26, -0.2*sqrt(G), 0.26, 0.28, 0.24, 400*G },
    { 0, 0, 0, 0, 0, 0, 0 }
  };
  struct case_t *c;
  double (*f)(vec_t,double,double,double,double);
  vec_t p;

  f = ctrl_slip_var_stiff_calc_stiffness;
  p = vec_create( 2 );
  for( c=cases; c->m>0; c++ ) {
    vec_set_elem_list( p, c->z, c->v );
    ASSERT_NEAR( c->expected_k,
                 f( p, c->m, c->zh, c->za, c->zb ), 1e-10 );
  }
  vec_destroy( p );
}

TEST(test_ctrl_slip_var_stiff_update)
{
  struct case_t{
    double z, v;
    double expected_fz;
  } cases[] = {
    { 0.26, -0.2*sqrt(G), 0 },  /* touchdown */
    { 0.24, 0, 4*G },           /* bottom */
    { 0.26, 0.2*sqrt(G), 0 },   /* lift-off */
    { 0.28, 0, 0 },             /* apex */
    { 0, 0, 0 }
  };
  struct case_t *c;
  vec_t p;

  p = vec_create( 2 );
  for( c=cases; c->z>0; c++ ) {
    vec_set_elem_list( p, c->z, c->v );
    ctrl_update( &ctrl, 0, p );
    ASSERT_NEAR( c->expected_fz, ctrl_fz(&ctrl), 1e-10 );
  }
  vec_destroy( p );
}

TEST_SUITE(test_ctrl_slip_var_stiff)
{
  CONFIGURE_SUITE( setup, teardown );
  RUN_TEST(test_ctrl_slip_var_stiff_create);
  RUN_TEST(test_ctrl_slip_var_stiff_destroy);
  RUN_TEST(test_ctrl_slip_var_stiff_calc_stiffness_decomp);
  RUN_TEST(test_ctrl_slip_var_stiff_calc_stiffness_comp);
  RUN_TEST(test_ctrl_slip_var_stiff_calc_stiffness);
  RUN_TEST(test_ctrl_slip_var_stiff_update);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_ctrl_slip_var_stiff);
  TEST_REPORT();
  TEST_EXIT();
}
