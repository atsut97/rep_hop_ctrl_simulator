#include "rhc_phase_portrait_plotter.h"

#define PHASE_PORTRAIT_PLOTTER_DEFAULT_NUM_SC 10
ppp_t *ppp_init(ppp_t *self, cmd_t *cmd, ctrl_t *ctrl, model_t *model, logger_t *logger)
{
  register int i;

  self->dim = 2;
  ppp_cmd(self) = cmd;
  ppp_model(self) = model;
  ppp_ctrl(self) = ctrl;
  ppp_logger(self) = logger;
  self->pmin = vec_create( ppp_dim( self ) );
  self->pmax = vec_create( ppp_dim( self ) );
  vec_clear( self->pmin );
  vec_clear( self->pmax );
  if( (self->n_sc = nalloc( int, ppp_dim(self) ) ) == NULL ) {
    ALLOC_ERR();
    exit( 1 );
  }
  for( i=0; i<ppp_dim(self); i++ )
    self->n_sc[i] = PHASE_PORTRAIT_PLOTTER_DEFAULT_NUM_SC;

  simulator_init( ppp_simulator(self), ppp_cmd(self), ppp_ctrl(self), ppp_model(self ) );
  simulator_set_default_logger( ppp_simulator(self), ppp_logger(self) );

  /* prepare initial points list */
  vec_list_init( ppp_p0_list(self) );
  return self;
}

void ppp_destroy(ppp_t *self)
{
  ppp_cmd(self) = NULL;
  ppp_ctrl(self) = NULL;
  ppp_model(self) = NULL;
  vec_destroy( ppp_min( self ) );
  vec_destroy( ppp_max( self ) );
  ppp_min( self ) = NULL;
  ppp_max( self ) = NULL;
  sfree( self->n_sc );

  simulator_destroy( ppp_simulator(self) );
  vec_list_destroy( ppp_p0_list(self) );
}

void ppp_set_lim(ppp_t *self, vec_t pmin, vec_t pmax)
{
  vec_copy( pmin, ppp_min(self) );
  vec_copy( pmax, ppp_max(self) );
}

void ppp_set_lim_xy(ppp_t *self, double xmin, double xmax, double ymin, double ymax)
{
  vec_t min, max;

  min = vec_create_list( 2, xmin, ymin );
  max = vec_create_list( 2, xmax, ymax );
  ppp_set_lim( self, min, max );
  vec_destroy( min );
  vec_destroy( max );
}

void ppp_set_n_sc(ppp_t *self, int *n_sc)
{
  register int i;

  for( i=0; i<ppp_dim(self); i++ ){
    ppp_n_sc( self, i ) = n_sc[i];
  }
}

vec_t ppp_push_p0(ppp_t *self, vec_t p0)
{
  vec_list_node_t *node;

  if( ( node = nalloc( vec_list_node_t, 1 ) ) == NULL ) {
    ALLOC_ERR();
    return NULL;
  }
  vec_list_node_set_data( node, vec_clone( p0 ) );
  vec_list_push( ppp_p0_list(self), node );
  return p0;
}

void ppp_generate_edge_points_dec2bin(ppp_t *self, int dec, int bin[])
{
  register int i;

  i = 0;
  while( dec > 0 ){
    bin[i] = dec % 2;
    dec = dec / 2;
    i++;
  }
}

void ppp_generate_edge_points_set_min_max(ppp_t *self, int ax, int ith, vec_t p0)
{
  int bin[BUFSIZ];
  register int i, j;

  memset( bin, 0, BUFSIZ );
  ppp_generate_edge_points_dec2bin( self, ith, bin );
  j = ppp_dim(self) - 2;
  for( i=0; i<ppp_dim(self); i++ ){
    if( i == ax ) continue;
    vec_elem( p0, i ) = bin[j--] ? vec_elem( ppp_max(self), i ) : vec_elem( ppp_min(self), i );
  }
}

bool ppp_generate_edge_points_is_descending(ppp_t *self, int ax, int ith)
{
  int bin[BUFSIZ];
  register int i;
  int bin_sum = 0;

  memset( bin, 0, BUFSIZ );
  ppp_generate_edge_points_dec2bin( self, ith, bin );
  for( i=0; i<ppp_dim(self); i++ )
    bin_sum += bin[i];
  return (bool)( ( ax + bin_sum ) % 2 );
}

void ppp_generate_edge_points_set_step(ppp_t *self, int ax, int i, bool descending, vec_t p0)
{
  double min, max, dif;
  int n_sc;

  min = vec_elem( ppp_min(self), ax );
  max = vec_elem( ppp_max(self), ax );
  dif = max - min;
  n_sc = ppp_n_sc( self, ax );
  if( descending )
    vec_elem( p0, ax ) = max - dif * i / n_sc;
  else
    vec_elem( p0, ax ) = min + dif * i / n_sc;
}

void ppp_generate_edge_points_on_each_axis(ppp_t *self, int ax)
{
  register int i, j;
  vec_t p0;
  bool is_descending;

  p0 = vec_create( ppp_dim(self) );
  for( i=0; i<pow(2, ppp_dim(self)-1); i++ ){
    ppp_generate_edge_points_set_min_max( self, ax, i, p0 );
    is_descending = ppp_generate_edge_points_is_descending( self, ax, i );
    for( j=0; j<ppp_n_sc( self, ax ); j++ ){
      ppp_generate_edge_points_set_step( self, ax, j, is_descending, p0 );
      ppp_push_p0( self, p0 );
    }
  }
  vec_destroy( p0 );
}

void ppp_generate_edge_points(ppp_t *self)
{
  register int i;

  for( i=0; i<ppp_dim(self); i++ ){
    ppp_generate_edge_points_on_each_axis( self, i );
  }
}

void ppp_run(ppp_t *self, double max_time, double dt)
{
  vec_list_node_t *node;

  vec_list_for_each( ppp_p0_list(self), node ){
    simulator_run( ppp_simulator(self), node->v, max_time, dt, ppp_logger(self), NULL );
  }
}
