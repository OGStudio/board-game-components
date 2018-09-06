FEATURE main.h/Include
#include "tile-theme.png.h"

#include "resource.h"

FEATURE main.h/Impl
private:
    resource::Pool *internalThemes;
    void setupInternalThemes()
    {
        // Create pool.
        this->internalThemes = new resource::Pool;

        // Register internal themes.
        {
            resource::Resource res(
                "themes",
                "tile-theme.png",
                tile_theme_png,
                tile_theme_png_len
            );
            this->internalThemes->addResource(res);
        }
    }
    void tearInternalThemesDown()
    {
        delete this->internalThemes;
    }
