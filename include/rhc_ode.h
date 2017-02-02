#ifndef __RHC_ODE_H__
#define __RHC_ODE_H__

#include "rhc_vec.h"

typedef struct _ode_t{
  vec_t  (* f)(double,vec_t,void*,vec_t);
  struct _ode_t* (*init)(struct _ode_t*,int,vec_t (*)(double,vec_t,void*,vec_t));
  vec_t  (*update)(struct _ode_t*,double,vec_t,double,void*);
  void   (*destroy)(struct _ode_t*);
  void *_ws;
} ode_t;

#define ode_assign(self,type) do{\
  (self)->init = ode_init_##type;\
  (self)->update = ode_update_##type;\
  (self)->destroy = ode_destroy_##type;\
} while( 0 )

#define ode_init(self,dim,f) (self)->init( self, dim, f )
#define ode_update(self,t,x,dt,u) (self)->update( self, t, x, dt, u )
#define ode_destroy(self) (self)->destroy( self )

#include "rhc_ode_euler.h"

#endif /* __RHC_ODE_H__ */
