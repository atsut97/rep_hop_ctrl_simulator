#include "rhc_ctrl_arl.h"
#include "rhc_ctrl.h"

ctrl_t *ctrl_arl_create(ctrl_t *self, cmd_t *cmd, model_t *model, enum ctrl_arl_types type)
{
  ctrl_init( self, cmd, model );
  self->_update = ctrl_arl_update;
  self->_destroy = ctrl_arl_destroy;
  self->_header = ctrl_arl_header;
  self->_writer = ctrl_arl_writer;

  if ( ( self->prp = nalloc( ctrl_arl_prp, 1 ) ) == NULL ){
    ALLOC_ERR();
    return NULL;
  }
  ((ctrl_arl_prp *)(self->prp))->type = type;
  ((ctrl_arl_prp *)(self->prp))->c = 0;
  return self;
}

void ctrl_arl_destroy(ctrl_t *self)
{
  sfree( self->prp );
  ctrl_destroy_default( self );
}

ctrl_t *ctrl_arl_set_params(ctrl_t *self, double k, double beta)
{
  ctrl_arl_set_k( self, k );
  ctrl_arl_set_beta( self, beta );
  return self;
}

ctrl_t *ctrl_arl_update(ctrl_t *self, double t, vec_t p)
{
  return self;
}

void ctrl_arl_header(FILE *fp, void *util)
{
  fprintf( fp, "\n" );
}
void ctrl_arl_writer(FILE *fp, ctrl_t *self, void *util)
{
  fprintf( fp, "\n" );
}
