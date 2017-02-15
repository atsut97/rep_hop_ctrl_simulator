#include "rhc_ctrl_dummy.h"

ctrl_t *ctrl_dummy_create(ctrl_t *self, cmd_t *cmd, double k)
{
  ctrl_dummy_prp *prp;

  ctrl_init( self, cmd  );
  self->_update = ctrl_dummy_update;
  self->_destroy = ctrl_dummy_destroy;

  if( ( self->prp = nalloc( ctrl_dummy_prp, 1 ) ) == NULL ){
    ALLOC_ERR();
    return NULL;
  }
  prp = self->prp;
  prp->k = k;
  return self;
}

void ctrl_dummy_destroy(ctrl_t *self)
{
  sfree( self->prp );
  ctrl_destroy_default( self );
}

ctrl_t *ctrl_dummy_update(ctrl_t *self, double t, vec_t p)
{
  ctrl_dummy_prp *prp;

  prp = self->prp;
  self->fz = prp->k;
  return self;
}
