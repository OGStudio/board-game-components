FEATURE render.h/Impl
#include "log.h"
#include "format.h"
#define OMC_RENDER_TILE_THEME_LOG_PREFIX "render::TileTheme(%p) %s"
#define OMC_RENDER_TILE_THEME_LOG(...) \
    log::logprintf( \
        OMC_RENDER_TILE_THEME_LOG_PREFIX, \
        this, \
        format::printfString(__VA_ARGS__).c_str() \
    )
