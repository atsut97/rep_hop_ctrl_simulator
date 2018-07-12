#include "rhc_logger.h"
#include "rhc_string.h"

logger_t *logger_init(logger_t *self)
{
  self->filename[0] = '\0';
  self->fp = NULL;
  return self;
}

void logger_destroy(logger_t *self)
{
  logger_close( self );
}

FILE* logger_open(logger_t *self, const char *filename)
{
  string_copy(filename, self->filename);
  self->fp = fopen(filename, "w");
  if(!self->fp) {
    RUNTIME_ERR(filename);
    return NULL;
  }
  return self->fp;
}

void logger_close(logger_t *self)
{
  if( self->fp )
    fclose( self->fp );
  self->fp = NULL;
}
