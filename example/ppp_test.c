#include "rhc_phase_portrait_plotter.h"
#include "rhc_ctrl_slip.h"
#include "rhc_ctrl_slip_var_stiff.h"
#include "rhc_ctrl_regulator.h"

#define DT 0.001
#define T  10

cmd_t cmd;
model_t model;
ctrl_t ctrl;
logger_t logger;
ppp_t plotter;

enum ctrl_list {
  NONE = -1,
  SLIP = 0, SLIP_VAR_STIFF,
  REGULATOR,
};
enum ctrl_list ctrl_id = NONE;

void parse(int argc, char *argv[])
{
  char *name;

  if( argc < 2 ){
    ctrl_id = SLIP;
    return;
  }

  name = argv[1];
  if( strcmp( name, "slip" ) == 0 )
    ctrl_id = SLIP;
  else if( strcmp( name, "slip_var_stiff" ) == 0 )
    ctrl_id = SLIP_VAR_STIFF;
  else if( strcmp( name, "regulator" ) == 0)
    ctrl_id = REGULATOR;
  else {
    fprintf( stderr, "%s is not available\n", name );
    ctrl_id = NONE;
  }
}

void init()
{
  cmd_default_init( &cmd );
  model_init( &model, 10 );
  logger_init( &logger );
  ppp_init( &plotter, &cmd, &ctrl, &model, &logger );
}

void usage()
{
  fprintf( stderr, "Usage:\n" );
  fprintf( stderr, "ppp_test <controller name>\n");
  fprintf( stderr, "  Controller names:\n" );
  fprintf( stderr, "    slip             SLIP model\n" );
  fprintf( stderr, "    slip_var_stiff   SLIP model with variable stiffness\n" );
  fprintf( stderr, "    regulator        Regulator\n" );
}

void make_initial_points(ppp_t *ppp)
{
  double z0;
  vec_t p0;
  cmd_t *cmd;

  p0 = vec_create( 2 );
  cmd = ppp_cmd(ppp);
  for( z0 = cmd->z0 - 0.005; z0 < cmd->zd + 0.1; z0 += 0.005 ) {
    vec_set_elem_list( p0, z0, 0.0 );
    ppp_push_p0( ppp, p0 );
  }
  vec_destroy( p0 );
}

void make_edge_points(ppp_t *ppp)
{
  /* ppp_set_lim_xy( ppp, 0.23, 0.29, -0.8, 0.8 ); */
  ppp_set_n_sc_xy( ppp, 10, 10 );
  ppp_generate_edge_points( ppp );
}

void init_slip()
{
  /* make_edge_points( &plotter ); */
  make_initial_points( &plotter );
  ctrl_slip_create( &ctrl, &cmd, &model );
}

void init_slip_var_stiff()
{
  make_initial_points( &plotter );
  ctrl_slip_var_stiff_create( &ctrl, &cmd, &model );
}

void init_regulator()
{
  make_edge_points( &plotter );
  ctrl_regulator_create( &ctrl, &cmd, &model );
}

void setup_ctrl()
{
  ppp_set_lim_xy( &plotter, 0.23, 0.29, -0.8, 0.8 );
  switch( ctrl_id ){
    case SLIP:
      init_slip();
      break;
    case SLIP_VAR_STIFF:
      init_slip_var_stiff();
      break;
    case REGULATOR:
      init_regulator();
      break;
    case NONE:
    default:
      usage();
      exit( 1 );
      break;
  }
}

void run()
{
  ppp_run( &plotter, T, DT );
}

void destroy()
{
  ppp_destroy( &plotter );
  logger_destroy( &logger);
  model_destroy( &model );
  ctrl_destroy( &ctrl );
  cmd_destroy( &cmd );
}

int main(int argc, char *argv[])
{
  parse( argc, argv );
  init();
  setup_ctrl();
  run();
  destroy();
  return 0;
}
