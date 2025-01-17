#include "rhc_ctrl_rep_hop_stand.h"
#include "rhc_phase_portrait_plotter.h"
#include "rhc_string.h"

#define DT 0.001
#define T  10

cmd_t cmd;
model_t model;
ctrl_t ctrl;
logger_t logger;
ppp_t plotter;

enum mode_list {
  NONE = -1,
  STAND = 0,
  HOP,
  SQUAT,
};
enum mode_list mode_id = NONE;

void usage(int argc, const char *argv[])
{
  fprintf( stderr, "Usage:\n" );
  fprintf( stderr, "  %s <mode>\n", argv[0] );
  fprintf( stderr, "\n" );
  fprintf( stderr, "  Acceptable mode names:\n" );
  fprintf( stderr, "    stand\n" );
  fprintf( stderr, "    hop\n" );
  fprintf( stderr, "    squat\n" );
}

void parse(int argc, const char *argv[])
{
  if( argc < 2 ) {
    mode_id = STAND;
    return;
  }

  if( string_ends_with( argv[1], "help" ) ) {
    usage( argc, argv );
    exit( 0 );
  } else if( strcmp( argv[1], "stand" ) == 0 )
    mode_id = STAND;
  else if( strcmp( argv[1], "hop" ) == 0 )
    mode_id = HOP;
  else if( strcmp( argv[1], "squat" ) == 0)
    mode_id = SQUAT;
  else {
    fprintf( stderr, "unrecognized mode name: %s\n", argv[1] );
    mode_id = NONE;
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
  ctrl_rep_hop_stand_create( &ctrl, &cmd, &model );
}

void make_edge_points(ppp_t *ppp)
{
  ppp_set_lim_xy( &plotter, 0.16, 0.36, -1.5, 1.5 );
  ppp_set_n_sc_xy( ppp, 10, 10 );
}

void set_params_stand()
{
  ctrl_rep_hop_stand_set_rho( &ctrl, 0.0 );
  ppp_generate_edge_points( &plotter );
}

void set_params_hop()
{
  double zh, zm;
  vec_t p0;

  p0 = vec_create( 2 );
  ctrl_rep_hop_stand_set_rho( &ctrl, 1.0 );
  ctrl_rep_hop_stand_set_k( &ctrl, 4.0 );
  ppp_generate_edge_points( &plotter );
  for( zh = cmd.zb; zh < cmd.za; zh += 0.004 ) {
    vec_set_elem_list( p0, zh, 0.0 );
    ppp_push_p0( &plotter, p0 );
  }

  zm = ctrl_rep_hop_stand_calc_zm( cmd.za, cmd.zh, cmd.zb );
  if( !istiny( zm - cmd.zm ) ){
    vec_set_elem_list( p0, zm-1e-6, 0.0 );
    ppp_push_p0( &plotter, p0 );
    vec_set_elem_list( p0, zm+1e-6, 0.0 );
    ppp_push_p0( &plotter, p0 );
  } else {
    vec_set_elem_list( p0, cmd.zm - 1.0e-6, 0.0 );
    ppp_push_p0( &plotter, p0 );
    vec_set_elem_list( p0, cmd.zm + 1.0e-6, 0.0 );
    ppp_push_p0( &plotter, p0 );
  }
  vec_destroy( p0 );
}

void set_params_squat()
{
  vec_t p0;

  p0 = vec_create( 2 );
  ctrl_rep_hop_stand_set_rho( &ctrl, 1.0 );
  ctrl_rep_hop_stand_set_k( &ctrl, 4.0 );
  cmd.za = 0.255;
  ppp_generate_edge_points( &plotter );

  vec_set_elem_list( p0, cmd.za, 0.0 );
  ppp_push_p0( &plotter, p0 );

  vec_set_elem_list( p0, 0.5*(cmd.za + cmd.zb) - 1.0e-6, 0.0 );
  ppp_push_p0( &plotter, p0 );
  vec_set_elem_list( p0, 0.5*(cmd.za + cmd.zb) + 1.0e-6, 0.0 );
  ppp_push_p0( &plotter, p0 );

  vec_set_elem_list( p0, cmd.zb, 0.0 );
  ppp_push_p0( &plotter, p0 );

  vec_destroy( p0 );
}

void setup_ctrl()
{
  make_edge_points( &plotter );
  switch( mode_id ){
  case STAND:
    set_params_stand();
    break;
  case HOP:
    set_params_hop();
    break;
  case SQUAT:
    set_params_squat();
    break;
  case NONE:
  default:
    exit( 1 );
    break;
  }
}

void print_cmd()
{
  cmd_f_write_rep_hop_stand( stderr, &cmd );
  fprintf( stderr, "--\n" );
  cmd_f_write_rep_hop_stand( stderr, ctrl_rep_hop_stand_params(&ctrl) );
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
