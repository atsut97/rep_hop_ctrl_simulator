#ifndef __RHC_ODE_RK4_H__
#define __RHC_ODE_RK4_H__

ode_t *ode_init_rk4(ode_t *self, int dim, vec_t (* f)(double,vec_t,void*,vec_t));
vec_t ode_update_rk4(ode_t *self, double t, vec_t x, double dt, void *util);
void ode_destroy_rk4(ode_t *self);

#endif /* __RHC_ODE_RK4_H__ */
