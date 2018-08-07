FEATURE main.h/Impl
#include "log.h"
#include "format.h"
#define MC_MAIN_LOG_PREFIX "main %s"
#define MC_MAIN_LOG(...) \
    log::logprintf( \
        MC_MAIN_LOG_PREFIX, \
        format::printfString(__VA_ARGS__).c_str() \
    )
