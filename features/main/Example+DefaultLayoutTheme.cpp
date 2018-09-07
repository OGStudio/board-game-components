FEATURE main.h/Include
#include "mahjong.h"

FEATURE main.h/Impl
private:
    mahjong::Layout layout;
    void setupDefaultLayoutTheme()
    {
        // Load internal "X_shaped.layout" layout by default.
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
            MAIN_EXAMPLE_LOG("ERROR Could not parse internal layout");
            return;
        }

        // Load internal "numbers-theme.png" theme by default.
        auto themeResource =
            this->internalThemes->resource("themes", "numbers-theme.png");
        if (!themeResource)
        {
            MAIN_EXAMPLE_LOG("ERROR Could not locate internal theme");
            return;
        }
        auto texture = resource::createTexture(*themeResource);
        this->themeMaterial->setTextureAttributeAndModes(0, texture);
        this->themeMaterialSelected->setTextureAttributeAndModes(0, texture);
    }
