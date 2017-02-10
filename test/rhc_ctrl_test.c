#include "rhc_ctrl.h"
#include "rhc_test.h"

static cmd_t cmd;
static ctrl_t ctrl;
static vec_t p;

void setup()
{
  cmd_default_init( &cmd );
  ctrl_init( &ctrl, &cmd );
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
  ASSERT_EQ( 0, ctrl_fz( &ctrl ) );
}

TEST(test_ctrl_destroy)
{
  ctrl_destroy( &ctrl );
  ASSERT_PTREQ( NULL, ctrl_cmd( &ctrl ) );
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

TEST_SUITE(test_ctrl)
{
  CONFIGURE_SUITE( setup, teardown );
  RUN_TEST(test_ctrl_init);
  RUN_TEST(test_ctrl_destroy);
  RUN_TEST(test_ctrl_flight);
  RUN_TEST(test_ctrl_compression);
  RUN_TEST(test_ctrl_decompression);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_ctrl);
  TEST_REPORT();
  TEST_EXIT();
}
