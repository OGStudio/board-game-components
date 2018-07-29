FEATURE layout.h/Impl
#include "log.h"
#include "format.h"
#define MC_LAYOUT_LOG_PREFIX "mc::layout %s"
#define MC_LAYOUT_LOG(...) \
    log::logprintf( \
        MC_LAYOUT_LOG_PREFIX, \
        format::printfString(__VA_ARGS__).c_str() \
    )
