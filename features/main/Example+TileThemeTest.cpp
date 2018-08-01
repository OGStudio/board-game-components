FEATURE main.h/Include
#include "ppl.frag.h"
#include "ppl.vert.h"
#include "tile-low.osgt.h"
#include "tile-theme.png.h"
#include "resource.h"

#include "scene.h"

#include <osg/MatrixTransform>

FEATURE main.h/Setup
this->setupTileThemeTest();

FEATURE main.h/Impl
private:
    osg::ref_ptr<osg::MatrixTransform> tileScene;
    void setupTileThemeTest()
    {
        // Create tile scene to host tiles.
        this->tileScene = new osg::MatrixTransform;
        this->scene->addChild(this->tileScene);
        // Setup single texture atlas for the tile scene.
        this->setupTexture();
        // Rotate the tile scene to have a better view.
        scene::setSimpleRotation(this->tileScene, {60, 0, 0});

        // Load model with geode only.
        resource::Resource res(
            "models",
            "tile-low.osgt",
            tile_low_osgt,
            tile_low_osgt_len
        );
        auto node = resource::node(res);
        auto tile = reinterpret_cast<osg::Geode *>(node.get());
        // Make sure tile is valid.
        if (!tile)
        {
            MC_MAIN_EXAMPLE_LOG(
                "ERROR Could not load model '%s/%s'",
                res.group.c_str(),
                res.name.c_str()
            );
            return;
        }
        // This specific model has four face texture coordinates at 20th position.
        const int texCoordStartIndex = 20;

        // Configure tile.
        this->tileTheme->setFaceId(0, tile, texCoordStartIndex);
        // Add it to the scene.
        this->tileScene->addChild(tile);

        // Create another tile.
        auto leftTile = new osg::Geode(*tile, osg::CopyOp::DEEP_COPY_ALL);
        // Configure it.
        this->tileTheme->setFaceId(6, leftTile, texCoordStartIndex);
        // Move it to the left.
        auto leftTransform = new osg::MatrixTransform;
        leftTransform->addChild(leftTile);
        scene::setSimplePosition(leftTransform, {-3, 0, 0});
        // Add it to the scene.
        this->tileScene->addChild(leftTransform);

        // Create one more tile.
        auto rightTile = new osg::Geode(*tile, osg::CopyOp::DEEP_COPY_ALL);
        // Configure it.
        this->tileTheme->setFaceId(3, rightTile, texCoordStartIndex);
        // Move it to the right.
        auto rightTransform = new osg::MatrixTransform;
        rightTransform->addChild(rightTile);
        scene::setSimplePosition(rightTransform, {3, 0, 0});
        // Add it to the scene.
        this->tileScene->addChild(rightTransform);
    }
    void setupTexture()
    {
        // Create resources.
        resource::Resource shaderFrag("shaders", "ppl.frag", ppl_frag, ppl_frag_len);
        resource::Resource shaderVert("shaders", "ppl.vert", ppl_vert, ppl_vert_len);
        resource::Resource texRes(
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
        auto material = this->tileScene->getOrCreateStateSet();
        material->setAttribute(prog);
        // Set texture image.
        auto texture = resource::createTexture(texRes);
        material->setTextureAttributeAndModes(0, texture);
        material->addUniform(new osg::Uniform("image", 0));
    }
