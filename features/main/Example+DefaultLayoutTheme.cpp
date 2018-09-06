FEATURE main.h/Include
#include "tile-theme.png.h"
#include "mahjong.h"

FEATURE main.h/Impl
private:
    mahjong::Layout layout;
    void setupDefaultLayoutTheme()
    {
        // Load internal "X_shaped.layout" by default.
        auto layoutResource =
            this->internalLayouts->resource("layouts", "X_shaped.layout");
        if (!layoutResource)
        {
            MAIN_EXAMPLE_LOG("ERROR Could not locate internal layout");
            return;
        }
        resource::ResourceStreamBuffer buf(*layoutResource);
        std::istream in(&buf);
        if (!mahjong::parseLayout(in, this->layout))
        {
            MAIN_EXAMPLE_LOG("ERROR Could not parse built-in layout");
            return;
        }

        // Set theme materials.
        resource::Resource texRes(
            "textures",
            "tile-theme.png",
            tile_theme_png,
            tile_theme_png_len
        );
        auto texture = resource::createTexture(texRes);
        this->themeMaterial->setTextureAttributeAndModes(0, texture);
        this->themeMaterialSelected->setTextureAttributeAndModes(0, texture);
    }
