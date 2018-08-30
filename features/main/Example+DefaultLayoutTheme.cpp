FEATURE main.h/Include
#include "X_shaped.layout.h"
#include "tile-theme.png.h"

#include "mahjong.h"

FEATURE main.h/Impl
private:
    mahjong::Layout layout;
    void setupDefaultLayoutTheme()
    {
        // Load default built-in layout.
        resource::Resource layoutResource(
            "layouts",
            "X_shaped.layout",
            X_shaped_layout,
            X_shaped_layout_len
        );
        resource::ResourceStreamBuffer buf(layoutResource);
        std::istream in(&buf);
        if (!mahjong::parseLayout(in, this->layout))
        {
            OMC_MAIN_EXAMPLE_LOG("Could not parse built-in layout");
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
