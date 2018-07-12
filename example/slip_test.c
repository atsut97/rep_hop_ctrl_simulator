#include "rhc_simulator.h"
#include "rhc_ctrl_slip.h"

#define DT 0.001
#define T  10
int main(int argc, char *argv[])
{
  cmd_t cmd;
  model_t model;
  ctrl_t ctrl;
  simulator_t sim;
  vec_t p;
  /* double t; */

  cmd_default_init( &cmd );
  model_init( &model, 10 );
  ctrl_slip_create( &ctrl, &cmd, &model );
  simulator_init( &sim, &cmd, &ctrl, &model );
  p = vec_create_list( 2, 0.28, 0.0 );

  simulator_run( &sim, p, T, DT );
  /* simulator_set_state( &sim, p ); */
  /* for( t=0; t<=T; t+=DT ){ */
  /*   vec_write( simulator_state(&sim) ); */
  /*   simulator_update( &sim, 0, DT ); */
  /* } */

  vec_destroy( p );
  simulator_destroy( &sim );
  ctrl_destroy( &ctrl );
  model_destroy( &model );
  cmd_destroy( &cmd );
  return 0;
}
