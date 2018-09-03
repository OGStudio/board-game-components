FEATURE render.h/Impl
#include "log.h"
#include "format.h"
#define RENDER_TILE_THEME_LOG_PREFIX "render::TileTheme(%p) %s"
#define RENDER_TILE_THEME_LOG(...) \
    log::logprintf( \
        RENDER_TILE_THEME_LOG_PREFIX, \
        this, \
        format::printfString(__VA_ARGS__).c_str() \
    )
