#include "rhc_model.h"

model_t *model_init(model_t *self, double m)
{
  model_set_mass( self, m );
  model_set_acc( self, 0 );
  model_set_gravity( self, G );
  return self;
}

void model_destroy(model_t *self)
{
  model_set_mass( self, 0 );
  model_set_gravity( self, G );
}

double model_calc_acc(double m, double fz, double fe)
{
  if( fz < 0 ) fz = 0;
  return fz / m - G + fe / m;
}

model_t *model_update(model_t *self, double fz, double fe)
{
  double acc;

  acc = model_calc_acc( model_mass(self), fz, fe );
  model_set_acc( self, acc );
  return self;
}
