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
  return self;
}

void ppp_destroy(ppp_t *self)
{
  ppp_cmd(self) = NULL;
  ppp_ctrl(self) = NULL;
  ppp_model(self) = NULL;
  sfree( self->n_sc );
}
