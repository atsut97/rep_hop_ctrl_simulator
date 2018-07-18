#ifndef __RHC_LOGGER_H__
#define __RHC_LOGGER_H__

#include "rhc_misc.h"
#include "rhc_cmd.h"
#include "rhc_ctrl.h"
#include "rhc_model.h"

typedef void (*logger_writer_fp_t)(FILE* fp, double t, vec_t state, double fe, cmd_t *cmd, model_t *model, void *util);

typedef struct{
  char filename[BUFSIZ];
  FILE* fp;
  logger_writer_fp_t writer;
} logger_t;

#define logger_filename(self) (self)->filename
#define logger_is_open(self)  ( (self)->fp != NULL )

logger_t *logger_init(logger_t *self);
void logger_destroy(logger_t *self);

FILE* logger_open(logger_t *self, const char *filename);
void logger_close(logger_t *self);
void logger_register(logger_t *self, logger_writer_fp_t writer);
void logger_write(logger_t *self, double t, vec_t state, double fe, cmd_t *cmd, model_t *model, void *util);

#endif /* __RHC_LOGGER_H__ */
