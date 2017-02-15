#include "rhc_model.h"
#include "rhc_test.h"

model_t model;

void setup()
{
  model_init( &model, 1.0 );
}

void teardown()
{
  model_destroy( &model );
}

TEST(test_model_init)
{
  ASSERT_EQ( 1.0, model_mass( &model ) );
  ASSERT_EQ( 0.0, model_acc( &model ) );
}

TEST(test_model_destroy)
{
  model_destroy( &model );
  ASSERT_EQ( 0.0, model_mass( &model ) );
}

TEST(test_model_set_mass)
{
  model_set_mass( &model, 2.0 );
  ASSERT_EQ( 2.0, model_mass(&model) );
  model_set_mass( &model, 4.0 );
  ASSERT_EQ( 4.0, model_mass(&model) );
}

TEST(test_model_calc_acc)
{
  struct case_t {
    double m, fz, fe;
    double expected;
  } cases[] = {
    { 1.0, 1.0, 0.0, 1-G },  /* stance */
    { 1.0, 2.0, 0.0, 2-G },  /* stance */
    { 2.0, 6.0, 0.0, 3-G },  /* stance */
    { 1.0, -1.0, 0.0, -G },  /* negative force */
    { 2.0, -2.0, 0.0, -G },  /* negative force */
    { 3.0, -1.0, 0.0, -G },  /* negative force */
    { 1.0, 1.0, 1.0, 2-G },  /* add ext force */
    { 2.0, 2.0, 4.0, 3-G },  /* add ext force */
    { 1.0, 0.0, 2.0, 2-G },  /* add ext force */
    { 0, 0, 0, 0 }
  };
  struct case_t *c;

  for( c=cases; c->m>0; c++ ){
    double acc;
    acc = model_calc_acc( c->m, c->fz, c->fe );
    ASSERT_DOUBLE_EQ( c->expected, acc );
  }
}

TEST(test_model_update)
{
  struct case_t {
    double m, fz, fe;
    double expected;
  } cases[] = {
    { 1.0, 1.0, 0.0, 1-G },  /* stance */
    { 1.0, 2.0, 0.0, 2-G },  /* stance */
    { 2.0, 6.0, 0.0, 3-G },  /* stance */
    { 1.0, -1.0, 0.0, -G },  /* negative force */
    { 2.0, -2.0, 0.0, -G },  /* negative force */
    { 3.0, -1.0, 0.0, -G },  /* negative force */
    { 1.0, 1.0, 1.0, 2-G },  /* add ext force */
    { 2.0, 2.0, 4.0, 3-G },  /* add ext force */
    { 1.0, 0.0, 2.0, 2-G },  /* add ext force */
    { 0, 0, 0, 0 }
  };
  struct case_t *c;

  for( c=cases; c->m>0; c++ ){
    model_set_mass( &model, c->m );
    model_update( &model, c->fz, c->fe );
    ASSERT_DOUBLE_EQ( c->expected, model_acc( &model ) );
  }
}

TEST_SUITE(test_model)
{
  CONFIGURE_SUITE( setup, teardown );
  RUN_TEST(test_model_init);
  RUN_TEST(test_model_destroy);
  RUN_TEST(test_model_set_mass);
  RUN_TEST(test_model_calc_acc);
  RUN_TEST(test_model_update);
}

int main(int argc, char *argv[])
{
  RUN_SUITE(test_model);
  TEST_REPORT();
  TEST_EXIT();
}
