FEATURE main.h/Include
#include "ppl-theme.frag.h"
#include "ppl-theme.vert.h"
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
    osg::ref_ptr<osg::StateSet> normalMaterial;
    osg::ref_ptr<osg::StateSet> selectedMaterial;
    void setupTileThemeTest()
    {
        // Create tile scene to host tiles.
        this->tileScene = new osg::MatrixTransform;
        this->scene->addChild(this->tileScene);

        this->setupMaterials();
        // Apply normal state material to the whole scene.
        this->tileScene->setStateSet(this->normalMaterial);

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

        // Configure tile.
        this->tileTheme->setFaceId(0, tile);
        // Add it to the scene.
        this->tileScene->addChild(tile);

        // Create another tile.
        auto leftTile = new osg::Geode(*tile, osg::CopyOp::DEEP_COPY_ALL);
        // Configure it.
        this->tileTheme->setFaceId(6, leftTile);
        // Move it to the left.
        auto leftTransform = new osg::MatrixTransform;
        leftTransform->addChild(leftTile);
        scene::setSimplePosition(leftTransform, {-3, 0, 0});
        // Add it to the scene.
        this->tileScene->addChild(leftTransform);
        // Assign selected state material to the left tile.
        leftTile->setStateSet(this->selectedMaterial);

        // Create one more tile.
        auto rightTile = new osg::Geode(*tile, osg::CopyOp::DEEP_COPY_ALL);
        // Configure it.
        this->tileTheme->setFaceId(3, rightTile);
        // Move it to the right.
        auto rightTransform = new osg::MatrixTransform;
        rightTransform->addChild(rightTile);
        scene::setSimplePosition(rightTransform, {3, 0, 0});
        // Add it to the scene.
        this->tileScene->addChild(rightTransform);
    }
    void setupMaterials()
    {
        // Create resources.
        resource::Resource shaderFrag(
            "shaders",
            "ppl-theme.frag",
            ppl_theme_frag,
            ppl_theme_frag_len
        );
        resource::Resource shaderVert(
            "shaders",
            "ppl-theme.vert",
            ppl_theme_vert,
            ppl_theme_vert_len
        );
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
        // Create texture.
        auto texture = resource::createTexture(texRes);

        // Create normal state material.
        this->normalMaterial = new osg::StateSet;
        this->normalMaterial->setAttribute(prog);
        this->normalMaterial->setTextureAttributeAndModes(0, texture);
        this->normalMaterial->addUniform(new osg::Uniform("image", 0));
        this->normalMaterial->addUniform(new osg::Uniform("isSelected", false));

        // Create selected state material.
        this->selectedMaterial = new osg::StateSet;
        this->selectedMaterial->setAttribute(prog);
        this->selectedMaterial->setTextureAttributeAndModes(0, texture);
        this->selectedMaterial->addUniform(new osg::Uniform("image", 0));
        this->selectedMaterial->addUniform(new osg::Uniform("isSelected", true));
    }
