#include "rhc_misc.h"
#include "rhc_logger.h"
#include "rhc_string.h"

logger_t *logger_init(logger_t *self)
{
  self->filename[0] = '\0';
  self->fp = NULL;
  self->header = NULL;
  self->writer = NULL;
  self->header_written_flag = false;
  string_copy( "\n", self->eol );
  return self;
}

void logger_destroy(logger_t *self)
{
  logger_close( self );
}

logger_t *logger_create(const char *filename, logger_header_fp_t header, logger_writer_fp_t writer)
{
  logger_t *logger;

  if( ( logger = nalloc( logger_t, 1 ) ) == NULL ) {
    ALLOC_ERR();
    return NULL;
  }
  logger_init( logger );
  logger_open( logger, filename );
  logger_register( logger, header, writer );
  return logger;
}

logger_t *logger_set_eol(logger_t *self, const char *eol)
{
  string_copy( eol, self->eol );
  return self;
}

FILE* logger_open(logger_t *self, const char *filename)
{
  if( filename ){
    string_copy(filename, self->filename);
    self->fp = fopen(filename, "w");
    if( !self->fp ){
      RUNTIME_ERR(filename);
      return NULL;
    }
  } else{
    self->filename[0] = '\0';
    self->fp = stdout;
  }
  self->header_written_flag = false;
  return self->fp;
}

void logger_close(logger_t *self)
{
  if( self->fp && self->fp != stdout )
    fclose( self->fp );
  self->fp = NULL;
}

void logger_register(logger_t *self, logger_header_fp_t header, logger_writer_fp_t writer)
{
  self->header = header;
  self->writer = writer;
}

void logger_delegate(logger_t *src, logger_t *dst)
{
  logger_init( dst );
  string_copy( logger_filename(src), logger_filename(dst) );
  dst->fp = src->fp;
  dst->header = src->header;
  dst->writer = src->writer;
  dst->header_written_flag = src->header_written_flag;
  string_copy( logger_eol(src), logger_eol(dst) );
  logger_init( src );
}

logger_t *logger_reset(logger_t *self, const char *filename)
{
  logger_close( self );
  logger_open( self, filename );
  return self;
}

void logger_write_header(logger_t *self, simulator_t *simulator, void *util)
{
  if( !self->fp ) return;
  if( !self->header ) return;
  self->header( self->fp, simulator, util );
  fprintf( self->fp, "%s", logger_eol(self) );
  self->header_written_flag = true;
}

void logger_write_data(logger_t *self, simulator_t *simulator, void *util)
{
  if( !self->fp ) return;
  if( !self->writer ) return;
  self->writer( self->fp, simulator, util );
  fprintf( self->fp, "%s", logger_eol(self) );
}

void logger_write(logger_t *self, simulator_t *simulator, void *util)
{
  if( !logger_is_header_written(self) )
    logger_write_header( self, simulator, util );
  logger_write_data( self, simulator, util );
}
