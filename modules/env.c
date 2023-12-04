#include "env.h"
#include "fail.h"

#include <stdlib.h>

API const char* LIBR(getenv_mandatory)(const char* const env)
{
    const char* const v = getenv(env);
    if(v == NULL) { failwith("environment variable %s not set", env); }
    return v;
}
