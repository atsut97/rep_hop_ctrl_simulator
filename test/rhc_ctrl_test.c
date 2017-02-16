#include "rhc_ctrl.h"
#include "rhc_test.h"

static cmd_t cmd;
static model_t model;
static ctrl_t ctrl;
static vec_t p;

void setup()
{
  cmd_default_init( &cmd );
  model_init( &model, 10 );
  ctrl_init( &ctrl, &cmd, &model );
  p = vec_create( 2 );
}

void teardown()
{
  vec_destroy( p );
  ctrl_destroy( &ctrl );
  cmd_destroy( &cmd );
}

TEST(test_ctrl_init)
{
  ASSERT_PTREQ( &cmd, ctrl_cmd( &ctrl ) );
  ASSERT_PTREQ( &model, ctrl_model( &ctrl ) );
  ASSERT_EQ( 0, ctrl_fz( &ctrl ) );
  ASSERT_PTREQ( ctrl_update_default, ctrl._update );
  ASSERT_PTREQ( ctrl_destroy_default, ctrl._destroy );
  ASSERT_PTREQ( NULL, ctrl.prp);
  ASSERT_EQ( 0, ctrl_n( &ctrl ) );
  ASSERT_EQ( 0, ctrl_phi( &ctrl ) );
}

TEST(test_ctrl_destroy)
{
  ctrl_destroy( &ctrl );
  ASSERT_PTREQ( NULL, ctrl_cmd( &ctrl ) );
  ASSERT_PTREQ( NULL, ctrl_model( &ctrl ) );
  ASSERT_PTREQ( NULL, ctrl.prp );
  ASSERT_EQ( 0, ctrl_n( &ctrl ) );
  ASSERT_EQ( 0, ctrl_phi( &ctrl ) );
}

TEST(test_ctrl_z0)
{
  double cases[] = { 0.26, 0.28, 0.3, 0.0 };
  double *c;

  for( c=cases; *c>0; c++ ){
    cmd.z0 = *c;
    ASSERT_EQ( *c, ctrl_z0( &ctrl ) );
  }
}

TEST(test_ctrl_zd)
{
  double cases[] = { 0.28, 0.3, 0.32, 0.0 };
  double *c;

  for( c=cases; *c>0; c++ ){
    cmd.zd = *c;
    ASSERT_EQ( *c, ctrl_zd( &ctrl ) );
  }
}

TEST(test_ctrl_zb)
{
  double cases[] = { 0.24, 0.26, 0.28, 0.0 };
  double *c;

  for( c=cases; *c>0; c++ ){
    cmd.zb = *c;
    ASSERT_EQ( *c, ctrl_zb( &ctrl ) );
  }
}

TEST(test_ctrl_flight)
{
  struct case_t {
    double z0, z, v;
    bool expected;
  } cases[] = {
    { 0.26, 0.28, 0.0,   true  },     /* flight */
    { 0.26, 0.27, 0.1,   true  },     /* flight */
    { 0.26, 0.265, -0.2, true  },     /* flight */
    { 0.3,  0.33, 0.0,   true  },     /* flight */
    { 0.3,  0.32, 0.1,   true  },     /* flight */
    { 0.3,  0.31, -0.2,  true  },     /* flight */
    { 0.26, 0.26, -0.1,  false },     /* compression */
    { 0.26, 0.25, -0.1,  false },     /* compression */
    { 0.26, 0.24, -0.15, false },     /* compression */
    { 0.3,  0.3,  -0.1,  false },     /* compression */
    { 0.3,  0.28, -0.1,  false },     /* compression */
    { 0.3,  0.26, -0.15, false },     /* compression */
    { 0.26, 0.24, 0.0,   false },     /* bottom */
    { 0.3,  0.26, 0.0,   false },     /* bottom */
    { 0.26, 0.26, 0.1,   false },     /* decompression */
    { 0.26, 0.25, 0.1,   false },     /* decompression */
    { 0.26, 0.24, 0.15,  false },     /* decompression */
    { 0.3,  0.3,  0.1,   false },     /* decompression */
    { 0.3,  0.28, 0.1,   false },     /* decompression */
    { 0.3,  0.26, 0.15,  false },     /* decompression */
    { 0.0, 0.0, 0.0, false }
  };
  struct case_t *c;
  bool (*method)(ctrl_t*,vec_t);

  method = ctrl_is_in_flight;
  for( c=cases; c->z>0; c++ ){
    cmd.z0 = c->z0;
    vec_set_elem_list( p, 2, c->z, c->v );
    if( c->expected )
      ASSERT_TRUE( method( &ctrl, p ) );
    else
      ASSERT_FALSE( method( &ctrl, p ) );
  }
}

TEST(test_ctrl_compression)
{
  struct case_t {
    double z0, z, v;
    bool expected;
  } cases[] = {
    { 0.26, 0.28, 0.0,   false },     /* flight */
    { 0.26, 0.27, 0.1,   false },     /* flight */
    { 0.26, 0.265, -0.2, false },     /* flight */
    { 0.3,  0.33, 0.0,   false },     /* flight */
    { 0.3,  0.32, 0.1,   false },     /* flight */
    { 0.3,  0.31, -0.2,  false },     /* flight */
    { 0.26, 0.26, -0.1,  true  },     /* compression */
    { 0.26, 0.25, -0.1,  true  },     /* compression */
    { 0.26, 0.24, -0.15, true  },     /* compression */
    { 0.3,  0.3,  -0.1,  true  },     /* compression */
    { 0.3,  0.28, -0.1,  true  },     /* compression */
    { 0.3,  0.26, -0.15, true  },     /* compression */
    { 0.26, 0.24, 0.0,   false },     /* bottom */
    { 0.3,  0.26, 0.0,   false },     /* bottom */
    { 0.26, 0.26, 0.1,   false },     /* decompression */
    { 0.26, 0.25, 0.1,   false },     /* decompression */
    { 0.26, 0.24, 0.15,  false },     /* decompression */
    { 0.3,  0.3,  0.1,   false },     /* decompression */
    { 0.3,  0.28, 0.1,   false },     /* decompression */
    { 0.3,  0.26, 0.15,  false },     /* decompression */
    { 0.0, 0.0, 0.0, false }
  };
  struct case_t *c;
  bool (*method)(ctrl_t*,vec_t);

  method = ctrl_is_in_compression;
  for( c=cases; c->z>0; c++ ){
    cmd.z0 = c->z0;
    vec_set_elem_list( p, 2, c->z, c->v );
    if( c->expected )
      ASSERT_TRUE( method( &ctrl, p ) );
    else
      ASSERT_FALSE( method( &ctrl, p ) );
  }
}

TEST(test_ctrl_decompression)
{
  struct case_t {
    double z0, z, v;
    bool expected;
  } cases[] = {
    { 0.26, 0.28, 0.0,   false },     /* flight */
    { 0.26, 0.27, 0.1,   false },     /* flight */
    { 0.26, 0.265, -0.2, false },     /* flight */
    { 0.3,  0.33, 0.0,   false },     /* flight */
    { 0.3,  0.32, 0.1,   false },     /* flight */
    { 0.3,  0.31, -0.2,  false },     /* flight */
    { 0.26, 0.26, -0.1,  false },     /* compression */
    { 0.26, 0.25, -0.1,  false },     /* compression */
    { 0.26, 0.24, -0.15, false },     /* compression */
    { 0.3,  0.3,  -0.1,  false },     /* compression */
    { 0.3,  0.28, -0.1,  false },     /* compression */
    { 0.3,  0.26, -0.15, false },     /* compression */
    { 0.26, 0.24, 0.0,   true  },     /* bottom */
    { 0.3,  0.26, 0.0,   true  },     /* bottom */
    { 0.26, 0.26, 0.1,   true  },     /* decompression */
    { 0.26, 0.25, 0.1,   true  },     /* decompression */
    { 0.26, 0.24, 0.15,  true  },     /* decompression */
    { 0.3,  0.3,  0.1,   true  },     /* decompression */
    { 0.3,  0.28, 0.1,   true  },     /* decompression */
    { 0.3,  0.26, 0.15,  true  },     /* decompression */
    { 0.0, 0.0, 0.0, false }
  };
  struct case_t *c;
  bool (*method)(ctrl_t*,vec_t);

  method = ctrl_is_in_decompression;
  for( c=cases; c->z>0; c++ ){
    cmd.z0 = c->z0;
    vec_set_elem_list( p, 2, c->z, c->v );
    if( c->expected )
      ASSERT_TRUE( method( &ctrl, p ) );
    else
      ASSERT_FALSE( method( &ctrl, p ) );
  }
}

TEST(test_ctrl_v0)
{
  struct case_t {
    double z0, zd;
    double expected;
  } cases[] = {
    { 0.26, 0.28, sqrt(0.04*G) },
    { 0.25, 0.28, sqrt(0.06*G) },
    { 0.27, 0.3,  sqrt(0.06*G) },
    { 0, 0, 0 }
  };
  struct case_t *c;

  for( c=cases; c->z0>0; c++ ){
    cmd.z0 = c->z0;
    cmd.zd = c->zd;
    ASSERT_DOUBLE_EQ( c->expected, ctrl_v0( &ctrl ) );
  }
}

TEST(test_ctrl_update_default)
{
  struct case_t {
    double zd, z0, zb;
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
  for( c=cases; c->zd>0; c++ ){
    vec_set_elem_list( p, 2, c->z, c->v );
    ctrl_update( &ctrl, t, p );
    ASSERT_DOUBLE_EQ( c->expct_n, ctrl_n( &ctrl ) );
    ASSERT_DOUBLE_EQ( c->expct_phi, ctrl_phi( &ctrl ) );
    t += 0.01;
  }
}

TEST(test_ctrl_calc_phase_complex)
{
  struct case_t {
    double zd, z0, zb;
    double z, v;
    complex_t expected;
  } cases[] = {
    { 0.28, 0.26, 0.24, 0.28, 0.0, { 1.0, 0.0 } },           /* top */
    { 0.28, 0.26, 0.24, 0.26, -sqrt(0.04*G), { 0.0, 1.0 } }, /* touchdown */
    { 0.28, 0.26, 0.24, 0.24, 0.0, { -1.0, 0.0 } },          /* bottom */
    { 0.28, 0.26, 0.24, 0.24, -0.0, { -1.0, 0.0 } },         /* bottom */
    { 0.28, 0.26, 0.24, 0.26, sqrt(0.04*G), { 0.0, -1.0 } }, /* lift-off */
    { 0, 0, 0, 0, 0, { 0, 0 } },
  };
  struct case_t *c;
  complex_t cp;

  for( c=cases; c->zd>0; c++ ){
    vec_set_elem_list( p, 2, c->z, c->v );
    ctrl_calc_phase_complex( c->z0, c->zd, c->zb, p, &cp );
    ASSERT_DOUBLE_EQ( c->expected.re, cp.re );
    ASSERT_DOUBLE_EQ( c->expected.im, cp.im );
  }
}

TEST(test_ctrl_phase_complex)
{
  struct case_t {
    double zd, z0, zb;
    double z, v;
    complex_t expected;
  } cases[] = {
    { 0.28, 0.26, 0.24, 0.28, 0.0, { 1.0, 0.0 } },           /* top */
    { 0.28, 0.26, 0.24, 0.26, -sqrt(0.04*G), { 0.0, 1.0 } }, /* touchdown */
    { 0.28, 0.26, 0.24, 0.24, 0.0, { -1.0, 0.0 } },          /* bottom */
    { 0.28, 0.26, 0.24, 0.24, -0.0, { -1.0, 0.0 } },         /* bottom */
    { 0.28, 0.26, 0.24, 0.26, sqrt(0.04*G), { 0.0, -1.0 } }, /* lift-off */
    { 0, 0, 0, 0, 0, { 0, 0 } },
  };
  struct case_t *c;
  complex_t cp;

  for( c=cases; c->zd>0; c++ ){
    cmd.zd = c->zd;
    cmd.z0 = c->z0;
    cmd.zb = c->zb;
    vec_set_elem_list( p, 2, c->z, c->v );
    ctrl_phase_complex( &ctrl, p, &cp );
    ASSERT_DOUBLE_EQ( c->expected.re, cp.re );
    ASSERT_DOUBLE_EQ( c->expected.im, cp.im );
  }
}

TEST(test_ctrl_calc_phi)
{
  struct case_t {
    double zd, z0, zb;
    double z, v;
    double expected;
  } cases[] = {
    { 0.28, 0.26, 0.24, 0.28, 0.0, 0.0 },            /* top */
    { 0.28, 0.26, 0.24, 0.26, -sqrt(0.04*G), PI_2 }, /* touchdown */
    { 0.28, 0.26, 0.24, 0.24, -0.0, PI },            /* bottom */
    { 0.28, 0.26, 0.24, 0.24, 0.0, -PI },            /* bottom */
    { 0.28, 0.26, 0.24, 0.26, sqrt(0.04*G), -PI_2 }, /* lift-off */
    { 0, 0, 0, 0, 0, 0 },
  };
  struct case_t *c;

  for( c=cases; c->zd>0; c++ ){
    vec_set_elem_list( p, 2, c->z, c->v );
    ASSERT_DOUBLE_EQ( c->expected, ctrl_calc_phi( c->z0, c->zd, c->zb, p ) );
  }
}

TEST_SUITE(test_ctrl)
{
  CONFIGURE_SUITE( setup, teardown );
  RUN_TEST(test_ctrl_init);
  RUN_TEST(test_ctrl_destroy);
  RUN_TEST(test_ctrl_z0);
  RUN_TEST(test_ctrl_zd);
  RUN_TEST(test_ctrl_zb);
  RUN_TEST(test_ctrl_flight);
  RUN_TEST(test_ctrl_compression);
  RUN_TEST(test_ctrl_decompression);
  RUN_TEST(test_ctrl_v0);
  RUN_TEST(test_ctrl_update_default);
  RUN_TEST(test_ctrl_calc_phase_complex);
  RUN_TEST(test_ctrl_phase_complex);
  RUN_TEST(test_ctrl_calc_phi);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_ctrl);
  TEST_REPORT();
  TEST_EXIT();
}
