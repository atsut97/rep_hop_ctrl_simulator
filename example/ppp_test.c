#include "rhc_ctrl_regulator.h"
#include "rhc_ctrl_slip.h"
#include "rhc_ctrl_slip_var_stiff.h"
#include "rhc_phase_portrait_plotter.h"
#include "rhc_string.h"

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

void usage(int argc, const char *argv[])
{
  fprintf( stderr, "Usage:\n" );
  fprintf( stderr, "  %s <controller name>\n", argv[0] );
  fprintf( stderr, "\n" );
  fprintf( stderr, "  Acceptable controller names:\n" );
  fprintf( stderr, "    slip             SLIP model\n" );
  fprintf( stderr, "    slip_var_stiff   SLIP model with variable stiffness\n" );
  fprintf( stderr, "    regulator        Regulator\n" );
}

void parse(int argc, const char *argv[])
{
  if( argc < 2 ) {
    ctrl_id = SLIP;
    return;
  }

  if( string_ends_with( argv[1], "help" ) ) {
    usage( argc, argv );
    exit( 0 );
  } else if( strcmp( argv[1], "slip" ) == 0 )
    ctrl_id = SLIP;
  else if( strcmp( argv[1], "slip_var_stiff" ) == 0 )
    ctrl_id = SLIP_VAR_STIFF;
  else if( strcmp( argv[1], "regulator" ) == 0)
    ctrl_id = REGULATOR;
  else {
    fprintf( stderr, "%s is not available\n", argv[1] );
    ctrl_id = NONE;
    usage( argc, argv );
    exit( 1 );
  }
}

void init()
{
  cmd_default_init( &cmd );
  model_init( &model, 10 );
  logger_init( &logger );
  ppp_init( &plotter, &cmd, &ctrl, &model, &logger );
}

void make_initial_points(ppp_t *ppp)
{
  double zh;
  vec_t p0;
  cmd_t *cmd;

  p0 = vec_create( 2 );
  cmd = ppp_cmd(ppp);
  for( zh = cmd->zh - 0.005; zh < cmd->za + 0.1; zh += 0.005 ) {
    vec_set_elem_list( p0, zh, 0.0 );
    ppp_push_p0( ppp, p0 );
  }
  vec_destroy( p0 );
}

void make_edge_points(ppp_t *ppp)
{
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
  ppp_set_lim_xy( &plotter, 0.16, 0.36, -1.5, 1.5 );
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
  logger_destroy( &logger );
  model_destroy( &model );
  ctrl_destroy( &ctrl );
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
