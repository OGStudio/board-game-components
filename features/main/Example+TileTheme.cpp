FEATURE main.h/Include
#include "ppl.frag.h"
#include "ppl.vert.h"
#include "tile-low.osgt.h"
#include "tile-theme.png.h"
#include "resource.h"

#include "scene.h"

#include <osg/MatrixTransform>

FEATURE main.h/Setup
this->testTileTheme();

FEATURE main.h/Impl
private:
    osg::ref_ptr<osg::MatrixTransform> themeScene;

    void testTileTheme()
    {
        this->themeScene = new osg::MatrixTransform;
        scene::setSimpleRotation(this->themeScene, {45, 0, 0});
        this->scene->addChild(this->themeScene);
        this->setupTexture();

        resource::Resource tile("models", "tile-low.osgt", tile_low_osgt, tile_low_osgt_len);

        auto node = resource::node(tile);
        this->themeScene->addChild(node);
        this->setTileId(node, 0);

        auto leftNode =
            reinterpret_cast<osg::Node *>(node->clone(osg::CopyOp::DEEP_COPY_ALL));
        auto leftNodeTransform =
            new osg::MatrixTransform;
        leftNodeTransform->addChild(leftNode);
        // TODO If leftNode...
        scene::setSimplePosition(leftNodeTransform, {-3, 0, 0});
        this->themeScene->addChild(leftNodeTransform);
        this->setTileId(leftNode, 6);
    }
    void setTileId(osg::Node *node, int id)
    {
        auto group = reinterpret_cast<osg::Group *>(node);
        // TODO If group has been casted successfully...
        // Get geometry from the node.
        auto transform =
            reinterpret_cast<osg::MatrixTransform *>(group->getChild(0));
        // TODO If geode...
        auto geode =
            reinterpret_cast<osg::Geode *>(transform->getChild(0));
        // TODO If geom...
        auto geom = reinterpret_cast<osg::Geometry *>(geode->getDrawable(0));

        MC_MAIN_EXAMPLE_LOG("update geometry to use face of tile id '%d'", id);
        // TODO Extract some entity to manage these specs.
        // NOTE These values are only valid for 1024x2048 texture with
        // NOTE 200x300 tile items.
        const float tileWidth = 0.19531;
        const float tileHeight = 0.14648;

        const int tilesPerRow = 5;

        int row = id / tilesPerRow;
        int column = id - row * tilesPerRow;

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
