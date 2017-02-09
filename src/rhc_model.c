#include "rhc_model.h"

model_t *model_init(model_t *self, double m, cmd_t *cmd)
{
  model_set_mass( self, m );
  model_set_cmd( self, cmd );
  return self;
}

void model_destroy(model_t *self)
{
  model_set_mass( self, 0 );
  model_set_cmd( self, NULL );
}

double model_calc_acc(model_t *self, double z, double z0, double fz, double fe)
{
  if( fz < 0 || z > z0 ) fz = 0;
  return fz / self->m - G + fe / self->m;
}
