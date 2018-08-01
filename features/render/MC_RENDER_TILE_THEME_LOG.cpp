FEATURE render.h/Impl
#include "log.h"
#include "format.h"
#define MC_RENDER_TILE_THEME_LOG_PREFIX "render::TileTheme(%p) %s"
#define MC_RENDER_TILE_THEME_LOG(...) \
    log::logprintf( \
        MC_RENDER_TILE_THEME_LOG_PREFIX, \
        this, \
        format::printfString(__VA_ARGS__).c_str() \
    )
