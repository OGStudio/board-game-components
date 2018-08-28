FEATURE main.h/Include
#include "scene.h"
#include <osg/MatrixTransform>

FEATURE main.h/Setup
this->setupThemeTest();

FEATURE main.h/Impl
private:
    osg::ref_ptr<osg::MatrixTransform> tileScene;
    void setupThemeTest()
    {
        // Create tile scene to host tiles.
        this->tileScene = new osg::MatrixTransform;
        this->scene->addChild(this->tileScene);
        // Apply normal state material to the whole scene.
        this->tileScene->setStateSet(this->themeMaterial);

        // Rotate the tile scene to have a better view.
        scene::setSimpleRotation(this->tileScene, {60, 0, 0});

        // Configure tile.
        this->theme->setFaceId(0, this->tileModel);
        // Add it to the scene.
        this->tileScene->addChild(this->tileModel);

        // Create another tile.
        auto leftTile = new osg::Geode(*this->tileModel, osg::CopyOp::DEEP_COPY_ALL);
        // Configure it.
        this->theme->setFaceId(6, leftTile);
        // Move it to the left.
        auto leftTransform = new osg::MatrixTransform;
        leftTransform->addChild(leftTile);
        scene::setSimplePosition(leftTransform, {-3, 0, 0});
        // Add it to the scene.
        this->tileScene->addChild(leftTransform);
        // Assign selected state material to the left tile.
        leftTile->setStateSet(this->themeMaterialSelected);

        // Create one more tile.
        auto rightTile = new osg::Geode(*this->tileModel, osg::CopyOp::DEEP_COPY_ALL);
        // Configure it.
        this->theme->setFaceId(3, rightTile);
        // Move it to the right.
        auto rightTransform = new osg::MatrixTransform;
        rightTransform->addChild(rightTile);
        scene::setSimplePosition(rightTransform, {3, 0, 0});
        // Add it to the scene.
        this->tileScene->addChild(rightTransform);

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
