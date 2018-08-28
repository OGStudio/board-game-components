FEATURE mahjong.h/Impl
#include "log.h"
#include "format.h"
#define OMC_MAHJONG_LOG_PREFIX "mahjong %s"
#define OMC_MAHJONG_LOG(...) \
    log::logprintf( \
        OMC_MAHJONG_LOG_PREFIX, \
        format::printfString(__VA_ARGS__).c_str() \
    )
