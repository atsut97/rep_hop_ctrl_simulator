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
  ASSERT_EQ( 0, ctrl_events_touchdown_t(&events) );
  ASSERT_EQ( 0, ctrl_events_touchdown_z(&events) );
  ASSERT_EQ( 0, ctrl_events_bottom_t(&events) );
  ASSERT_EQ( 0, ctrl_events_bottom_z(&events) );
  ASSERT_EQ( 0, ctrl_events_liftoff_t(&events) );
  ASSERT_EQ( 0, ctrl_events_liftoff_z(&events) );
  ASSERT_EQ( invalid, events.phase );
  ASSERT_EQ( 0, ctrl_events_phi(&events) );
  ASSERT_EQ( 0, ctrl_events_n(&events) );
}

TEST(test_ctrl_events_destroy)
{
  ctrl_events_destroy( &events );
  ASSERT_EQ( 0, ctrl_events_apex_t(&events) );
  ASSERT_EQ( 0, ctrl_events_apex_z(&events) );
  ASSERT_EQ( 0, ctrl_events_touchdown_t(&events) );
  ASSERT_EQ( 0, ctrl_events_touchdown_z(&events) );
  ASSERT_EQ( 0, ctrl_events_bottom_t(&events) );
  ASSERT_EQ( 0, ctrl_events_bottom_z(&events) );
  ASSERT_EQ( 0, ctrl_events_liftoff_t(&events) );
  ASSERT_EQ( 0, ctrl_events_liftoff_z(&events) );
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
    /* zd ,  z0 ,  zb ,   t ,    z ,    v ,apx:{ t,    z } */
    { 0.30, 0.25, 0.20, 0.00, 0.300,  0.00, { 0.00, 0.000 }, },  /* initial */
    { 0.30, 0.25, 0.20, 0.01, 0.300, -0.01, { 0.00, 0.000 }, },  /* falling */
    { 0.30, 0.25, 0.20, 0.10, 0.250, -0.30, { 0.00, 0.000 }, },  /* touchdown */
    { 0.30, 0.25, 0.20, 0.20, 0.200,  0.00, { 0.00, 0.000 }, },  /* bottom */

    { 0.30, 0.25, 0.20, 0.30, 0.250,  sqrt(0.1*G), { 0.30, 0.250 }, },  /* lift-off */
    { 0.30, 0.25, 0.20, 0.31, 0.251,  0.29, { 0.31, 0.251 }, },  /* rising */
    { 0.30, 0.25, 0.20, 0.32, 0.252,  0.28, { 0.32, 0.252 }, },  /* rising */
    { 0.30, 0.25, 0.20, 0.39, 0.259,  0.01, { 0.39, 0.259 }, },  /* rising */
    { 0.30, 0.25, 0.20, 0.40, 0.300,  0.00, { 0.40, 0.300 }, },  /* apex */

    { 0.30, 0.25, 0.20, 0.41, 0.300, -0.01, { 0.40, 0.300 }, },  /* falling */
    { 0.30, 0.25, 0.20, 0.50, 0.250, -0.30, { 0.40, 0.300 }, },  /* touchdown */
    { 0.30, 0.25, 0.20, 0.60, 0.200,  0.00, { 0.40, 0.300 }, },  /* bottom */

    { 0.30, 0.25, 0.20, 0.70, 0.250,  sqrt(0.1*G), { 0.70, 0.250 }, },  /* lift-off */
    { 0.30, 0.25, 0.20, 0.71, 0.251,  0.29, { 0.71, 0.251 }, },  /* rising */
    { 0.30, 0.25, 0.20, 0.72, 0.252,  0.28, { 0.72, 0.252 }, },  /* rising */
    { 0.30, 0.25, 0.20, 0.79, 0.259,  0.01, { 0.79, 0.259 }, },  /* rising */
    { 0.30, 0.25, 0.20, 0.80, 0.300,  0.00, { 0.80, 0.300 }, },  /* apex */

    { 0.30, 0.25, 0.20, 0.81, 0.300, -0.01, { 0.80, 0.300 }, },  /* falling */
    { 0.00, 0.00, 0.00, 0.00, 0.000,  0.00, { 0.00, 0.000 }, },  /* terminator */
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
    /* zd ,  z0 ,  zb ,   t ,    z ,    v ,apx:{ t,    z } */
    { 0.30, 0.25, 0.20, 0.00, 0.300,  0.00, { 0.00, 0.000 }, },  /* initial */
    { 0.30, 0.25, 0.20, 0.01, 0.300, -0.01, { 0.00, 0.000 }, },  /* falling */
    { 0.30, 0.25, 0.20, 0.10, 0.250, -0.90, { 0.00, 0.000 }, },  /* touchdown */
    { 0.30, 0.25, 0.20, 0.20, 0.200,  0.00, { 0.00, 0.000 }, },  /* bottom */
    { 0.30, 0.25, 0.20, 0.29, 0.249,  0.90, { 0.00, 0.000 }, },  /* extension */

    { 0.30, 0.25, 0.20, 0.30, 0.252,  0.80, { 0.30, 0.252 }, },  /* rising */
    { 0.30, 0.25, 0.20, 0.31, 0.253,  0.78, { 0.31, 0.253 }, },  /* rising */
    { 0.30, 0.25, 0.20, 0.39, 0.290,  0.02, { 0.39, 0.290 }, },  /* rising */
    { 0.30, 0.25, 0.20, 0.40, 0.295,  0.01, { 0.40, 0.295 }, },  /* rising */

    { 0.30, 0.25, 0.20, 0.41, 0.280, -0.01, { 0.40, 0.295 }, },  /* falling */
    { 0.30, 0.25, 0.20, 0.42, 0.270, -0.02, { 0.40, 0.295 }, },  /* falling */
    { 0.30, 0.25, 0.20, 0.50, 0.250, -0.30, { 0.40, 0.295 }, },  /* touchdown */
    { 0.30, 0.25, 0.20, 0.60, 0.200,  0.00, { 0.40, 0.295 }, },  /* bottom */
    { 0.30, 0.25, 0.20, 0.69, 0.245,  0.80, { 0.40, 0.295 }, },  /* extension */

    { 0.30, 0.25, 0.20, 0.70, 0.251,  0.85, { 0.70, 0.251 }, },  /* rising */
    { 0.30, 0.25, 0.20, 0.71, 0.252,  0.83, { 0.71, 0.252 }, },  /* rising */
    { 0.30, 0.25, 0.20, 0.79, 0.259,  0.05, { 0.79, 0.259 }, },  /* rising */
    { 0.30, 0.25, 0.20, 0.80, 0.280,  0.03, { 0.80, 0.280 }, },  /* rising */
    { 0.30, 0.25, 0.20, 0.81, 0.295, -0.01, { 0.81, 0.295 }, },  /* falling */

    { 0.30, 0.25, 0.20, 0.82, 0.290, -0.02, { 0.81, 0.295 }, },  /* falling */
    { 0.00, 0.00, 0.00, 0.00, 0.000,  0.00, { 0.00, 0.000 }, },  /* terminator */
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
    { 0.26, 0.26, 0.1,   false },     /* extension */
    { 0.26, 0.25, 0.1,   false },     /* extension */
    { 0.26, 0.24, 0.15,  false },     /* extension */
    { 0.3,  0.3,  0.1,   false },     /* extension */
    { 0.3,  0.28, 0.1,   false },     /* extension */
    { 0.3,  0.26, 0.15,  false },     /* extension */
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
    { 0.26, 0.26, 0.1,   false },     /* extension */
    { 0.26, 0.25, 0.1,   false },     /* extension */
    { 0.26, 0.24, 0.15,  false },     /* extension */
    { 0.3,  0.3,  0.1,   false },     /* extension */
    { 0.3,  0.28, 0.1,   false },     /* extension */
    { 0.3,  0.26, 0.15,  false },     /* extension */
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

TEST(test_ctrl_extension)
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
    { 0.26, 0.26, 0.1,   true  },     /* extension */
    { 0.26, 0.25, 0.1,   true  },     /* extension */
    { 0.26, 0.24, 0.15,  true  },     /* extension */
    { 0.3,  0.3,  0.1,   true  },     /* extension */
    { 0.3,  0.28, 0.1,   true  },     /* extension */
    { 0.3,  0.26, 0.15,  true  },     /* extension */
    { 0.0, 0.0, 0.0, false }
  };
  struct case_t *c;
  bool (*method)(ctrl_t*,vec_t);

  method = ctrl_is_in_extension;
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
  RUN_TEST(test_ctrl_extension);
  RUN_TEST(test_ctrl_v0);
  RUN_TEST(test_ctrl_update_default);
  RUN_TEST(test_ctrl_calc_phase_complex);
  RUN_TEST(test_ctrl_phase_complex);
  RUN_TEST(test_ctrl_calc_phi);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_ctrl_events);
  RUN_SUITE(test_ctrl);
  TEST_REPORT();
  TEST_EXIT();
}
