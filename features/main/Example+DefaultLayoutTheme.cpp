FEATURE main.h/Include
#include "X_shaped.layout.h"
#include "tile-theme.png.h"

#include "mahjong.h"
#include "scene.h"

FEATURE main.h/Setup
this->setupDefaultLayoutTheme();

FEATURE main.h/Impl
private:
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
        mahjong::Layout layout;
        if (!mahjong::parseLayout(in, layout))
        {
            OMC_MAIN_EXAMPLE_LOG("Could not parse built-in layout");
            return;
        }
        // Reorder layout positions with seed.
        auto positions = mahjong::orderedLayoutPositions(layout.positions, time(0));

        // Create tiles.
        auto matchIds = mahjong::matchIds(positions.size());
        auto tileScene = this->createTiles(positions, matchIds);
        // Apply normal state material to the whole scene.
        tileScene->setStateSet(this->themeMaterial);
        // Rotate the tile scene to have a better view.
        scene::setSimpleRotation(tileScene, {60, 0, 0});
        // Add tile scene to the scene.
        this->scene->addChild(tileScene);
        // Reset the scene.
        this->app->setScene(this->scene);
        // Set texture to materials.
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
