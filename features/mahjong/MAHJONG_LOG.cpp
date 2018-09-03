FEATURE mahjong.h/Impl
#include "log.h"
#include "format.h"
#define MAHJONG_LOG_PREFIX "mahjong %s"
#define MAHJONG_LOG(...) \
    log::logprintf( \
        MAHJONG_LOG_PREFIX, \
        format::printfString(__VA_ARGS__).c_str() \
    )
