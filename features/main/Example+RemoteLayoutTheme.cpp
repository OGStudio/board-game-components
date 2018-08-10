FEATURE main.h/Include
#include "ppl-theme.frag.h"
#include "ppl-theme.vert.h"
#include "tile-low.osgt.h"
#include "layout.h"
#include "resource.h"
#include "scene.h"

#include <osg/MatrixTransform>

FEATURE main.h/Setup
this->setupRemoteLayoutTheme(parameters);

FEATURE main.h/Impl
private:
    osg::ref_ptr<osg::MatrixTransform> tileScene;
    osg::ref_ptr<osg::StateSet> normalMaterial;
    osg::ref_ptr<osg::StateSet> selectedMaterial;
    osg::ref_ptr<osg::Geode> tile;

    void setupRemoteLayoutTheme(const Parameters &parameters)
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
        this->tile = reinterpret_cast<osg::Geode *>(node.get());
        // Make sure tile is valid.
        if (!this->tile)
        {
            MC_MAIN_EXAMPLE_LOG(
                "ERROR Could not load model '%s/%s'",
                res.group.c_str(),
                res.name.c_str()
            );
            return;
        }

        // Load remote layout and/or theme.
        for (auto parameter : parameters)
        {
            auto key = parameter.first;
            auto value = parameter.second;
            if (key == "layout")
            {
                this->loadRemoteLayout(value);
            }
            else if (key == "theme")
            {
                this->loadRemoteTheme(value);
            }
        }
    }

    void createTiles(const layout::Layout &layout)
    {
        int facesCount = 42;
        int faceId = 0;
        for (auto pos : layout.positions)
        {
            // Clone tile.
            auto tile = new osg::Geode(*this->tile, osg::CopyOp::DEEP_COPY_ALL);
            // Set its position.
            float z = pos.x();
            float y = pos.y() * 1.5 /* Factor depends on the model */;
            float x = pos.z();
            auto node = new osg::MatrixTransform;
            node->addChild(tile);
            scene::setSimplePosition(node, {x, y, z});
            // Add tile to the scene.
            this->tileScene->addChild(node);
            // Set tile face id.
            this->tileTheme->setFaceId(faceId, tile);
            // Cycle face id.
            if (++faceId >= facesCount)
            {
                faceId = 0;
            }

        }
    }
    void loadRemoteLayout(const std::string &url)
    {
        auto success = [&](std::string response) {
            this->parseLayoutResponse(response, url);
        };
        auto failure = [&](std::string reason) {
            MC_MAIN_EXAMPLE_LOG(
                "ERROR Could not load layout: %s",
                reason.c_str()
            );
        };
        MC_MAIN_EXAMPLE_LOG("Loading layout from '%s'", url.c_str());
        this->app->httpClient->get(url, success, failure);
    }
    void parseLayoutResponse(
        const std::string &response,
        const std::string &url
    ) {
        layout::Layout layout;
        std::istringstream in(response);
        if (layout::parseLayout(in, layout))
        {
            this->createTiles(layout);
            // Reset scene.
            this->app->setScene(this->scene);
            MC_MAIN_EXAMPLE_LOG("Successfully loaded layout");
        }
        else
        {
            MC_MAIN_EXAMPLE_LOG("ERROR Could not parse loaded layout");
        }
    }

    void loadRemoteTheme(const std::string &url)
    {
        auto success = [=](std::string response) {
            // NOTE We use `=` in lambda capture to capture url copy
            // NOTE Otherwise we have crash when parsing.
            this->parseThemeResponse(response, url);
        };
        auto failure = [&](std::string reason) {
            MC_MAIN_EXAMPLE_LOG(
                "ERROR Could not load theme: %s",
                reason.c_str()
            );
        };
        MC_MAIN_EXAMPLE_LOG("Loading theme from '%s'", url.c_str());
        this->app->httpClient->get(url, success, failure);
    }
    void parseThemeResponse(const std::string &response, const std::string &url)
    {
        resource::Resource
            themeRes(
                "theme-remote",
                url,
                resource::stringToResourceContents(response),
                response.length()
            );
        // Set texture to materials.
        auto texture = resource::createTexture(themeRes);
        this->normalMaterial->setTextureAttributeAndModes(0, texture);
        this->selectedMaterial->setTextureAttributeAndModes(0, texture);
        MC_MAIN_EXAMPLE_LOG("Successfully loaded theme");
    }
    void setupMaterials()
    {
        // Create built-in resources.
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

        // Create shader program.
        auto prog =
            render::createShaderProgram(
                resource::string(shaderVert),
                resource::string(shaderFrag)
            );

        // Create normal state material.
        this->normalMaterial = new osg::StateSet;
        this->normalMaterial->setAttribute(prog);
        this->normalMaterial->addUniform(new osg::Uniform("image", 0));
        this->normalMaterial->addUniform(new osg::Uniform("isSelected", false));

        // Create selected state material.
        this->selectedMaterial = new osg::StateSet;
        this->selectedMaterial->setAttribute(prog);
        this->selectedMaterial->addUniform(new osg::Uniform("image", 0));
        this->selectedMaterial->addUniform(new osg::Uniform("isSelected", true));
    }
