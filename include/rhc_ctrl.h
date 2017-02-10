#ifndef __RHC_CTRL_H__
#define __RHC_CTRL_H__

#include "rhc_vec.h"
#include "rhc_cmd.h"

typedef struct{
  cmd_t *cmd;
  double fz;
} ctrl_t;

#define ctrl_cmd(self) (self)->cmd
#define ctrl_fz(self)  (self)->fz
#define ctrl_z0(self)  ctrl_cmd(self)->z0
#define ctrl_zd(self)  ctrl_cmd(self)->zd
#define ctrl_zb(self)  ctrl_cmd(self)->zb

ctrl_t *ctrl_init(ctrl_t *self, cmd_t *cmd);
void ctrl_destroy(ctrl_t *self);

bool ctrl_is_in_flight(ctrl_t *self, vec_t p);
bool ctrl_is_in_compression(ctrl_t *self, vec_t p);
bool ctrl_is_in_decompression(ctrl_t *self, vec_t p);

double ctrl_calc_sqr_v0(double z0, double zd);
#define ctrl_calc_v0(self,z0,zd) sqrt( ctrl_calc_sqr_v0( z0, zd ) )
#define ctrl_sqr_v0(self) ctrl_calc_sqr_v0( ctrl_z0(self), ctrl_zd(self) )
#define ctrl_v0(self) sqrt( ctrl_sqr_v0( self ) )

#endif /* __RHC_CTRL_H__ */
