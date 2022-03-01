#include "rhc_cmd.h"
#include "rhc_ctrl.h"
#include "rhc_test.h"
#include "rhc_vec.h"

static cmd_t cmd;
static ctrl_events_t events;
static vec_t p;

void setup_events()
{
  cmd_default_init( &cmd );
  ctrl_events_init( &events );
  p = vec_create( 2 );
}

void teardown_events()
{
  vec_destroy( p );
  ctrl_events_destroy( &events );
  cmd_destroy( &cmd );
}

TEST(test_ctrl_events_init)
{
  ctrl_events_init( &events );
  ASSERT_EQ( 0, ctrl_events_apex_t(&events) );
  ASSERT_EQ( 0, ctrl_events_apex_z(&events) );
  ASSERT_EQ( 0, ctrl_events_apex_v(&events) );
  ASSERT_EQ( 0, ctrl_events_touchdown_t(&events) );
  ASSERT_EQ( 0, ctrl_events_touchdown_z(&events) );
  ASSERT_EQ( 0, ctrl_events_touchdown_v(&events) );
  ASSERT_EQ( 0, ctrl_events_bottom_t(&events) );
  ASSERT_EQ( 0, ctrl_events_bottom_z(&events) );
  ASSERT_EQ( 0, ctrl_events_bottom_v(&events) );
  ASSERT_EQ( 0, ctrl_events_liftoff_t(&events) );
  ASSERT_EQ( 0, ctrl_events_liftoff_z(&events) );
  ASSERT_EQ( 0, ctrl_events_liftoff_v(&events) );
  ASSERT_EQ( invalid, events.phase );
  ASSERT_EQ( 0, ctrl_events_phi(&events) );
  ASSERT_EQ( 0, ctrl_events_n(&events) );
}

TEST(test_ctrl_events_destroy)
{
  ctrl_events_destroy( &events );
  ASSERT_EQ( 0, ctrl_events_apex_t(&events) );
  ASSERT_EQ( 0, ctrl_events_apex_z(&events) );
  ASSERT_EQ( 0, ctrl_events_apex_v(&events) );
  ASSERT_EQ( 0, ctrl_events_touchdown_t(&events) );
  ASSERT_EQ( 0, ctrl_events_touchdown_z(&events) );
  ASSERT_EQ( 0, ctrl_events_touchdown_v(&events) );
  ASSERT_EQ( 0, ctrl_events_bottom_t(&events) );
  ASSERT_EQ( 0, ctrl_events_bottom_z(&events) );
  ASSERT_EQ( 0, ctrl_events_bottom_v(&events) );
  ASSERT_EQ( 0, ctrl_events_liftoff_t(&events) );
  ASSERT_EQ( 0, ctrl_events_liftoff_z(&events) );
  ASSERT_EQ( 0, ctrl_events_liftoff_v(&events) );
  ASSERT_EQ( invalid, events.phase );
  ASSERT_EQ( 0, ctrl_events_phi(&events) );
  ASSERT_EQ( 0, ctrl_events_n(&events) );
}

TEST(test_ctrl_events_calc_phase_complex)
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
    ctrl_events_calc_phase_complex( c->z0, c->zd, c->zb, p, &cp );
    ASSERT_DOUBLE_EQ( c->expected.re, cp.re );
    ASSERT_DOUBLE_EQ( c->expected.im, cp.im );
  }
}

TEST(test_ctrl_events_calc_phi)
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
    ASSERT_DOUBLE_EQ( c->expected, ctrl_events_calc_phi( c->z0, c->zd, c->zb, p ) );
  }
}

TEST(test_ctrl_events_is_in_rising)
{
  struct case_t {
    double zd, z0, zb;
    double z, v;
    bool expected;
  } cases[] = {
    /* zd ,  z0 ,  zb ,    z ,     v , expected */
    { 0.28, 0.26, 0.24, 0.280,  0.000, false },  /* initial */
    { 0.28, 0.26, 0.24, 0.270, -0.010, false },  /* falling */
    { 0.28, 0.26, 0.24, 0.260, -0.030, false },  /* touchdown */
    { 0.28, 0.26, 0.24, 0.250, -0.020, false },  /* compression */
    { 0.28, 0.26, 0.24, 0.240,  0.000, false },  /* bottom */
    { 0.28, 0.26, 0.24, 0.250,  0.020, false },  /* extension */
    { 0.28, 0.26, 0.24, 0.260,  0.030, true  },  /* lift-off */
    { 0.28, 0.26, 0.24, 0.265,  0.025, true  },  /* rising */
    { 0.28, 0.26, 0.24, 0.270,  0.020, true  },  /* rising */
    { 0.28, 0.26, 0.24, 0.275,  0.010, true  },  /* rising */
    { 0.28, 0.26, 0.24, 0.280,  0.000, false },  /* apex */
    { 0.28, 0.26, 0.24, 0.270, -0.010, false },  /* falling */
    {    0,    0,    0,     0,      0, false },  /* terminator */
  };
  struct case_t *c;

  for( c=cases; c->zd>0; c++ ){
    cmd.zd = c->zd;
    cmd.z0 = c->z0;
    cmd.zb = c->zb;
    vec_set_elem_list( p, c->z, c->v );
    ctrl_events_update( &events, 0, p, &cmd );
    if( c->expected )
      ASSERT_TRUE( ctrl_events_is_in_rising( &events ) );
    else
      ASSERT_FALSE( ctrl_events_is_in_rising( &events ) );
  }
}

TEST(test_ctrl_events_is_in_falling)
{
  struct case_t {
    double zd, z0, zb;
    double z, v;
    bool expected;
  } cases[] = {
    /* zd ,  z0 ,  zb ,    z ,     v , expected */
    { 0.28, 0.26, 0.24, 0.280,  0.000, true  },  /* initial */
    { 0.28, 0.26, 0.24, 0.279, -0.001, true  },  /* falling */
    { 0.28, 0.26, 0.24, 0.270, -0.010, true  },  /* falling */
    { 0.28, 0.26, 0.24, 0.265, -0.020, true  },  /* falling */
    { 0.28, 0.26, 0.24, 0.260, -0.030, false },  /* touchdown */
    { 0.28, 0.26, 0.24, 0.250, -0.020, false },  /* compression */
    { 0.28, 0.26, 0.24, 0.240,  0.000, false },  /* bottom */
    { 0.28, 0.26, 0.24, 0.250,  0.020, false },  /* extension */
    { 0.28, 0.26, 0.24, 0.260,  0.030, false },  /* lift-off */
    { 0.28, 0.26, 0.24, 0.270,  0.020, false },  /* rising */
    { 0.28, 0.26, 0.24, 0.280,  0.000, true  },  /* apex */
    { 0.28, 0.26, 0.24, 0.279, -0.001, true  },  /* falling */
    { 0.28, 0.26, 0.24, 0.270, -0.010, true  },  /* falling */
    { 0.28, 0.26, 0.24, 0.265, -0.020, true  },  /* falling */
    { 0.28, 0.26, 0.24, 0.260, -0.030, false },  /* touchdown */
    {    0,    0,    0,     0,      0, false },  /* terminator */
  };
  struct case_t *c;

  for( c=cases; c->zd>0; c++ ){
    cmd.zd = c->zd;
    cmd.z0 = c->z0;
    cmd.zb = c->zb;
    vec_set_elem_list( p, c->z, c->v );
    ctrl_events_update( &events, 0, p, &cmd );
    if( c->expected )
      ASSERT_TRUE( ctrl_events_is_in_falling( &events ) );
    else
      ASSERT_FALSE( ctrl_events_is_in_falling( &events ) );
  }
}

TEST(test_ctrl_events_is_in_flight)
{
  struct case_t {
    double zd, z0, zb;
    double z, v;
    bool expected;
  } cases[] = {
    /* zd ,  z0 ,  zb ,    z ,     v , expected */
    { 0.28, 0.26, 0.24, 0.280,  0.000, true  },  /* initial */
    { 0.28, 0.26, 0.24, 0.279, -0.001, true  },  /* falling */
    { 0.28, 0.26, 0.24, 0.270, -0.010, true  },  /* falling */
    { 0.28, 0.26, 0.24, 0.265, -0.020, true  },  /* falling */
    { 0.28, 0.26, 0.24, 0.260, -0.030, false },  /* touchdown */
    { 0.28, 0.26, 0.24, 0.250, -0.020, false },  /* compression */
    { 0.28, 0.26, 0.24, 0.240,  0.000, false },  /* bottom */
    { 0.28, 0.26, 0.24, 0.250,  0.020, false },  /* extension */
    { 0.28, 0.26, 0.24, 0.260,  0.030, true  },  /* lift-off */
    { 0.28, 0.26, 0.24, 0.265,  0.025, true  },  /* rising */
    { 0.28, 0.26, 0.24, 0.270,  0.020, true  },  /* rising */
    { 0.28, 0.26, 0.24, 0.275,  0.010, true  },  /* rising */
    { 0.28, 0.26, 0.24, 0.280,  0.000, true  },  /* apex */
    { 0.28, 0.26, 0.24, 0.279, -0.001, true  },  /* falling */
    { 0.28, 0.26, 0.24, 0.270, -0.010, true  },  /* falling */
    { 0.28, 0.26, 0.24, 0.265, -0.020, true  },  /* falling */
    { 0.28, 0.26, 0.24, 0.260, -0.030, false },  /* touchdown */
    {    0,    0,    0,     0,      0, false },  /* terminator */
  };
  struct case_t *c;

  for( c=cases; c->zd>0; c++ ){
    cmd.zd = c->zd;
    cmd.z0 = c->z0;
    cmd.zb = c->zb;
    vec_set_elem_list( p, c->z, c->v );
    ctrl_events_update( &events, 0, p, &cmd );
    if( c->expected )
      ASSERT_TRUE( ctrl_events_is_in_flight( &events ) );
    else
      ASSERT_FALSE( ctrl_events_is_in_flight( &events ) );
  }
}

TEST(test_ctrl_events_is_in_compression)
{
  struct case_t {
    double zd, z0, zb;
    double z, v;
    bool expected;
  } cases[] = {
    /* zd ,  z0 ,  zb ,    z ,     v , expected */
    { 0.28, 0.26, 0.24, 0.280,  0.000, false },  /* initial */
    { 0.28, 0.26, 0.24, 0.270, -0.010, false },  /* falling */
    { 0.28, 0.26, 0.24, 0.260, -0.030, true  },  /* touchdown */
    { 0.28, 0.26, 0.24, 0.255, -0.025, true  },  /* compression */
    { 0.28, 0.26, 0.24, 0.250, -0.020, true  },  /* compression */
    { 0.28, 0.26, 0.24, 0.245, -0.010, true  },  /* compression */
    { 0.28, 0.26, 0.24, 0.240,  0.000, false },  /* bottom */
    { 0.28, 0.26, 0.24, 0.241,  0.001, false },  /* extension */
    { 0.28, 0.26, 0.24, 0.250,  0.020, false },  /* extension */
    { 0.28, 0.26, 0.24, 0.260,  0.030, false },  /* lift-off */
    { 0.28, 0.26, 0.24, 0.270,  0.020, false },  /* rising */
    { 0.28, 0.26, 0.24, 0.280,  0.000, false },  /* apex */
    { 0.28, 0.26, 0.24, 0.270, -0.010, false },  /* falling */
    { 0.28, 0.26, 0.24, 0.260, -0.030, true  },  /* touchdown */
    { 0.28, 0.26, 0.24, 0.255, -0.025, true  },  /* compression */
    { 0.28, 0.26, 0.24, 0.250, -0.020, true  },  /* compression */
    { 0.28, 0.26, 0.24, 0.245, -0.010, true  },  /* compression */
    { 0.28, 0.26, 0.24, 0.240,  0.000, false },  /* bottom */
    { 0.28, 0.26, 0.24, 0.241,  0.001, false },  /* extension */
    {    0,    0,    0,     0,      0, false },  /* terminator */
  };
  struct case_t *c;

  for( c=cases; c->zd>0; c++ ){
    cmd.zd = c->zd;
    cmd.z0 = c->z0;
    cmd.zb = c->zb;
    vec_set_elem_list( p, c->z, c->v );
    ctrl_events_update( &events, 0, p, &cmd );
    if( c->expected )
      ASSERT_TRUE( ctrl_events_is_in_compression( &events ) );
    else
      ASSERT_FALSE( ctrl_events_is_in_compression( &events ) );
  }
}

TEST(test_ctrl_events_is_in_extension)
{
  struct case_t {
    double zd, z0, zb;
    double z, v;
    bool expected;
  } cases[] = {
    /* zd ,  z0 ,  zb ,    z ,     v , expected */
    { 0.28, 0.26, 0.24, 0.280,  0.000, false },  /* initial */
    { 0.28, 0.26, 0.24, 0.270, -0.010, false },  /* falling */
    { 0.28, 0.26, 0.24, 0.260, -0.030, false },  /* touchdown */
    { 0.28, 0.26, 0.24, 0.250, -0.020, false },  /* compression */
    { 0.28, 0.26, 0.24, 0.240,  0.000, true  },  /* bottom */
    { 0.28, 0.26, 0.24, 0.241,  0.001, true  },  /* extension */
    { 0.28, 0.26, 0.24, 0.245,  0.010, true  },  /* extension */
    { 0.28, 0.26, 0.24, 0.250,  0.020, true  },  /* extension */
    { 0.28, 0.26, 0.24, 0.255,  0.025, true  },  /* extension */
    { 0.28, 0.26, 0.24, 0.260,  0.030, false },  /* lift-off */
    { 0.28, 0.26, 0.24, 0.270,  0.020, false },  /* rising */
    { 0.28, 0.26, 0.24, 0.280,  0.000, false },  /* apex */
    { 0.28, 0.26, 0.24, 0.270, -0.010, false },  /* falling */
    { 0.28, 0.26, 0.24, 0.260, -0.030, false },  /* touchdown */
    { 0.28, 0.26, 0.24, 0.250, -0.020, false },  /* compression */
    { 0.28, 0.26, 0.24, 0.240,  0.000, true  },  /* bottom */
    { 0.28, 0.26, 0.24, 0.241,  0.001, true  },  /* extension */
    { 0.28, 0.26, 0.24, 0.245,  0.010, true  },  /* extension */
    { 0.28, 0.26, 0.24, 0.250,  0.020, true  },  /* extension */
    { 0.28, 0.26, 0.24, 0.255,  0.025, true  },  /* extension */
    { 0.28, 0.26, 0.24, 0.260,  0.030, false },  /* lift-off */
    {    0,    0,    0,     0,      0, false },  /* terminator */
  };
  struct case_t *c;

  for( c=cases; c->zd>0; c++ ){
    cmd.zd = c->zd;
    cmd.z0 = c->z0;
    cmd.zb = c->zb;
    vec_set_elem_list( p, c->z, c->v );
    ctrl_events_update( &events, 0, p, &cmd );
    if( c->expected )
      ASSERT_TRUE( ctrl_events_is_in_extension( &events ) );
    else
      ASSERT_FALSE( ctrl_events_is_in_extension( &events ) );
  }
}

TEST(test_ctrl_events_update_phase)
{
  struct case_t {
    double zd, z0, zb;
    double z, v;
    enum _ctrl_events_phases_t expected;
  } cases[] = {
    /* zd ,  z0 ,  zb ,    z ,     v , expected */
    { 0.28, 0.26, 0.24, 0.280,  0.000, falling     },  /* initial */
    { 0.28, 0.26, 0.24, 0.270, -0.010, falling     },  /* falling */
    { 0.28, 0.26, 0.24, 0.260, -0.030, compression },  /* touchdown */
    { 0.28, 0.26, 0.24, 0.250, -0.020, compression },  /* compression */
    { 0.28, 0.26, 0.24, 0.240,  0.000, extension   },  /* bottom */
    { 0.28, 0.26, 0.24, 0.250,  0.020, extension   },  /* extension */
    { 0.28, 0.26, 0.24, 0.260,  0.030, rising      },  /* lift-off */
    { 0.28, 0.26, 0.24, 0.270,  0.020, rising      },  /* rising */
    { 0.28, 0.26, 0.24, 0.280,  0.000, falling     },  /* apex */
    { 0.28, 0.26, 0.24, 0.270, -0.010, falling     },  /* falling */
    { 0.28, 0.26, 0.24, 0.260, -0.030, compression },  /* touchdown */
    { 0.28, 0.26, 0.24, 0.250, -0.020, compression },  /* compression */
    { 0.28, 0.26, 0.24, 0.240,  0.000, extension   },  /* bottom */
    { 0.28, 0.26, 0.24, 0.250,  0.020, extension   },  /* extension */
    { 0.28, 0.26, 0.24, 0.260,  0.030, rising      },  /* lift-off */
    { 0.28, 0.26, 0.24, 0.270,  0.020, rising      },  /* rising */
    { 0.28, 0.26, 0.24, 0.280,  0.000, falling     },  /* apex */
    {    0,    0,    0,     0,      0, invalid     },  /* terminator */
  };
  struct case_t *c;

  for( c=cases; c->zd>0; c++ ){
    cmd.zd = c->zd;
    cmd.z0 = c->z0;
    cmd.zb = c->zb;
    vec_set_elem_list( p, c->z, c->v );
    ctrl_events_update( &events, 0, p, &cmd );
    ASSERT_EQ( c->expected, ctrl_events_phase(&events) );
  }
}

TEST(test_ctrl_events_update_n)
{
  struct case_t {
    double zd, z0, zb;
    double z, v;
    int expected;
  } cases[] = {
    /* zd ,  z0 ,  zb ,    z ,     v , expected */
    { 0.28, 0.26, 0.24, 0.280,  0.000, 0 },  /* initial */
    { 0.28, 0.26, 0.24, 0.270, -0.010, 0 },  /* falling */
    { 0.28, 0.26, 0.24, 0.260, -0.030, 0 },  /* touchdown */
    { 0.28, 0.26, 0.24, 0.250, -0.020, 0 },  /* compression */
    { 0.28, 0.26, 0.24, 0.240,  0.000, 0 },  /* bottom */
    { 0.28, 0.26, 0.24, 0.250,  0.020, 0 },  /* extension */
    { 0.28, 0.26, 0.24, 0.260,  0.030, 0 },  /* lift-off */
    { 0.28, 0.26, 0.24, 0.270,  0.020, 0 },  /* rising */

    { 0.28, 0.26, 0.24, 0.280,  0.000, 1 },  /* apex */
    { 0.28, 0.26, 0.24, 0.270, -0.010, 1 },  /* falling */
    { 0.28, 0.26, 0.24, 0.260, -0.030, 1 },  /* touchdown */
    { 0.28, 0.26, 0.24, 0.250, -0.020, 1 },  /* compression */
    { 0.28, 0.26, 0.24, 0.240,  0.000, 1 },  /* bottom */
    { 0.28, 0.26, 0.24, 0.250,  0.020, 1 },  /* extension */
    { 0.28, 0.26, 0.24, 0.260,  0.030, 1 },  /* lift-off */
    { 0.28, 0.26, 0.24, 0.270,  0.020, 1 },  /* rising */
    { 0.28, 0.26, 0.24, 0.279,  0.029, 1 },  /* rising */

    { 0.28, 0.26, 0.24, 0.278, -0.002, 2 },  /* falling */
    { 0.28, 0.26, 0.24, 0.270, -0.010, 2 },  /* falling */
    { 0.28, 0.26, 0.24, 0.260, -0.030, 2 },  /* touchdown */
    { 0.28, 0.26, 0.24, 0.250, -0.020, 2 },  /* compression */
    { 0.28, 0.26, 0.24, 0.240,  0.000, 2 },  /* bottom */
    { 0.28, 0.26, 0.24, 0.250,  0.020, 2 },  /* extension */
    { 0.28, 0.26, 0.24, 0.260,  0.030, 2 },  /* lift-off */
    { 0.28, 0.26, 0.24, 0.270,  0.020, 2 },  /* rising */
    { 0.28, 0.26, 0.24, 0.278,  0.028, 2 },  /* rising */

    { 0.28, 0.26, 0.24, 0.279, -0.001, 3 },  /* falling */
    { 0.28, 0.26, 0.24, 0.270, -0.010, 3 },  /* falling */
    { 0.28, 0.26, 0.24, 0.260, -0.030, 3 },  /* touchdown */
    { 0.28, 0.26, 0.24, 0.250, -0.020, 3 },  /* compression */
    { 0.28, 0.26, 0.24, 0.240,  0.000, 3 },  /* bottom */
    { 0.28, 0.26, 0.24, 0.250,  0.020, 3 },  /* extension */
    { 0.28, 0.26, 0.24, 0.260,  0.030, 3 },  /* lift-off */
    { 0.28, 0.26, 0.24, 0.270,  0.020, 3 },  /* rising */

    { 0.28, 0.26, 0.24, 0.280,  0.000, 4 },  /* apex */
    { 0.28, 0.26, 0.24, 0.270, -0.010, 4 },  /* falling */

    {    0,    0,    0,     0,      0, 0 },  /* terminator */
  };
  struct case_t *c;

  for( c=cases; c->zd>0; c++ ){
    cmd.zd = c->zd;
    cmd.z0 = c->z0;
    cmd.zb = c->zb;
    vec_set_elem_list( p, c->z, c->v );
    ctrl_events_update( &events, 0, p, &cmd );
    ASSERT_EQ( c->expected, ctrl_events_n(&events) );
  }
}

TEST(test_ctrl_events_update_apex_1)
{
  struct case_t {
    /* parameters */
    double zd, z0, zb;
    /* states */
    double t, z, v;
    /* expected recorded events */
    struct _ctrl_events_tuple_t apex;
  } cases[] = {
    /* zd ,  z0 ,  zb ,   t ,    z ,    v ,apx:{ t,    z ,   v } */
    { 0.30, 0.25, 0.20, 0.00, 0.300,  0.00, { 0.00, 0.000, 0.00 }, },  /* initial */
    { 0.30, 0.25, 0.20, 0.01, 0.300, -0.01, { 0.00, 0.000, 0.00 }, },  /* falling */
    { 0.30, 0.25, 0.20, 0.10, 0.250, -0.30, { 0.00, 0.000, 0.00 }, },  /* touchdown */
    { 0.30, 0.25, 0.20, 0.20, 0.200,  0.00, { 0.00, 0.000, 0.00 }, },  /* bottom */

    { 0.30, 0.25, 0.20, 0.30, 0.250,  sqrt(0.1*G), { 0.30, 0.250, sqrt(0.1*G) }, },  /* lift-off */
    { 0.30, 0.25, 0.20, 0.31, 0.251,  0.29, { 0.31, 0.251, 0.29 }, },  /* rising */
    { 0.30, 0.25, 0.20, 0.32, 0.252,  0.28, { 0.32, 0.252, 0.28 }, },  /* rising */
    { 0.30, 0.25, 0.20, 0.39, 0.259,  0.01, { 0.39, 0.259, 0.01 }, },  /* rising */
    { 0.30, 0.25, 0.20, 0.40, 0.300,  0.00, { 0.40, 0.300, 0.00 }, },  /* apex */

    { 0.30, 0.25, 0.20, 0.41, 0.300, -0.01, { 0.40, 0.300, 0.00 }, },  /* falling */
    { 0.30, 0.25, 0.20, 0.50, 0.250, -0.30, { 0.40, 0.300, 0.00 }, },  /* touchdown */
    { 0.30, 0.25, 0.20, 0.60, 0.200,  0.00, { 0.40, 0.300, 0.00 }, },  /* bottom */

    { 0.30, 0.25, 0.20, 0.70, 0.250,  sqrt(0.1*G), { 0.70, 0.250, sqrt(0.1*G) }, },  /* lift-off */
    { 0.30, 0.25, 0.20, 0.71, 0.251,  0.29, { 0.71, 0.251, 0.29 }, },  /* rising */
    { 0.30, 0.25, 0.20, 0.72, 0.252,  0.28, { 0.72, 0.252, 0.28 }, },  /* rising */
    { 0.30, 0.25, 0.20, 0.79, 0.259,  0.01, { 0.79, 0.259, 0.01 }, },  /* rising */
    { 0.30, 0.25, 0.20, 0.80, 0.300,  0.00, { 0.80, 0.300, 0.00 }, },  /* apex */

    { 0.30, 0.25, 0.20, 0.81, 0.300, -0.01, { 0.80, 0.300, 0.00 }, },  /* falling */
    { 0.00, 0.00, 0.00, 0.00, 0.000,  0.00, { 0.00, 0.000, 0.00 }, },  /* terminator */
  };
  struct case_t *c;

  for( c=cases; c->zd>0; c++ ){
    cmd.zd = c->zd;
    cmd.z0 = c->z0;
    cmd.zb = c->zb;
    vec_set_elem_list( p, c->z, c->v );
    ctrl_events_update( &events, c->t, p, &cmd );
    ASSERT_EQ( c->apex.t, ctrl_events_apex_t(&events) );
    ASSERT_EQ( c->apex.z, ctrl_events_apex_z(&events) );
    ASSERT_EQ( c->apex.v, ctrl_events_apex_v(&events) );
  }
}

TEST(test_ctrl_events_update_apex_2)
{
  struct case_t {
    /* parameters */
    double zd, z0, zb;
    /* states */
    double t, z, v;
    /* expected recorded events */
    struct _ctrl_events_tuple_t apex;
  } cases[] = {
    /* zd ,  z0 ,  zb ,   t ,    z ,    v ,apx:{ t,    z ,   v } */
    { 0.30, 0.25, 0.20, 0.00, 0.300,  0.00, { 0.00, 0.000,  0.00 }, },  /* initial */
    { 0.30, 0.25, 0.20, 0.01, 0.300, -0.01, { 0.00, 0.000,  0.00 }, },  /* falling */
    { 0.30, 0.25, 0.20, 0.10, 0.250, -0.90, { 0.00, 0.000,  0.00 }, },  /* touchdown */
    { 0.30, 0.25, 0.20, 0.20, 0.200,  0.00, { 0.00, 0.000,  0.00 }, },  /* bottom */
    { 0.30, 0.25, 0.20, 0.29, 0.249,  0.90, { 0.00, 0.000,  0.00 }, },  /* extension */

    { 0.30, 0.25, 0.20, 0.30, 0.252,  0.80, { 0.30, 0.252,  0.80 }, },  /* rising */
    { 0.30, 0.25, 0.20, 0.31, 0.253,  0.78, { 0.31, 0.253,  0.78 }, },  /* rising */
    { 0.30, 0.25, 0.20, 0.39, 0.290,  0.02, { 0.39, 0.290,  0.02 }, },  /* rising */
    { 0.30, 0.25, 0.20, 0.40, 0.295,  0.01, { 0.40, 0.295,  0.01 }, },  /* rising */

    { 0.30, 0.25, 0.20, 0.41, 0.280, -0.01, { 0.40, 0.295,  0.01 }, },  /* falling */
    { 0.30, 0.25, 0.20, 0.42, 0.270, -0.02, { 0.40, 0.295,  0.01 }, },  /* falling */
    { 0.30, 0.25, 0.20, 0.50, 0.250, -0.30, { 0.40, 0.295,  0.01 }, },  /* touchdown */
    { 0.30, 0.25, 0.20, 0.60, 0.200,  0.00, { 0.40, 0.295,  0.01 }, },  /* bottom */
    { 0.30, 0.25, 0.20, 0.69, 0.245,  0.80, { 0.40, 0.295,  0.01 }, },  /* extension */

    { 0.30, 0.25, 0.20, 0.70, 0.251,  0.85, { 0.70, 0.251,  0.85 }, },  /* rising */
    { 0.30, 0.25, 0.20, 0.71, 0.252,  0.83, { 0.71, 0.252,  0.83 }, },  /* rising */
    { 0.30, 0.25, 0.20, 0.79, 0.259,  0.05, { 0.79, 0.259,  0.05 }, },  /* rising */
    { 0.30, 0.25, 0.20, 0.80, 0.280,  0.03, { 0.80, 0.280,  0.03 }, },  /* rising */
    { 0.30, 0.25, 0.20, 0.81, 0.295, -0.01, { 0.81, 0.295, -0.01 }, },  /* falling */

    { 0.30, 0.25, 0.20, 0.82, 0.290, -0.02, { 0.81, 0.295, -0.01 }, },  /* falling */
    { 0.00, 0.00, 0.00, 0.00, 0.000,  0.00, { 0.00, 0.000,  0.00 }, },  /* terminator */
  };
  struct case_t *c;

  for( c=cases; c->zd>0; c++ ){
    cmd.zd = c->zd;
    cmd.z0 = c->z0;
    cmd.zb = c->zb;
    vec_set_elem_list( p, c->z, c->v );
    ctrl_events_update( &events, c->t, p, &cmd );
    ASSERT_EQ( c->apex.t, ctrl_events_apex_t(&events) );
    ASSERT_EQ( c->apex.z, ctrl_events_apex_z(&events) );
    ASSERT_EQ( c->apex.v, ctrl_events_apex_v(&events) );
  }
}

TEST(test_ctrl_events_update_touchdown_1)
{
  struct case_t {
    /* parameters */
    double zd, z0, zb;
    /* states */
    double t, z, v;
    /* expected recorded events */
    struct _ctrl_events_tuple_t touchdown;
  } cases[] = {
    /* zd ,  z0 ,  zb ,   t ,    z ,    v , td:{ t,    z ,   v } */
    { 0.30, 0.25, 0.20, 0.00, 0.300,  0.00,
                      { 0.00, 0.300,  0.00 }, },         /* initial */
    { 0.30, 0.25, 0.20, 0.01, 0.299, -0.01,
                      { 0.01, 0.299, -0.01 }, },         /* falling */
    { 0.30, 0.25, 0.20, 0.09, 0.255, -0.90,
                      { 0.09, 0.255, -0.90 }, },         /* falling */
    { 0.30, 0.25, 0.20, 0.10, 0.250, -sqrt(0.1*G),
                      { 0.10, 0.250, -sqrt(0.1*G) }, },  /* touchdown */

    { 0.30, 0.25, 0.20, 0.11, 0.249, -0.90,
                      { 0.10, 0.250, -sqrt(0.1*G) }, },  /* compression */
    { 0.30, 0.25, 0.20, 0.20, 0.200,  0.00,
                      { 0.10, 0.250, -sqrt(0.1*G) }, },  /* bottom */
    { 0.30, 0.25, 0.20, 0.30, 0.250,  sqrt(0.1*G),
                      { 0.10, 0.250, -sqrt(0.1*G) }, },  /* lift-off */

    { 0.30, 0.25, 0.20, 0.40, 0.300,  0.00,
                      { 0.40, 0.300,  0.00 }, },         /* apex */
    { 0.30, 0.25, 0.20, 0.41, 0.299, -0.01,
                      { 0.41, 0.299, -0.01 }, },         /* falling */
    { 0.30, 0.25, 0.20, 0.49, 0.255, -0.90,
                      { 0.49, 0.255, -0.90 }, },         /* falling */
    { 0.30, 0.25, 0.20, 0.50, 0.250, -sqrt(0.1*G),
                      { 0.50, 0.250, -sqrt(0.1*G) }, },  /* touchdown */

    { 0.30, 0.25, 0.20, 0.51, 0.249, -0.90,
                      { 0.50, 0.250, -sqrt(0.1*G) }, },  /* compression */
    { 0.00, 0.00, 0.00, 0.00, 0.000,  0.00, { 0.00, 0.000, 0.00 }, },  /* terminator */
  };
  struct case_t *c;

  for( c=cases; c->zd>0; c++ ){
    cmd.zd = c->zd;
    cmd.z0 = c->z0;
    cmd.zb = c->zb;
    vec_set_elem_list( p, c->z, c->v );
    ctrl_events_update( &events, c->t, p, &cmd );
    ASSERT_EQ( c->touchdown.t, ctrl_events_touchdown_t(&events) );
    ASSERT_EQ( c->touchdown.z, ctrl_events_touchdown_z(&events) );
    ASSERT_EQ( c->touchdown.v, ctrl_events_touchdown_v(&events) );
  }
}

TEST(test_ctrl_events_update_touchdown_2)
{
  struct case_t {
    /* parameters */
    double zd, z0, zb;
    /* states */
    double t, z, v;
    /* expected recorded events */
    struct _ctrl_events_tuple_t touchdown;
  } cases[] = {
    /* zd ,  z0 ,  zb ,   t ,    z ,    v , td:{ t,    z ,   v } */
    { 0.30, 0.25, 0.20, 0.00, 0.300,  0.00,
                      { 0.00, 0.300,  0.00 }, },         /* initial */
    { 0.30, 0.25, 0.20, 0.01, 0.299, -0.01,
                      { 0.01, 0.299, -0.01 }, },         /* falling */
    { 0.30, 0.25, 0.20, 0.09, 0.260, -0.80,
                      { 0.09, 0.260, -0.80 }, },         /* falling */
    { 0.30, 0.25, 0.20, 0.10, 0.249, -0.90,
                      { 0.10, 0.249, -0.90 }, },         /* compression */

    { 0.30, 0.25, 0.20, 0.11, 0.240, -0.80,
                      { 0.10, 0.249, -0.90 }, },         /* compression */
    { 0.30, 0.25, 0.20, 0.20, 0.200,  0.00,
                      { 0.10, 0.249, -0.90 }, },         /* bottom */
    { 0.30, 0.25, 0.20, 0.30, 0.250,  sqrt(0.1*G),
                      { 0.10, 0.249, -0.90 }, },         /* lift-off */
    { 0.30, 0.25, 0.20, 0.39, 0.299,  0.01,
                      { 0.10, 0.249, -0.90 }, },         /* rising */

    { 0.30, 0.25, 0.20, 0.40, 0.295, -0.02,
                      { 0.40, 0.295, -0.02 }, },         /* falling */
    { 0.30, 0.25, 0.20, 0.41, 0.280, -0.04,
                      { 0.41, 0.280, -0.04 }, },         /* falling */
    { 0.30, 0.25, 0.20, 0.49, 0.251, -0.90,
                      { 0.49, 0.251, -0.90 }, },         /* falling */

    { 0.30, 0.25, 0.20, 0.50, 0.248, -0.80,
                      { 0.49, 0.251, -0.90 }, },         /* compression */
    { 0.30, 0.25, 0.20, 0.60, 0.200,  0.00,
                      { 0.49, 0.251, -0.90 }, },         /* bottom */
    { 0.30, 0.25, 0.20, 0.70, 0.250,  sqrt(0.1*G),
                      { 0.49, 0.251, -0.90 }, },         /* lift-off */
    { 0.30, 0.25, 0.20, 0.79, 0.290,  0.03,
                      { 0.49, 0.251, -0.90 }, },         /* rising */

    { 0.30, 0.25, 0.20, 0.80, 0.295, -0.01,
                      { 0.80, 0.295, -0.01 }, },         /* falling */
    { 0.00, 0.00, 0.00, 0.00, 0.000,  0.00,
                      { 0.00, 0.000,  0.00 }, },         /* terminator */
  };
  struct case_t *c;

  for( c=cases; c->zd>0; c++ ){
    cmd.zd = c->zd;
    cmd.z0 = c->z0;
    cmd.zb = c->zb;
    vec_set_elem_list( p, c->z, c->v );
    ctrl_events_update( &events, c->t, p, &cmd );
    ASSERT_EQ( c->touchdown.t, ctrl_events_touchdown_t(&events) );
    ASSERT_EQ( c->touchdown.z, ctrl_events_touchdown_z(&events) );
    ASSERT_EQ( c->touchdown.v, ctrl_events_touchdown_v(&events) );
  }
}

TEST(test_ctrl_events_update_bottom_1)
{
  struct case_t {
    /* parameters */
    double zd, z0, zb;
    /* states */
    double t, z, v;
    /* expected recorded events */
    struct _ctrl_events_tuple_t bottom;
  } cases[] = {
    /* zd ,  z0 ,  zb ,   t ,    z ,    v ,btm:{ t,    z ,   v } */
    { 0.30, 0.25, 0.20, 0.00, 0.300,  0.00,
                      { 0.00, 0.000,  0.00 }, },         /* initial */
    { 0.30, 0.25, 0.20, 0.01, 0.299, -0.01,
                      { 0.00, 0.000,  0.00 }, },         /* falling */

    { 0.30, 0.25, 0.20, 0.10, 0.250, -sqrt(0.1*G),
                      { 0.10, 0.250, -sqrt(0.1*G) }, },  /* touchdown */
    { 0.30, 0.25, 0.20, 0.11, 0.249, -0.90,
                      { 0.11, 0.249, -0.90 }, },         /* compression */
    { 0.30, 0.25, 0.20, 0.19, 0.201, -0.01,
                      { 0.19, 0.201, -0.01 }, },         /* compression */
    { 0.30, 0.25, 0.20, 0.20, 0.200,  0.00,
                      { 0.20, 0.200,  0.00 }, },         /* bottom */

    { 0.30, 0.25, 0.20, 0.21, 0.201,  0.01,
                      { 0.20, 0.200,  0.00 }, },         /* extension */
    { 0.30, 0.25, 0.20, 0.30, 0.250,  sqrt(0.1*G),
                      { 0.20, 0.200,  0.00 }, },         /* lift-off */
    { 0.30, 0.25, 0.20, 0.40, 0.300,  0.00,
                      { 0.20, 0.200,  0.00 }, },         /* apex */
    { 0.30, 0.25, 0.20, 0.49, 0.255, -0.90,
                      { 0.20, 0.200,  0.00 }, },         /* falling */

    { 0.30, 0.25, 0.20, 0.50, 0.250, -sqrt(0.1*G),
                      { 0.50, 0.250, -sqrt(0.1*G) }, },  /* touchdown */
    { 0.30, 0.25, 0.20, 0.51, 0.249, -0.90,
                      { 0.51, 0.249, -0.90 }, },         /* compression */
    { 0.30, 0.25, 0.20, 0.59, 0.201, -0.01,
                      { 0.59, 0.201, -0.01 }, },         /* compression */
    { 0.30, 0.25, 0.20, 0.60, 0.200,  0.00,
                      { 0.60, 0.200,  0.00 }, },         /* bottom */

    { 0.30, 0.25, 0.20, 0.61, 0.201,  0.01,
                      { 0.60, 0.200,  0.00 }, },         /* extension */
    { 0.00, 0.00, 0.00, 0.00, 0.000,  0.00,
                      { 0.00, 0.000,  0.00 }, },         /* terminator */
  };
  struct case_t *c;

  for( c=cases; c->zd>0; c++ ){
    cmd.zd = c->zd;
    cmd.z0 = c->z0;
    cmd.zb = c->zb;
    vec_set_elem_list( p, c->z, c->v );
    ctrl_events_update( &events, c->t, p, &cmd );
    ASSERT_EQ( c->bottom.t, ctrl_events_bottom_t(&events) );
    ASSERT_EQ( c->bottom.z, ctrl_events_bottom_z(&events) );
    ASSERT_EQ( c->bottom.v, ctrl_events_bottom_v(&events) );
  }
}

TEST(test_ctrl_events_update_bottom_2)
{
  struct case_t {
    /* parameters */
    double zd, z0, zb;
    /* states */
    double t, z, v;
    /* expected recorded events */
    struct _ctrl_events_tuple_t bottom;
  } cases[] = {
    /* zd ,  z0 ,  zb ,   t ,    z ,    v ,btm:{ t,    z ,   v } */
    { 0.30, 0.25, 0.20, 0.00, 0.300,  0.00,
                      { 0.00, 0.000,  0.00 }, },         /* initial */
    { 0.30, 0.25, 0.20, 0.01, 0.299, -0.01,
                      { 0.00, 0.000,  0.00 }, },         /* falling */
    { 0.30, 0.25, 0.20, 0.09, 0.251, -0.90,
                      { 0.00, 0.000,  0.00 }, },         /* falling */

    { 0.30, 0.25, 0.20, 0.10, 0.248, -0.80,
                      { 0.10, 0.248, -0.80 }, },         /* compression */
    { 0.30, 0.25, 0.20, 0.11, 0.240, -0.70,
                      { 0.11, 0.240, -0.70 }, },         /* compression */
    { 0.30, 0.25, 0.20, 0.19, 0.201, -0.01,
                      { 0.19, 0.201, -0.01 }, },         /* compression */

    { 0.30, 0.25, 0.20, 0.20, 0.205,  0.02,
                      { 0.19, 0.201, -0.01 }, },         /* extension */
    { 0.30, 0.25, 0.20, 0.22, 0.210,  0.05,
                      { 0.19, 0.201, -0.01 }, },         /* extension */
    { 0.30, 0.25, 0.20, 0.30, 0.250,  sqrt(0.1*G),
                      { 0.19, 0.201, -0.01 }, },         /* lift-off */
    { 0.30, 0.25, 0.20, 0.40, 0.300,  0.00,
                      { 0.19, 0.201, -0.01 }, },         /* apex */
    { 0.30, 0.25, 0.20, 0.49, 0.255, -0.80,
                      { 0.19, 0.201, -0.01 }, },         /* falling */

    { 0.30, 0.25, 0.20, 0.50, 0.249, -0.90,
                      { 0.50, 0.249, -0.90 }, },         /* compression */
    { 0.30, 0.25, 0.20, 0.51, 0.240, -0.70,
                      { 0.51, 0.240, -0.70 }, },         /* compression */
    { 0.30, 0.25, 0.20, 0.59, 0.205, -0.02,
                      { 0.59, 0.205, -0.02 }, },         /* compression */
    { 0.30, 0.25, 0.20, 0.60, 0.201,  0.01,
                      { 0.60, 0.201,  0.01 }, },         /* extension */

    { 0.30, 0.25, 0.20, 0.61, 0.205,  0.02,
                      { 0.60, 0.201,  0.01 }, },         /* extension */
    { 0.00, 0.00, 0.00, 0.00, 0.000,  0.00,
                      { 0.00, 0.000,  0.00 }, },         /* terminator */
  };
  struct case_t *c;

  for( c=cases; c->zd>0; c++ ){
    cmd.zd = c->zd;
    cmd.z0 = c->z0;
    cmd.zb = c->zb;
    vec_set_elem_list( p, c->z, c->v );
    ctrl_events_update( &events, c->t, p, &cmd );
    ASSERT_EQ( c->bottom.t, ctrl_events_bottom_t(&events) );
    ASSERT_EQ( c->bottom.z, ctrl_events_bottom_z(&events) );
    ASSERT_EQ( c->bottom.v, ctrl_events_bottom_v(&events) );
  }
}

TEST(test_ctrl_events_update_liftoff_1)
{
  struct case_t {
    /* parameters */
    double zd, z0, zb;
    /* states */
    double t, z, v;
    /* expected recorded events */
    struct _ctrl_events_tuple_t liftoff;
  } cases[] = {
    /* zd ,  z0 ,  zb ,   t ,    z ,    v , lo:{ t,    z ,   v } */
    { 0.30, 0.25, 0.20, 0.00, 0.300,  0.00,
                      { 0.00, 0.000,  0.00 }, },         /* initial */
    { 0.30, 0.25, 0.20, 0.01, 0.299, -0.01,
                      { 0.00, 0.000,  0.00 }, },         /* falling */
    { 0.30, 0.25, 0.20, 0.10, 0.250, -sqrt(0.1*G),
                      { 0.00, 0.000,  0.00 }, },         /* touchdown */

    { 0.30, 0.25, 0.20, 0.20, 0.200,  0.00,
                      { 0.20, 0.200,  0.00 }, },         /* bottom */
    { 0.30, 0.25, 0.20, 0.21, 0.201,  0.01,
                      { 0.21, 0.201,  0.01 }, },         /* extension */
    { 0.30, 0.25, 0.20, 0.29, 0.249,  0.90,
                      { 0.29, 0.249,  0.90 }, },         /* extension */
    { 0.30, 0.25, 0.20, 0.30, 0.250,  sqrt(0.1*G),
                      { 0.30, 0.250,  sqrt(0.1*G) }, },  /* lift-off */

    { 0.30, 0.25, 0.20, 0.31, 0.251,  0.90,
                      { 0.30, 0.250,  sqrt(0.1*G) }, },  /* rising */
    { 0.30, 0.25, 0.20, 0.40, 0.300,  0.00,
                      { 0.30, 0.250,  sqrt(0.1*G) }, },  /* apex */
    { 0.30, 0.25, 0.20, 0.50, 0.250, -sqrt(0.1*G),
                      { 0.30, 0.250,  sqrt(0.1*G) }, },  /* touchdown */

    { 0.30, 0.25, 0.20, 0.60, 0.200,  0.00,
                      { 0.60, 0.200,  0.00 }, },         /* bottom */
    { 0.30, 0.25, 0.20, 0.61, 0.201,  0.01,
                      { 0.61, 0.201,  0.01 }, },         /* extension */
    { 0.30, 0.25, 0.20, 0.69, 0.249,  0.90,
                      { 0.69, 0.249,  0.90 }, },         /* extension */
    { 0.30, 0.25, 0.20, 0.70, 0.250,  sqrt(0.1*G),
                      { 0.70, 0.250,  sqrt(0.1*G) }, },  /* lift-off */

    { 0.30, 0.25, 0.20, 0.71, 0.251,  0.90,
                      { 0.70, 0.250,  sqrt(0.1*G) }, },  /* rising */
    { 0.00, 0.00, 0.00, 0.00, 0.000,  0.00,
                      { 0.00, 0.000,  0.00 }, },         /* terminator */
  };
  struct case_t *c;

  for( c=cases; c->zd>0; c++ ){
    cmd.zd = c->zd;
    cmd.z0 = c->z0;
    cmd.zb = c->zb;
    vec_set_elem_list( p, c->z, c->v );
    ctrl_events_update( &events, c->t, p, &cmd );
    ASSERT_EQ( c->liftoff.t, ctrl_events_liftoff_t(&events) );
    ASSERT_EQ( c->liftoff.z, ctrl_events_liftoff_z(&events) );
    ASSERT_EQ( c->liftoff.v, ctrl_events_liftoff_v(&events) );
  }
}

TEST(test_ctrl_events_update_liftoff_2)
{
  struct case_t {
    /* parameters */
    double zd, z0, zb;
    /* states */
    double t, z, v;
    /* expected recorded events */
    struct _ctrl_events_tuple_t liftoff;
  } cases[] = {
    /* zd ,  z0 ,  zb ,   t ,    z ,    v , lo:{ t,    z ,   v } */
    { 0.30, 0.25, 0.20, 0.00, 0.300,  0.00,
                      { 0.00, 0.000,  0.00 }, },         /* initial */
    { 0.30, 0.25, 0.20, 0.01, 0.299, -0.01,
                      { 0.00, 0.000,  0.00 }, },         /* falling */
    { 0.30, 0.25, 0.20, 0.10, 0.250, -sqrt(0.1*G),
                      { 0.00, 0.000,  0.00 }, },         /* touchdown */
    { 0.30, 0.25, 0.20, 0.19, 0.201, -0.01,
                      { 0.00, 0.000,  0.00 }, },         /* compression */

    { 0.30, 0.25, 0.20, 0.20, 0.202,  0.02,
                      { 0.20, 0.202,  0.02 }, },         /* extension */
    { 0.30, 0.25, 0.20, 0.21, 0.205,  0.04,
                      { 0.21, 0.205,  0.04 }, },         /* extension */
    { 0.30, 0.25, 0.20, 0.29, 0.249,  0.90,
                      { 0.29, 0.249,  0.90 }, },         /* extension */

    { 0.30, 0.25, 0.20, 0.30, 0.252,  0.85,
                      { 0.29, 0.249,  0.90 }, },         /* rising */
    { 0.30, 0.25, 0.20, 0.31, 0.255,  0.80,
                      { 0.29, 0.249,  0.90 }, },         /* rising */
    { 0.30, 0.25, 0.20, 0.40, 0.300,  0.00,
                      { 0.29, 0.249,  0.90 }, },         /* apex */
    { 0.30, 0.25, 0.20, 0.50, 0.250, -sqrt(0.1*G),
                      { 0.29, 0.249,  0.90 }, },         /* touchdown */
    { 0.30, 0.25, 0.20, 0.59, 0.195, -0.05,
                      { 0.29, 0.249,  0.90 }, },         /* compression */

    { 0.30, 0.25, 0.20, 0.60, 0.201,  0.01,
                      { 0.60, 0.201,  0.01 }, },         /* extension */
    { 0.30, 0.25, 0.20, 0.61, 0.205,  0.02,
                      { 0.61, 0.205,  0.02 }, },         /* extension */
    { 0.30, 0.25, 0.20, 0.69, 0.248,  0.80,
                      { 0.69, 0.248,  0.80 }, },         /* extension */
    { 0.30, 0.25, 0.20, 0.70, 0.251,  0.90,
                      { 0.70, 0.251,  0.90, } },         /* rising */

    { 0.30, 0.25, 0.20, 0.71, 0.252,  0.85,
                      { 0.70, 0.251,  0.90 }, },         /* rising */
    { 0.00, 0.00, 0.00, 0.00, 0.000,  0.00,
                      { 0.00, 0.000,  0.00 }, },         /* terminator */
  };
  struct case_t *c;

  for( c=cases; c->zd>0; c++ ){
    cmd.zd = c->zd;
    cmd.z0 = c->z0;
    cmd.zb = c->zb;
    vec_set_elem_list( p, c->z, c->v );
    ctrl_events_update( &events, c->t, p, &cmd );
    ASSERT_EQ( c->liftoff.t, ctrl_events_liftoff_t(&events) );
    ASSERT_EQ( c->liftoff.z, ctrl_events_liftoff_z(&events) );
    ASSERT_EQ( c->liftoff.v, ctrl_events_liftoff_v(&events) );
  }
}

TEST_SUITE(test_ctrl_events)
{
  CONFIGURE_SUITE(setup_events, teardown_events);
  RUN_TEST(test_ctrl_events_init);
  RUN_TEST(test_ctrl_events_destroy);
  RUN_TEST(test_ctrl_events_calc_phase_complex);
  RUN_TEST(test_ctrl_events_calc_phi);
  RUN_TEST(test_ctrl_events_is_in_rising);
  RUN_TEST(test_ctrl_events_is_in_falling);
  RUN_TEST(test_ctrl_events_is_in_flight);
  RUN_TEST(test_ctrl_events_is_in_compression);
  RUN_TEST(test_ctrl_events_is_in_extension);
  RUN_TEST(test_ctrl_events_update_phase);
  RUN_TEST(test_ctrl_events_update_n);
  RUN_TEST(test_ctrl_events_update_apex_1);
  RUN_TEST(test_ctrl_events_update_apex_2);
  RUN_TEST(test_ctrl_events_update_touchdown_1);
  RUN_TEST(test_ctrl_events_update_touchdown_2);
  RUN_TEST(test_ctrl_events_update_bottom_1);
  RUN_TEST(test_ctrl_events_update_bottom_2);
  RUN_TEST(test_ctrl_events_update_liftoff_1);
  RUN_TEST(test_ctrl_events_update_liftoff_2);
}


static model_t model;
static ctrl_t ctrl;

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
  ASSERT_PTREQ( ctrl_header_default, ctrl._header );
  ASSERT_PTREQ( ctrl_writer_default, ctrl._writer );
  ASSERT_PTREQ( NULL, ctrl.prp);
  ASSERT_EQ( 0, ctrl_n( &ctrl ) );
  ASSERT_EQ( 0, ctrl_phi( &ctrl ) );
  ASSERT_EQ( invalid, ctrl_events_phase( ctrl_events( &ctrl ) ) );
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

TEST(test_ctrl_phase)
{
  double v0 = sqrt( 0.04 * G );
  struct case_t{
    /* parameters */
    double zd, z0, zb;
    /* states */
    double t, z, v;
    /* expectations */
    enum _ctrl_events_phases_t phase;
    bool flight, falling, compression, extension, rising;
  } cases[] = {
    { 0.28, 0.26, 0.24, 0.00, 0.28,  0.00, falling, true, true, false, false, false }, /* apex */
    { 0.28, 0.26, 0.24, 0.06, 0.27, -0.30, falling, true, true, false, false, false }, /* falling */
    { 0.28, 0.26, 0.24, 0.12, 0.26, -v0,   compression, false, false, true, false, false }, /* touchdown */
    { 0.28, 0.26, 0.24, 0.18, 0.25, -0.30, compression, false, false, true, false, false }, /* compression */
    { 0.28, 0.26, 0.24, 0.24, 0.24,  0.00, extension, false, false, false, true, false }, /* bottom */
    { 0.28, 0.26, 0.24, 0.30, 0.25,  0.30, extension, false, false, false, true, false }, /* extension */
    { 0.28, 0.26, 0.24, 0.36, 0.26,  v0,   rising, true, false, false, false, true }, /* lift-off */
    { 0.28, 0.26, 0.24, 0.42, 0.27,  0.30, rising, true, false, false, false, true }, /* rising */
    { 0.28, 0.26, 0.24, 0.48, 0.28,  0.00, falling, true, true, false, false, false }, /* apex */
    { 0, 0, 0, 0, 0, 0, invalid, false, false, false, false, false },
  };
  struct case_t *c;

  for( c=cases; c->phase != invalid; c++ ){
    vec_set_elem_list( p, c->z, c->v );
    ctrl_update( &ctrl, c->t, p );
    ASSERT_EQ( c->phase, ctrl_phase( &ctrl ) );
    ASSERT_EQ( c->flight, ctrl_phase_in( &ctrl, flight ) );
    ASSERT_EQ( c->falling, ctrl_phase_in( &ctrl, falling ) );
    ASSERT_EQ( c->compression, ctrl_phase_in( &ctrl, compression ) );
    ASSERT_EQ( c->extension, ctrl_phase_in( &ctrl, extension ) );
    ASSERT_EQ( c->rising, ctrl_phase_in( &ctrl, rising ) );
  }
}

TEST(test_ctrl_events_at_apex)
{
  double v0 = sqrt( 0.04 * G );
  struct case_t{
    /* parameters */
    double zd, z0, zb;
    /* states */
    double t, z, v;
    /* expectations */
    struct _ctrl_events_tuple_t apex;
  } cases[] = {
    { 0.28, 0.26, 0.24, 0.00, 0.28,  0.00, { 0.00, 0.00,  0.00 } }, /* apex */
    { 0.28, 0.26, 0.24, 0.06, 0.27, -0.30, { 0.00, 0.00,  0.00 } }, /* falling */
    { 0.28, 0.26, 0.24, 0.12, 0.26, -v0,   { 0.00, 0.00,  0.00 } }, /* touchdown */
    { 0.28, 0.26, 0.24, 0.18, 0.25, -0.30, { 0.00, 0.00,  0.00 } }, /* compression */
    { 0.28, 0.26, 0.24, 0.24, 0.24,  0.00, { 0.00, 0.00,  0.00 } }, /* bottom */
    { 0.28, 0.26, 0.24, 0.30, 0.25,  0.30, { 0.00, 0.00,  0.00 } }, /* extension */
    { 0.28, 0.26, 0.24, 0.36, 0.26,  v0,   { 0.36, 0.26,  v0   } }, /* lift-off */
    { 0.28, 0.26, 0.24, 0.42, 0.27,  0.30, { 0.42, 0.27,  0.30 } }, /* rising */
    { 0.28, 0.26, 0.24, 0.48, 0.28,  0.00, { 0.48, 0.28,  0.00 } }, /* apex */
    { 0, 0, 0, 0, 0, 0, { 0.00, 0.00,  0.00 } },
  };
  struct case_t *c;

  for( c=cases; c->zd > 0; c++ ){
    vec_set_elem_list( p, c->z, c->v );
    ctrl_update( &ctrl, c->t, p );
    ASSERT_EQ( c->apex.t, ctrl_events_at( &ctrl, apex ).t );
    ASSERT_EQ( c->apex.z, ctrl_events_at( &ctrl, apex ).z );
    ASSERT_EQ( c->apex.v, ctrl_events_at( &ctrl, apex ).v );
  }
}

TEST(test_ctrl_events_at_touchdown)
{
  double v0 = sqrt( 0.04 * G );
  struct case_t{
    /* parameters */
    double zd, z0, zb;
    /* states */
    double t, z, v;
    /* expectations */
    struct _ctrl_events_tuple_t touchdown;
  } cases[] = {
    { 0.28, 0.26, 0.24, 0.00, 0.28,  0.00, { 0.00, 0.28,  0.00 } }, /* apex */
    { 0.28, 0.26, 0.24, 0.06, 0.27, -0.30, { 0.06, 0.27, -0.30 } }, /* falling */
    { 0.28, 0.26, 0.24, 0.12, 0.26, -v0,   { 0.12, 0.26, -v0   } }, /* touchdown */
    { 0.28, 0.26, 0.24, 0.18, 0.25, -0.30, { 0.12, 0.26, -v0   } }, /* compression */
    { 0.28, 0.26, 0.24, 0.24, 0.24,  0.00, { 0.12, 0.26, -v0   } }, /* bottom */
    { 0.28, 0.26, 0.24, 0.30, 0.25,  0.30, { 0.12, 0.26, -v0   } }, /* extension */
    { 0.28, 0.26, 0.24, 0.36, 0.26,  v0,   { 0.12, 0.26, -v0   } }, /* lift-off */
    { 0.28, 0.26, 0.24, 0.42, 0.27,  0.30, { 0.12, 0.26, -v0   } }, /* rising */
    { 0.28, 0.26, 0.24, 0.48, 0.28,  0.00, { 0.48, 0.28,  0.00 } }, /* apex */
    { 0, 0, 0, 0, 0, 0, { 0.00, 0.00,  0.00 } },
  };
  struct case_t *c;

  for( c=cases; c->zd > 0; c++ ){
    vec_set_elem_list( p, c->z, c->v );
    ctrl_update( &ctrl, c->t, p );
    ASSERT_EQ( c->touchdown.t, ctrl_events_at( &ctrl, touchdown ).t );
    ASSERT_EQ( c->touchdown.z, ctrl_events_at( &ctrl, touchdown ).z );
    ASSERT_EQ( c->touchdown.v, ctrl_events_at( &ctrl, touchdown ).v );
  }
}

TEST(test_ctrl_events_at_bottom)
{
  double v0 = sqrt( 0.04 * G );
  struct case_t{
    /* parameters */
    double zd, z0, zb;
    /* states */
    double t, z, v;
    /* expectations */
    struct _ctrl_events_tuple_t bottom;
  } cases[] = {
    { 0.28, 0.26, 0.24, 0.00, 0.28,  0.00, { 0.00, 0.00,  0.00 } }, /* apex */
    { 0.28, 0.26, 0.24, 0.06, 0.27, -0.30, { 0.00, 0.00,  0.00 } }, /* falling */
    { 0.28, 0.26, 0.24, 0.12, 0.26, -v0,   { 0.12, 0.26, -v0   } }, /* touchdown */
    { 0.28, 0.26, 0.24, 0.18, 0.25, -0.30, { 0.18, 0.25, -0.30 } }, /* compression */
    { 0.28, 0.26, 0.24, 0.24, 0.24,  0.00, { 0.24, 0.24,  0.00 } }, /* bottom */
    { 0.28, 0.26, 0.24, 0.30, 0.25,  0.30, { 0.24, 0.24,  0.00 } }, /* extension */
    { 0.28, 0.26, 0.24, 0.36, 0.26,  v0,   { 0.24, 0.24,  0.00 } }, /* lift-off */
    { 0.28, 0.26, 0.24, 0.42, 0.27,  0.30, { 0.24, 0.24,  0.00 } }, /* rising */
    { 0.28, 0.26, 0.24, 0.48, 0.28,  0.00, { 0.24, 0.24,  0.00 } }, /* apex */
    { 0, 0, 0, 0, 0, 0, { 0.00, 0.00,  0.00 } },
  };
  struct case_t *c;

  for( c=cases; c->zd > 0; c++ ){
    vec_set_elem_list( p, c->z, c->v );
    ctrl_update( &ctrl, c->t, p );
    ASSERT_EQ( c->bottom.t, ctrl_events_at( &ctrl, bottom ).t );
    ASSERT_EQ( c->bottom.z, ctrl_events_at( &ctrl, bottom ).z );
    ASSERT_EQ( c->bottom.v, ctrl_events_at( &ctrl, bottom ).v );
  }
}

TEST(test_ctrl_events_at_liftoff)
{
  double v0 = sqrt( 0.04 * G );
  struct case_t{
    /* parameters */
    double zd, z0, zb;
    /* states */
    double t, z, v;
    /* expectations */
    struct _ctrl_events_tuple_t liftoff;
  } cases[] = {
    { 0.28, 0.26, 0.24, 0.00, 0.28,  0.00, { 0.00, 0.00,  0.00 } }, /* apex */
    { 0.28, 0.26, 0.24, 0.06, 0.27, -0.30, { 0.00, 0.00,  0.00 } }, /* falling */
    { 0.28, 0.26, 0.24, 0.12, 0.26, -v0,   { 0.00, 0.00,  0.00 } }, /* touchdown */
    { 0.28, 0.26, 0.24, 0.18, 0.25, -0.30, { 0.00, 0.00,  0.00 } }, /* compression */
    { 0.28, 0.26, 0.24, 0.24, 0.24,  0.00, { 0.24, 0.24,  0.00 } }, /* bottom */
    { 0.28, 0.26, 0.24, 0.30, 0.25,  0.30, { 0.30, 0.25,  0.30 } }, /* extension */
    { 0.28, 0.26, 0.24, 0.36, 0.26,  v0,   { 0.36, 0.26,  v0   } }, /* lift-off */
    { 0.28, 0.26, 0.24, 0.42, 0.27,  0.30, { 0.36, 0.26,  v0   } }, /* rising */
    { 0.28, 0.26, 0.24, 0.48, 0.28,  0.00, { 0.36, 0.26,  v0   } }, /* apex */
    { 0, 0, 0, 0, 0, 0, { 0.00, 0.00,  0.00 } },
  };
  struct case_t *c;

  for( c=cases; c->zd > 0; c++ ){
    vec_set_elem_list( p, c->z, c->v );
    ctrl_update( &ctrl, c->t, p );
    ASSERT_EQ( c->liftoff.t, ctrl_events_at( &ctrl, liftoff ).t );
    ASSERT_EQ( c->liftoff.z, ctrl_events_at( &ctrl, liftoff ).z );
    ASSERT_EQ( c->liftoff.v, ctrl_events_at( &ctrl, liftoff ).v );
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
  RUN_TEST(test_ctrl_v0);
  RUN_TEST(test_ctrl_update_default);
  RUN_TEST(test_ctrl_phase);
  RUN_TEST(test_ctrl_events_at_apex);
  RUN_TEST(test_ctrl_events_at_touchdown);
  RUN_TEST(test_ctrl_events_at_bottom);
  RUN_TEST(test_ctrl_events_at_liftoff);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_ctrl_events);
  RUN_SUITE(test_ctrl);
  TEST_REPORT();
  TEST_EXIT();
}
