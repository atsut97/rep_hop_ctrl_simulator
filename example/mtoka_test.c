#include "rhc_cmd.h"
#include "rhc_ctrl.h"
#include "rhc_ctrl_mtoka.h"
#include "rhc_logger.h"
#include "rhc_model.h"
#include "rhc_simulator.h"
#include "rhc_string.h"
#include "rhc_vec.h"

#define DT 0.001
#define TIME  5

cmd_t cmd;
model_t model;
ctrl_t ctrl;
logger_t logger;
simulator_t sim;

void usage(int argc, const char *argv[])
{
  fprintf( stderr, "Usage:\n" );
  fprintf( stderr, "  %s <z0> <za> <zb> <tau> <T> <a> <b> <th> <mu> <rho> <lam>\n", argv[0] );
}

void parse(int argc, const char *argv[])
{
  /* print usage and exit */
  if( argc < 2 ||
      string_ends_with( argv[1], "h" ) ||
      string_ends_with( argv[1], "help" ) ){
    usage( argc, argv );
    exit( 0 );
  } else if( argc != 12 ){
    fprintf( stderr, "Wrong number of arguments. Requires %d args, but %d given.\n", 12, argc);
    exit( 1 );
  }
}

void init()
{
  cmd_init( &cmd );
  model_init( &model, 10 );
  logger_init( &logger );
  simulator_init( &sim, &cmd, &ctrl, &model );
  simulator_set_default_logger( &sim, &logger );
}

void setup_ctrl(const char *argv[])
{
  cmd.z0 = atof( argv[1] );
  cmd.za = atof( argv[2] );
  cmd.zb = atof( argv[3] );
  ctrl_mtoka_create( &ctrl, &cmd, &model );
  ctrl_mtoka_rise_time_const(&ctrl)        = atof( argv[4] );
  ctrl_mtoka_adapt_time_const(&ctrl)       = atof( argv[5] );
  ctrl_mtoka_mutual_inhibit_weights(&ctrl) = atof( argv[6] );
  ctrl_mtoka_steady_firing_rate(&ctrl)     = atof( argv[7] );
  ctrl_mtoka_firing_threshold(&ctrl)       = atof( argv[8] );
  ctrl_mtoka_feedback_gain(&ctrl)          = atof( argv[9] );
  ctrl_mtoka_sensory_gain(&ctrl)           = atof( argv[10] );
  ctrl_mtoka_saturation_gain(&ctrl)        = atof( argv[11] );
}

void run()
{
  vec_t p0;

  p0 = vec_create_list( 2, cmd.za, 0.0 );
  simulator_run( &sim, p0, TIME, DT, &logger, NULL );
  vec_destroy( p0 );
}

void destroy()
{
  simulator_destroy( &sim );
  logger_destroy( &logger );
  ctrl_destroy( &ctrl );
  model_destroy( &model );
  cmd_destroy( &cmd );
}

int main(int argc, const char *argv[])
{
  parse( argc, argv );
  init();
  setup_ctrl( argv );
  run();
  destroy();
  return 0;
}
