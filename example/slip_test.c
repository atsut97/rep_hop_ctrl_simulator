#include "rhc_simulator.h"
#include "rhc_ctrl_slip.h"

#define DT 0.001
#define T  10

void header(FILE *fp, void *util) {
  fprintf(fp, "t,z,v\n" );
}

void output(FILE *fp, double t, vec_t state, double fe, cmd_t *cmd, model_t *model, void *util) {
  fprintf( fp, "%f,%f,%f\n", t, vec_elem(state, 0), vec_elem(state, 1) );
}

int main(int argc, char *argv[])
{
  cmd_t cmd;
  model_t model;
  ctrl_t ctrl;
  logger_t logger;
  simulator_t sim;
  vec_t p;

  cmd_default_init( &cmd );
  model_init( &model, 10 );
  ctrl_slip_create( &ctrl, &cmd, &model );
  logger_init( &logger );
  logger_register( &logger, header, output );
  simulator_init( &sim, &cmd, &ctrl, &model );
  p = vec_create_list( 2, 0.28, 0.0 );

  simulator_run( &sim, p, T, DT, &logger );

  vec_destroy( p );
  simulator_destroy( &sim );
  logger_destroy( &logger );
  ctrl_destroy( &ctrl );
  model_destroy( &model );
  cmd_destroy( &cmd );
  return 0;
}
