#include "rhc_phase_portrait_plotter.h"

#define PHASE_PORTRAIT_PLOTTER_DEFAULT_NUM_SC 10
ppp_t *ppp_init(ppp_t *self, cmd_t *cmd, ctrl_t *ctrl, model_t *model)
{
  register int i;

  self->dim = 2;
  ppp_cmd(self) = cmd;
  ppp_model(self) = model;
  ppp_ctrl(self) = ctrl;
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

  vec_list_destroy( ppp_p0_list(self) );
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
