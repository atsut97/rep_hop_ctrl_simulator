#ifndef __RHC_CTRL_H__
#define __RHC_CTRL_H__

#include "rhc_cmd.h"
#include "rhc_complex.h"
#include "rhc_model.h"
#include "rhc_vec.h"

typedef struct _ctrl_t{
  cmd_t *cmd;
  model_t *model;
  double fz;
  void *prp;
  double n, phi;
  struct _ctrl_t* (*_update)(struct _ctrl_t*,double,vec_t);
  void (*_destroy)(struct _ctrl_t*);
  void (*_header)(FILE*, void*);
  void (*_writer)(FILE*, struct _ctrl_t*, void*);
} ctrl_t;

#define ctrl_cmd(self)   ((ctrl_t*)self)->cmd
#define ctrl_model(self) ((ctrl_t*)self)->model
#define ctrl_fz(self)    ((ctrl_t*)self)->fz
#define ctrl_z0(self)    ctrl_cmd(self)->z0
#define ctrl_zd(self)    ctrl_cmd(self)->zd
#define ctrl_zb(self)    ctrl_cmd(self)->zb
#define ctrl_n(self)     ((ctrl_t*)self)->n
#define ctrl_phi(self)   ((ctrl_t*)self)->phi

#define ctrl_update(self,t,p)     ((ctrl_t*)self)->_update( self, t, p )
#define ctrl_destroy(self)        ((ctrl_t*)self)->_destroy( self )
#define ctrl_header(fp,self,util) ((ctrl_t*)self)->_header( fp, util )
#define ctrl_writer(fp,self,util) ((ctrl_t*)self)->_writer( fp, self, util )

ctrl_t *ctrl_init(ctrl_t *self, cmd_t *cmd, model_t *model);

bool ctrl_is_in_flight(ctrl_t *self, vec_t p);
bool ctrl_is_in_compression(ctrl_t *self, vec_t p);
bool ctrl_is_in_decompression(ctrl_t *self, vec_t p);

double ctrl_calc_sqr_v0(double z0, double zd);
#define ctrl_calc_v0(z0,zd) sqrt( ctrl_calc_sqr_v0( z0, zd ) )
#define ctrl_sqr_v0(self) ctrl_calc_sqr_v0( ctrl_z0(self), ctrl_zd(self) )
#define ctrl_v0(self) sqrt( ctrl_sqr_v0( self ) )

complex_t *ctrl_calc_phase_complex(double z0, double zd, double zb, vec_t p, complex_t *c);
#define ctrl_phase_complex(self,p,c) ctrl_calc_phase_complex( ctrl_z0(self), ctrl_zd(self), ctrl_zb(self), p, c )
double ctrl_calc_phi(double z0, double zd, double zb, vec_t p);

ctrl_t *ctrl_update_default(ctrl_t *self, double t, vec_t p);
void ctrl_destroy_default(ctrl_t *self);
void ctrl_header_default(FILE *fp, void *util);
void ctrl_writer_default(FILE *fp, ctrl_t *self, void *util);

#endif /* __RHC_CTRL_H__ */
