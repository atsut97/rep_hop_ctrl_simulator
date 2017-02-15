#ifndef __RHC_CTRL_H__
#define __RHC_CTRL_H__

#include "rhc_vec.h"
#include "rhc_cmd.h"

typedef struct _ctrl_t{
  cmd_t *cmd;
  double fz;
  void *prp;
  struct _ctrl_t* (*_update)(struct _ctrl_t*,double,vec_t);
  void (*_destroy)(struct _ctrl_t*);
} ctrl_t;

#define ctrl_cmd(self) ((ctrl_t*)self)->cmd
#define ctrl_fz(self)  ((ctrl_t*)self)->fz
#define ctrl_z0(self)  ctrl_cmd(self)->z0
#define ctrl_zd(self)  ctrl_cmd(self)->zd
#define ctrl_zb(self)  ctrl_cmd(self)->zb

#define ctrl_update(self,t,p) ((ctrl_t*)self)->_update( self, t, p )
#define ctrl_destroy(self)    ((ctrl_t*)self)->_destroy( self )

ctrl_t *ctrl_init(ctrl_t *self, cmd_t *cmd);

bool ctrl_is_in_flight(ctrl_t *self, vec_t p);
bool ctrl_is_in_compression(ctrl_t *self, vec_t p);
bool ctrl_is_in_decompression(ctrl_t *self, vec_t p);

double ctrl_calc_sqr_v0(double z0, double zd);
#define ctrl_calc_v0(self,z0,zd) sqrt( ctrl_calc_sqr_v0( z0, zd ) )
#define ctrl_sqr_v0(self) ctrl_calc_sqr_v0( ctrl_z0(self), ctrl_zd(self) )
#define ctrl_v0(self) sqrt( ctrl_sqr_v0( self ) )

ctrl_t *ctrl_update_default(ctrl_t *self, double t, vec_t p);
void ctrl_destroy_default(ctrl_t *self);

#endif /* __RHC_CTRL_H__ */
