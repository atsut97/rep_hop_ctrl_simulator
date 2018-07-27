#include "rhc_simulator.h"
#include "rhc_ctrl_slip_var_stiff.h"

#define DT 0.001
#define T  10

int main(int argc, char *argv[])
{
  cmd_t cmd;
  model_t model;
  ctrl_t ctrl;
  logger_t logger;
  simulator_t sim;
  vec_t p0;
  double z0;

  cmd_default_init( &cmd );
  model_init( &model, 10 );
  ctrl_slip_var_stiff_create( &ctrl, &cmd, &model );
  logger_init( &logger );
  simulator_init( &sim, &cmd, &ctrl, &model );
  simulator_set_default_logger( &sim, &logger );
  p0 = vec_create( 2 );

  for( z0 = cmd.z0 - 0.005; z0 < cmd.zd + 0.1; z0 += 0.005 ) {
    vec_set_elem_list( p0, z0, 0.0 );
    simulator_run( &sim, p0, T, DT, &logger, NULL );
  }

  vec_destroy( p0 );
  simulator_destroy( &sim );
  logger_destroy( &logger );
  ctrl_destroy( &ctrl );
  model_destroy( &model );
  cmd_destroy( &cmd );
  return 0;
}
