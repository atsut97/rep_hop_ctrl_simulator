#include "rhc_phase_portrait_plotter.h"

#define PHASE_PORTRAIT_PLOTTER_DEFAULT_NUM_SC 10
#define PHASE_PORTRAIT_PLOTTER_BUFFER_SIZE 300
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
  vec_fill( self->pmin, -HUGE_VAL );
  vec_fill( self->pmax, HUGE_VAL );
  if( (self->n_sc = nalloc( int, ppp_dim(self) ) ) == NULL ) {
    ALLOC_ERR();
    exit( 1 );
  }
  for( i=0; i<ppp_dim(self); i++ )
    self->n_sc[i] = PHASE_PORTRAIT_PLOTTER_DEFAULT_NUM_SC;

  simulator_init( ppp_simulator(self), ppp_cmd(self), ppp_ctrl(self), ppp_model(self ) );
  simulator_set_default_logger( ppp_simulator(self), ppp_logger(self) );
  simulator_set_update_fp( ppp_simulator(self), ppp_simulator_update );

  /* prepare initial points list */
  vec_list_init( ppp_p0_list(self) );

  /* prepare circular buffer of points */
  vec_ring_init( ppp_point_buf(self), ppp_dim(self),
                 PHASE_PORTRAIT_PLOTTER_BUFFER_SIZE );

  self->_a = vec_create( ppp_dim(self) );
  self->_b = vec_create( ppp_dim(self) );
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

  vec_list_destroy( ppp_p0_list(self) );
  vec_ring_destroy( ppp_point_buf(self) );
  simulator_destroy( ppp_simulator(self) );

  vec_destroy( self->_a );
  vec_destroy( self->_b );
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

void ppp_set_n_sc_xy(ppp_t *self, int n_sc_x, int n_sc_y)
{
  int n_sc[] = { n_sc_x, n_sc_y };

  ppp_set_n_sc( self, n_sc );
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

bool ppp_simulator_is_converged(ppp_t *self, vec_t p, double tol)
{
  if( vec_ring_empty( ppp_point_buf(self) ) )
    return false;
  return vec_near( p, vec_ring_head( ppp_point_buf(self) ), tol );
}

double __internally_divisional_proportion(ppp_t *self, vec_t p, vec_t v1, vec_t v2)
{
  vec_sub( p, v1, self->_a );
  vec_sub( v2, v1, self->_b );
  return vec_dot( self->_a, self->_b ) / vec_sqr_norm( self->_b );
}

bool ppp_simulator_is_on_limit_cycle(ppp_t *self, vec_t p, double tol)
{
  double k;
  vec_t v1, v2;
  register int i;

  for( i=vec_ring_size( ppp_point_buf(self) )-1; i>0; i-- ){
    v1 = vec_ring_item( ppp_point_buf(self), i );
    v2 = vec_ring_item( ppp_point_buf(self), i-1 );
    k = __internally_divisional_proportion( self, p, v1, v2 );
    if( k < 0.0 || k > 1.0 ) continue;
    vec_cat( v1, k, self->_b, self->_a );
    vec_sub( p, self->_a, self->_b );
    if( istol( vec_norm( self->_b ), tol ) )
      return true;
  }
  return false;
}

#define PHASE_PORTRAIT_PLOTTER_STABLE_TOL ( 1.0e-4 )
bool ppp_simulator_is_stable(ppp_t *self, vec_t p)
{
  if( ppp_simulator_is_converged( self, p, PHASE_PORTRAIT_PLOTTER_STABLE_TOL ) )
    return true;
  if( ppp_simulator_is_on_limit_cycle( self, p, PHASE_PORTRAIT_PLOTTER_STABLE_TOL ) )
    return true;
  return false;
}

bool __is_lower_any(vec_t v1, vec_t v2)
{
  register int i;

  for( i=0; i<vec_size(v1); i++ )
    if( vec_elem(v1,i) < vec_elem(v2,i) ) return true;
  return false;
}

bool __is_greater_any(vec_t v1, vec_t v2)
{
  register int i;

  for( i=0; i<vec_size(v1); i++ )
    if( vec_elem(v1,i) > vec_elem(v2,i) ) return true;
  return false;
}

bool ppp_simulator_is_out_of_region(ppp_t *self, vec_t p)
{
  return ( __is_lower_any( p, self->pmin ) ||
           __is_greater_any( p, self->pmax ) );
}

bool ppp_simulator_update(simulator_t *self, double fe, double dt, void *util)
{
  ppp_t *ppp = util;

  if( ppp_simulator_is_stable( ppp, simulator_state(self) ) )
    return false;
  if( ppp_simulator_is_out_of_region( ppp, simulator_state(self) ) )
    return false;
  vec_ring_push( ppp_point_buf(ppp), simulator_state(self) );
  ode_update( &self->ode, simulator_time(self), simulator_state(self), dt, self );
  return true;
}

void ppp_run(ppp_t *self, double max_time, double dt)
{
  vec_list_node_t *node;

  vec_list_for_each( ppp_p0_list(self), node ){
    simulator_run( ppp_simulator(self), node->v, max_time, dt, ppp_logger(self), self );
  }
}
