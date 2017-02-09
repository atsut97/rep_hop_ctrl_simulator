#include "rhc_model.h"
#include "rhc_test.h"

model_t model;
cmd_t cmd;

void setup()
{
  cmd_default_init( &cmd );
  model_init( &model, 1.0, &cmd );
}

void teardown()
{
  model_destroy( &model );
}

TEST(test_model_init)
{
  ASSERT_EQ( 1.0, model_mass( &model ) );
  ASSERT_PTREQ( &cmd, model_cmd( &model ) );
}

TEST(test_model_destroy)
{
  model_destroy( &model );
  ASSERT_EQ( 0.0, model_mass( &model ) );
  ASSERT_PTREQ( NULL, model_cmd( &model ) );
}

TEST(test_model_set_mass)
{
  model_set_mass( &model, 2.0 );
  ASSERT_EQ( 2.0, model_mass(&model) );
  model_set_mass( &model, 4.0 );
  ASSERT_EQ( 4.0, model_mass(&model) );
}

TEST(test_model_set_cmd)
{
  cmd_t cmd1, cmd2;

  model_set_cmd( &model, &cmd1 );
  ASSERT_PTREQ( &cmd1, model_cmd(&model) );
  model_set_cmd( &model, &cmd2 );
  ASSERT_PTREQ( &cmd2, model_cmd(&model) );
}

TEST(test_model_calc_acc)
{
  struct case_t {
    double m, z, z0, fz, fe;
    double expected;
  } cases[] = {
    { 1.0, 0.25, 0.26, 1.0, 0.0, 1-G },  /* stance */
    { 1.0, 0.25, 0.26, 2.0, 0.0, 2-G },  /* stance */
    { 2.0, 0.25, 0.26, 6.0, 0.0, 3-G },  /* stance */
    { 1.0, 0.27, 0.26, 1.0, 0.0, -G },   /* flight */
    { 1.0, 0.28, 0.26, 3.0, 0.0, -G },   /* flight */
    { 2.0, 0.28, 0.26, 3.0, 0.0, -G },   /* flight */
    { 1.0, 0.25, 0.26, -1.0, 0.0, -G },  /* negative force */
    { 1.0, 0.25, 0.26, -2.0, 0.0, -G },  /* negative force */
    { 1.0, 0.27, 0.26, -1.0, 0.0, -G },  /* negative force */
    { 1.0, 0.25, 0.26, 1.0, 1.0, 2-G },  /* add ext force */
    { 2.0, 0.25, 0.26, 2.0, 4.0, 3-G },  /* add ext force */
    { 1.0, 0.27, 0.26, 0.0, 2.0, 2-G },  /* add ext force */
    { 0, 0, 0, 0, 0, 0 }
  };
  struct case_t *c;

  for( c=cases; c->m>0; c++ ){
    double acc;
    model_set_mass( &model, c->m );
    acc = model_calc_acc( &model, c->z, c->z0, c->fz, c->fe );
    ASSERT_DOUBLE_EQ( c->expected, acc );
  }
}

TEST_SUITE(test_model)
{
  CONFIGURE_SUITE( setup, teardown );
  RUN_TEST(test_model_init);
  RUN_TEST(test_model_destroy);
  RUN_TEST(test_model_set_mass);
  RUN_TEST(test_model_set_cmd);
  RUN_TEST(test_model_calc_acc);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_model);
  TEST_REPORT();
  TEST_EXIT();
}
