#include "rhc_model.h"

model_t *model_init(model_t *self, double m)
{
  self->m = m;
  return self;
}

void model_destroy(model_t *self)
{
}
