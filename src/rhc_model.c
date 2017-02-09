#include "rhc_model.h"

model_t *model_init(model_t *self, double m)
{
  model_set_mass( self, m );
  return self;
}

void model_destroy(model_t *self)
{
  model_set_mass( self, 0 );
}

double model_calc_acc(double m, double fz, double fe)
{
  if( fz < 0 ) fz = 0;
  return fz / m - G + fe / m;
}
