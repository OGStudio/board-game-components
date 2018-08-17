FEATURE mahjong.h/Impl
#include "log.h"
#include "format.h"
#define MC_MAHJONG_LOG_PREFIX "mahjong %s"
#define MC_MAHJONG_LOG(...) \
    log::logprintf( \
        MC_MAHJONG_LOG_PREFIX, \
        format::printfString(__VA_ARGS__).c_str() \
    )
