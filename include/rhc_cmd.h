#ifndef __RHC_CMD_H__
#define __RHC_CMD_H__

#include "rhc_misc.h"

typedef struct{
  double za;       /* apex height */
  double zh;       /* lift-off height */
  double zm;       /* standing height */
  double zb;       /* crouching height */
  union {
    struct _regulator {
      double q1, q2;
    } regulator;
    struct _rep_hop_stand {
      double rho, k;
    } rep_hop_stand;
    struct _raibert {
      double delta, tau, gamma, yeta1, zr, mu;
    } raibert;
    struct _arl {
      double k, beta;
    } arl;
    struct _mtoka {
      double tau, T, a, b, c, th, mu, rho, lam;
    } mtoka;
  };
} cmd_t;

cmd_t *cmd_init(cmd_t *self);
cmd_t *cmd_default_init(cmd_t *self);
void cmd_destroy(cmd_t *self);

cmd_t *cmd_set(cmd_t *self, double za, double zh, double zm, double zb);

cmd_t *cmd_copy(cmd_t *src, cmd_t *dst);

#endif /* __RHC_CMD_H__ */
