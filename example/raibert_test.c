#include "rhc_cmd.h"
#include "rhc_ctrl.h"
#include "rhc_ctrl_raibert.h"
#include "rhc_logger.h"
#include "rhc_model.h"
#include "rhc_simulator.h"
#include "rhc_string.h"
#include "rhc_vec.h"

#define DT 0.001
#define T  10

cmd_t cmd;
model_t model;
ctrl_t ctrl;
logger_t logger;
simulator_t sim;
enum ctrl_raibert_types type_id = none;
vec_t z0_list;

void usage(int argc, const char *argv[])
{
  fprintf( stderr, "Usage:\n" );
  fprintf( stderr, "  %s <control type>\n", argv[0] );
  fprintf( stderr, "\n" );
  fprintf( stderr, "  Control types available:\n" );
  fprintf( stderr, "    fn | full_nonlinear\n" );
  fprintf( stderr, "    sn | simplified_nonlinear\n" );
  fprintf( stderr, "    fl | full_linear\n" );
  fprintf( stderr, "    sl | simplified_linear\n" );
}

void parse(int argc, const char *argv[])
{
  if( argc < 2 ){
    type_id = full_nonlinear;
    return;
  }

  if( string_ends_with( argv[1], "help" ) ){
    usage( argc, argv );
    exit( 0 );
  } else if( strcmp( argv[1], "fn" ) == 0 || \
             strcmp( argv[1], "full_nonlinear" ) ==0 ){
    type_id = full_nonlinear;
  } else if( strcmp( argv[1], "sn" ) == 0 || \
             strcmp( argv[1], "simplified_nonlinear" ) ==0 ){
    type_id = simplified_nonlinear;
  } else if( strcmp( argv[1], "fl" ) == 0 || \
             strcmp( argv[1], "full_linear" ) ==0 ){
    type_id = full_linear;
  } else if( strcmp( argv[1], "sl" ) == 0 || \
             strcmp( argv[1], "simplified_linear" ) ==0 ){
    type_id = simplified_linear;
  } else {
    fprintf( stderr, "%s is not available\n", argv[1] );
    usage( argc, argv );
    exit( 1 );
  }
}

void init()
{
  cmd_init( &cmd );
  model_init( &model, 1 );
  logger_init( &logger );
  simulator_init( &sim, &cmd, &ctrl, &model );
  simulator_set_default_logger( &sim, &logger );
}

void setup_ctrl()
{
  cmd.z0 = 0.5;
  cmd.zd = 1.0;
  cmd.zb = 0.2;
  ctrl_raibert_create( &ctrl, &cmd, &model, type_id );
  if( type_id == full_nonlinear ){
    ctrl_raibert_set_params_full_nonlinear( &ctrl, 0.01, 41.86, 2.33, 5.81, 1 );
    z0_list = vec_create_list( 2, 0.8, 0.9 );
  } else if( type_id == simplified_nonlinear ){
    /* Stable orbit */
    ctrl_raibert_set_params_simplified_nonlinear( &ctrl, 41.86, 5.81 );
    z0_list = vec_create_list( 2, 1.15, 1.4 );
    /* A limping gait */
    /* ctrl_raibert_set_params_simplified_nonlinear( &ctrl, 41.86, 2.33 ); */
    /* z0_list = vec_create_list( 1, 0.02 ); */
  } else if( type_id == full_linear ){
    ctrl_raibert_set_params_full_linear( &ctrl, 0.05, 41.86, 2.33, 46.5, 1, 1 );
    z0_list = vec_create_list( 2, 0.91, 0.98 );
  } else if( type_id == simplified_linear ){
    cmd.z0 = 0;
    cmd.zd = 0.5;
    cmd.zb = -0.3;
    /* Tuned */
    /* ctrl_raibert_set_params_simplified_linear( &ctrl, 0.05, 41.86, 0.58, 46.5, 0.1 ); */
    /* z0_list = vec_create_list( 2, 0.44, 0.5 ); */
    /* Untuned */
    ctrl_raibert_set_params_simplified_linear( &ctrl, 0.05, 41.86, 2.33, 46.5, 0.215 );
    z0_list = vec_create_list( 2, -0.16, -0.18 );
  }
}

void run()
{
  vec_t p0;
  register int i;

  p0 = vec_create( 2 );
  for( i=0; i<vec_size( z0_list ); i++ ){
    vec_set_elem_list( p0, vec_elem(z0_list, i), 0.0 );
    simulator_run( &sim, p0, T, DT, &logger, NULL );
  }
  vec_destroy( p0 );
}

void destroy()
{
  vec_destroy( z0_list );
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
  setup_ctrl();
  run();
  destroy();
  return 0;
}
