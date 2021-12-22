#ifndef __RHC_CTRL_RAIBERT_H__
#define __RHC_CTRL_RAIBERT_H__

#include "rhc_ctrl.h"

enum ctrl_raibert_type {
  none = -1,
  full_nonlinear = 0,
  simplified_nonlinear,
  full_linear,
  simplified_linear,
};

typedef struct{
  enum ctrl_raibert_type type;  /* controller type */
  double delta;                 /* thrust time */
  double tau;                   /* thrust force */
  double gamma;                 /* friction constant */
  double yeta1;                 /* initial spring constant */
  double zr;                    /* spring relax position */
  double mu;                    /* mass ration */
  double tb;                    /* time at the bottom */
  double zb;                    /* position at the bottom */
  double tet;                   /* time at the end of thrust phase */
  double zet;                   /* position at the ned of thrust phase */
} ctrl_raibert_prp;

ctrl_t *ctrl_raibert_create(ctrl_t *self, cmd_t *cmd, model_t *model);
void ctrl_raibert_destroy(ctrl_t *self);
ctrl_t *ctrl_raibert_update(ctrl_t *self, double t, vec_t p);
void ctrl_raibert_header(FILE *fp, void *util);
void ctrl_raibert_writer(FILE *fp, ctrl_t *self, void *util);

#endif /* __RHC_CTRL_RAIBERT_H__ */
