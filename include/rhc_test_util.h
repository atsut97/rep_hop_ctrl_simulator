#ifndef __RHC_TEST_UTIL_H__
#define __RHC_TEST_UTIL_H__

#include "rhc_test.h"

int set_spurious_values(void *object, size_t size);

#define SPURIOUS_VALUES_ERR_MSG "error while setting spurious values"
#define SHAM(obj,type) do{\
  ASSERT( !set_spurious_values( obj, sizeof(type) ), SPURIOUS_VALUES_ERR_MSG );\
} while( 0 )


#endif /* __RHC_TEST_UTIL_H__ */
