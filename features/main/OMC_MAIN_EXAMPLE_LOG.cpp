FEATURE main.h/Impl
#include "log.h"
#include "format.h"
#define OMC_MAIN_EXAMPLE_LOG_PREFIX "main::Example(%p) %s"
#define OMC_MAIN_EXAMPLE_LOG(...) \
    log::logprintf( \
        OMC_MAIN_EXAMPLE_LOG_PREFIX, \
        this, \
        format::printfString(__VA_ARGS__).c_str() \
    )
