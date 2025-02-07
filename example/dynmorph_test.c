#include "rhc_ctrl_dynmorph.h"
#include "rhc_phase_portrait_plotter.h"
#include "rhc_string.h"

#define DT 0.001
#define T  10

/*
  Usage:
    Running a specific mode:
      $ cd example
      $ make dynmorph_test
      $ mode=stand; ./dynmorph_test $mode >graph_plot/$mode.csv
      $ cd graph_plot
      $ uv run python plot_phase_portrait.py $mode.csv --xlim --ylim

    Run all modes:
      $ cd example/graph_plot
      $ modes=(stand stand_soft_land hop hop_soft_land squat squat_soft_land);
      $ (cd .. && make dynmorph_test) && \
        for mode in "${modes[@]}"; do \
          echo "Running '${mode}'...";
          ../dynmorph_test $mode >$mode.csv && \
          uv run python plot_phase_portrait.py $mode.csv --xlim --ylim || break; \
        done
 */

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
  STAND_SOFT_LAND,
  HOP_SOFT_LAND,
  SQUAT_SOFT_LAND,
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
  fprintf( stderr, "    stand_soft_land\n" );
  fprintf( stderr, "    hop_soft_land\n" );
  fprintf( stderr, "    squat_soft_land\n" );
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
  else if( strcmp( argv[1], "stand_soft_land" ) == 0 )
    mode_id = STAND_SOFT_LAND;
  else if( strcmp( argv[1], "hop_soft_land" ) == 0 )
    mode_id = HOP_SOFT_LAND;
  else if( strcmp( argv[1], "squat_soft_land" ) == 0)
    mode_id = SQUAT_SOFT_LAND;
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
  logger_open( &logger, NULL );
  ppp_init( &plotter, &cmd, &ctrl, &model, &logger );
  ctrl_dynmorph_create( &ctrl, &cmd, &model );
}

void make_edge_points(ppp_t *ppp)
{
  ppp_set_lim_xy( &plotter, 0.16, 0.36, -1.5, 1.5 );
  ppp_set_n_sc_xy( ppp, 10, 10 );
}

void set_params_stand()
{
  ctrl_dynmorph_set_rho( &ctrl, 0.0 );
  ctrl_dynmorph_disable_soft_landing( &ctrl );
  ppp_generate_edge_points( &plotter );
}

void set_params_stand_soft_land()
{
  vec_t p0;

  p0 = vec_create( 2 );
  ctrl_dynmorph_set_rho( &ctrl, 0.0 );
  ctrl_dynmorph_set_k( &ctrl, 4.0 );
  ctrl_dynmorph_enable_soft_landing( &ctrl );
  ppp_generate_edge_points( &plotter );

  vec_set_elem_list( p0, cmd.zh, -1.5 );
  ppp_remove_p0( &plotter, p0, 1e-6 );
  vec_destroy( p0 );
}

void set_params_hop()
{
  double zm;
  vec_t p0;

  p0 = vec_create( 2 );
  ctrl_dynmorph_set_rho( &ctrl, 1.0 );
  ctrl_dynmorph_set_k( &ctrl, 4.0 );
  ctrl_dynmorph_disable_soft_landing( &ctrl );
  ppp_generate_edge_points( &plotter );

  zm = ctrl_dynmorph_calc_zm( cmd.za, cmd.zh, cmd.zb );
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

void set_params_hop_soft_land()
{
  double zm;
  vec_t p0;

  p0 = vec_create( 2 );
  ctrl_dynmorph_set_rho( &ctrl, 1.0 );
  ctrl_dynmorph_set_k( &ctrl, 4.0 );
  ctrl_dynmorph_enable_soft_landing( &ctrl );
  cmd.za = 0.3;
  ppp_generate_edge_points( &plotter );

  vec_set_elem_list( p0, cmd.zh, -1.5 );
  ppp_remove_p0( &plotter, p0, 1e-6 );

  zm = ctrl_dynmorph_calc_zm( cmd.za, cmd.zh, cmd.zb );
  if( istiny( zm - cmd.zm ) ){
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
  ctrl_dynmorph_set_rho( &ctrl, 1.0 );
  ctrl_dynmorph_set_k( &ctrl, 4.0 );
  ctrl_dynmorph_disable_soft_landing( &ctrl );
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

void set_params_squat_soft_land()
{
  vec_t p0;

  p0 = vec_create( 2 );
  ctrl_dynmorph_set_rho( &ctrl, 1.0 );
  ctrl_dynmorph_set_k( &ctrl, 4.0 );
  ctrl_dynmorph_enable_soft_landing( &ctrl );
  cmd.za = 0.255;
  ppp_generate_edge_points( &plotter );

  vec_set_elem_list( p0, cmd.zh, -1.5 );
  ppp_remove_p0( &plotter, p0, 1e-6 );
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
  case STAND_SOFT_LAND:
    set_params_stand_soft_land();
    break;
  case HOP_SOFT_LAND:
    set_params_hop_soft_land();
    break;
  case SQUAT_SOFT_LAND:
    set_params_squat_soft_land();
    break;
  case NONE:
  default:
    exit( 1 );
    break;
  }
}

void print_cmd()
{
  cmd_f_write_dynmorph( stderr, &cmd );
  fprintf( stderr, "--\n" );
  cmd_f_write_dynmorph( stderr, ctrl_dynmorph_params(&ctrl) );
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
