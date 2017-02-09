#ifndef __RHC_CTRL_H__
#define __RHC_CTRL_H__

#include "rhc_vec.h"
#include "rhc_cmd.h"

typedef struct{
  cmd_t *cmd;
  double fz;
} ctrl_t;

#define ctrl_cmd(self) ( (self)->cmd )
#define ctrl_fz(self)  ( (self)->fz )

ctrl_t *ctrl_init(ctrl_t *self, cmd_t *cmd);
void ctrl_destroy(ctrl_t *self);

#endif /* __RHC_CTRL_H__ */
