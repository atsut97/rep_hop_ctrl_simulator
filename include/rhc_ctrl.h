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

#endif /* __RHC_CTRL_H__ */
