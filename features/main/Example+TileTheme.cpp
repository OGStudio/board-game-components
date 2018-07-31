FEATURE main.h/Include
#include "ppl.frag.h"
#include "ppl.vert.h"
#include "tile-low.osgt.h"
#include "tile-theme.png.h"
#include "resource.h"

#include <osg/MatrixTransform>

FEATURE main.h/Setup
this->testTileTheme();

FEATURE main.h/Impl
private:
    osg::ref_ptr<osg::MatrixTransform> themeScene;

    void testTileTheme()
    {
        this->themeScene = new osg::MatrixTransform;
        this->scene->addChild(this->themeScene);

        resource::Resource tile("models", "tile-low.osgt", tile_low_osgt, tile_low_osgt_len);
        auto node = resource::node(tile);
        this->themeScene->addChild(node);
        auto group = reinterpret_cast<osg::Group *>(node.get());

        // TODO If group has been casted successfully...
        // Get geometry from the node.
        auto transform =
            reinterpret_cast<osg::MatrixTransform *>(group->getChild(0));
        // TODO If geode...
        auto geode =
            reinterpret_cast<osg::Geode *>(transform->getChild(0));
        // TODO If geom...
        auto geom = reinterpret_cast<osg::Geometry *>(geode->getDrawable(0));

        // Configure geometry.
        this->setTileIdForGeometry(1, geom);
        this->setupTexture();
    }
    void setTileIdForGeometry(int tileId, osg::Geometry *geom)
    {
        MC_MAIN_EXAMPLE_LOG("update geometry of tile id '%d'", tileId);
        // TODO Extract some entity to manage these specs.
        // NOTE These values are only valid for 1024x2048 texture with
        // NOTE 200x300 tile items.
        const float tileWidth = 0.19531;
        const float tileHeight = 0.14648;

        const int tilesPerRow = 5;

        int row = tileId / tilesPerRow;
        int column = tileId - row * tilesPerRow;

        const osg::Vec2 topRight = {
            tileWidth * (column + 1),
            1.0f - tileHeight * row
        };
        const osg::Vec2 topLeft = {
            tileWidth * column,
            1.0f - tileHeight * row
        };
        const osg::Vec2 bottomLeft = {
            tileWidth * column,
            1.0f - tileHeight * (row + 1)
        };
        const osg::Vec2 bottomRight = {
            tileWidth * (column + 1),
            1.0f - tileHeight * (row + 1)
        };

        auto texCoordsCount = geom->getNumTexCoordArrays();
        if (!texCoordsCount)
        {
            MC_MAIN_EXAMPLE_LOG("ERROR Model has no texture coordinates");
            return;
        }

        // Take the first array.
        auto coords = reinterpret_cast<osg::Vec2Array *>(geom->getTexCoordArray(0));
        // TODO If coords...

        // Change the last four coordinates.
        auto index = coords->size() - 4;
        (*coords)[index++] = topRight;
        (*coords)[index++] = topLeft;
        (*coords)[index++] = bottomLeft;
        (*coords)[index] = bottomRight;

        // NOTE Other models would need other vertices to be overrident
        // NOTE they might not be the last ones. So be ready to update
        // NOTE at any offset.
        // TODO Override last four vertices of the geometry.

        /* NOTE
         *
         * tile id = 1 coords:
         * (top-right) w, 1.0
         * (top-left) 0.0, 1.0
         * (bottom-left) 0.0, 1.0 - h
         * (bottom-right) w, 1.0 - h
         *
         * tile id = 2 coords:
         * (tr) w * 2, 1.0
         * (tl) w, 1.0
         * (bl) w, 1.0 - h
         * (br) w * 2, 1.0 - h
         *
         * tile id = 3 coords:
         * (tr) w * 3, 1.0
         * (tl) w * 2, 1.0
         * (bl) w * 2, 1.0 - h
         * (br) w * 3, 1.0 -h
         *
         * tile id = 4 coords:
         * (tr) w, 1.0 - h
         * (tl) 0.0, 1.0 - h
         * (bl) 0.0, 1.0 - h * 2
         * (br) w, 1.0 - h * 2
         */



    }
    void setupTexture()
    {
        resource::Resource shaderFrag("shaders", "ppl.frag", ppl_frag, ppl_frag_len);
        resource::Resource shaderVert("shaders", "ppl.vert", ppl_vert, ppl_vert_len);
        resource::Resource tileTex(
            "textures",
            "tile-theme.png",
            tile_theme_png,
            tile_theme_png_len
        );

        // Create shader program.
        auto prog =
            render::createShaderProgram(
                resource::string(shaderVert),
                resource::string(shaderFrag)
            );
        // Apply the program.
        auto material = this->themeScene->getOrCreateStateSet();
        material->setAttribute(prog);
        // Set texture image.
        material->setTextureAttributeAndModes(0, resource::createTexture(tileTex));
        material->addUniform(new osg::Uniform("image", 0));
    }
