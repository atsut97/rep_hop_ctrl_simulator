#include <fcntl.h>
#include "rhc_test_util.h"
#include "rhc_misc.h"

int set_spurious_values(void *object, size_t size)
{
  int rnd = open("/dev/urandom", O_RDONLY);
  if( rnd < 0 ){
    RUNTIME_ERR("opening \"/dev/urandom\" has failed");
    return 1;
  } else {
    size_t len = 0;
    while( len < size ){
      ssize_t result = read( rnd, object + len, size - len );
      if( result < 0 ){
        RUNTIME_ERR("reading file handler \"rnd\" has failed");
        close( rnd );
        return 1;
      }
      len += result;
    }
    close( rnd );
  }
  return 0;
}
