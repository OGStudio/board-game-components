
/*
This file is part of Mahjong components:
  https://github.com/OGStudio/mahjong-components

Copyright (C) 2018 Opensource Game Studio

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef MAHJONG_COMPONENTS_MAIN_H
#define MAHJONG_COMPONENTS_MAIN_H

// Application+frame+Reporting Start
#include "core.h"

// Application+frame+Reporting End
// Application+handleEvent-web Start
#include <SDL2/SDL.h>

// Application+handleEvent-web End

// Application+Logging Start
#include "log.h"

// Application+Logging End
// Application+Rendering Start
#include "render.h"

#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>

// Application+Rendering End

// Example+RemoteLayoutTheme Start
#include "ppl-theme.frag.h"
#include "ppl-theme.vert.h"
#include "tile-low.osgt.h"
#include "layout.h"
#include "resource.h"
#include "scene.h"

#include <osg/MatrixTransform>

// Example+RemoteLayoutTheme End
// Example+Scene Start
#include <osg/MatrixTransform>

// Example+Scene End
// Example+TileTheme Start
#include "render.h"

// Example+TileTheme End
// Example+TileThemeTest Start
#include "ppl-theme.frag.h"
#include "ppl-theme.vert.h"
#include "tile-low.osgt.h"
#include "tile-theme.png.h"
#include "resource.h"

#include "scene.h"

#include <osg/MatrixTransform>

// Example+TileThemeTest End
// Example+VBO Start
#include "render.h"

// Example+VBO End

// MC_MAIN_LOG Start
#include "log.h"
#include "format.h"
#define MC_MAIN_LOG_PREFIX "main %s"
#define MC_MAIN_LOG(...) \
    log::logprintf( \
        MC_MAIN_LOG_PREFIX, \
        format::printfString(__VA_ARGS__).c_str() \
    )
// MC_MAIN_LOG End
// MC_MAIN_EXAMPLE_LOG Start
#include "log.h"
#include "format.h"
#define MC_MAIN_EXAMPLE_LOG_PREFIX "main::Example(%p) %s"
#define MC_MAIN_EXAMPLE_LOG(...) \
    log::logprintf( \
        MC_MAIN_EXAMPLE_LOG_PREFIX, \
        this, \
        format::printfString(__VA_ARGS__).c_str() \
    )
// MC_MAIN_EXAMPLE_LOG End

// Example+StaticPluginOSG Start
// Reference (statically) plugins to read `osgt` file.
USE_OSGPLUGIN(osg2)
USE_SERIALIZER_WRAPPER_LIBRARY(osg)
// Example+StaticPluginOSG End
// Example+StaticPluginPNG Start
// Reference (statically) plugins to read `png` file.
// Apple platforms use ImageIO. All others use libpng.

#ifdef __APPLE__
    USE_OSGPLUGIN(imageio)
#else
    USE_OSGPLUGIN(png)
#endif
// Example+StaticPluginPNG End

namespace mc
{
namespace main
{

// Application Start
class Application
{
    public:
        Application(const std::string &name)
        {

// Application End
            // Application+Logging Start
            this->setupLogging(name);
            
            // Application+Logging End
            // Application+Rendering Start
            this->setupRendering();
            
            // Application+Rendering End
// Application Start
        }
        ~Application()
        {

// Application End
            // Application+Rendering Start
            this->tearRenderingDown();
            
            // Application+Rendering End
            // Application+Logging Start
            this->tearLoggingDown();
            
            // Application+Logging End
// Application Start
        }

// Application End
    // Application+frame+Reporting Start
    public:
        core::Reporter frameReporter;
        void frame()
        {
            this->viewer->frame();
            this->frameReporter.report();
        }
    // Application+frame+Reporting End
    // Application+handleEvent-web Start
    private:
        bool fingerEventsDetected = false;
    public:
        bool handleEvent(const SDL_Event &e)
        {
            // Get event queue.
            osgViewer::GraphicsWindow *gw =
                dynamic_cast<osgViewer::GraphicsWindow *>(
                    this->viewer->getCamera()->getGraphicsContext());
            if (!gw)
            {
                return false;
            }
            osgGA::EventQueue &queue = *(gw->getEventQueue());
    
            // Detect finger events.
            if (
                e.type == SDL_FINGERMOTION ||
                e.type == SDL_FINGERDOWN ||
                e.type == SDL_FINGERUP
            ) {
                fingerEventsDetected = true;
            }
            // Handle mouse events unless finger events are detected.
            if (!fingerEventsDetected)
            {
                return handleMouseEvent(e, queue);
            }
            // Handle finger events.
            return handleFingerEvent(e, queue);
        }
    
    private:
        bool handleFingerEvent(const SDL_Event &e, osgGA::EventQueue &queue)
        {
            int absX = this->windowWidth * e.tfinger.x;
            int absY = this->windowHeight * e.tfinger.y;
            auto correctedY = -(this->windowHeight - absY);
            switch (e.type)
            {
                case SDL_FINGERMOTION:
                    queue.mouseMotion(absX, correctedY);
                    return true;
                case SDL_FINGERDOWN: 
                    queue.mouseButtonPress(absX, correctedY, e.tfinger.fingerId);
                    return true;
                case SDL_FINGERUP:
                    queue.mouseButtonRelease(absX, correctedY, e.tfinger.fingerId);
                    return true;
                default:
                    break;
            }
            return false;
        }
    
        bool handleMouseEvent(const SDL_Event &e, osgGA::EventQueue &queue)
        {
            switch (e.type)
            {
                case SDL_MOUSEMOTION: {
                    auto correctedY = -(this->windowHeight - e.motion.y);
                    queue.mouseMotion(e.motion.x, correctedY);
                    return true;
                }
                case SDL_MOUSEBUTTONDOWN: {
                    auto correctedY = -(this->windowHeight - e.button.y);
                    queue.mouseButtonPress(e.button.x, correctedY, e.button.button);
                    return true;
                }
                case SDL_MOUSEBUTTONUP: {
                    auto correctedY = -(this->windowHeight - e.button.y);
                    queue.mouseButtonRelease(e.button.x, correctedY, e.button.button);
                    return true;
                }
                default:
                    break;
            }
            return false;
        }
    // Application+handleEvent-web End
    // Application+setupWindow-embedded Start
    private:
        int windowWidth;
        int windowHeight;
    public:
        void setupWindow(int width, int height)
        {
            this->viewer->setUpViewerAsEmbeddedInWindow(0, 0, width, height);
            this->windowWidth = width;
            this->windowHeight = height;
        }
    // Application+setupWindow-embedded End

    // Application+Logging Start
    private:
        log::Logger *logger;
        void setupLogging(const std::string &appName)
        {
            // Create custom logger.
            this->logger = new log::Logger(appName);
            // Provide the logger to OpenSceneGraph.
            osg::setNotifyHandler(this->logger);
            // Only accept notifications of Info level or higher
            // like warnings and errors.
            //osg::setNotifyLevel(osg::INFO);
            osg::setNotifyLevel(osg::WARN);
        }
        void tearLoggingDown()
        {
            // Remove the logger from OpenSceneGraph.
            // This also destroys the logger: no need to deallocate it manually.
            osg::setNotifyHandler(0);
        }
    // Application+Logging End
    // Application+Rendering Start
    public:
        void setScene(osg::Node *scene)
        {
            // Make sure we reset the scene upon setting the same scene again.
            this->viewer->setSceneData(0);
            this->viewer->setSceneData(scene);
        }
    private:
        osgViewer::Viewer *viewer;
        void setupRendering()
        {
            // Create OpenSceneGraph viewer.
            this->viewer = new osgViewer::Viewer;
            // Use single thread: CRITICAL for mobile and web.
            this->viewer->setThreadingModel(osgViewer::ViewerBase::SingleThreaded);
            // Create manipulator: CRITICAL for mobile and web.
            this->viewer->setCameraManipulator(new osgGA::TrackballManipulator);
        }
        void tearRenderingDown()
        {
            delete this->viewer;
        }
    // Application+Rendering End
// Application Start
};
// Application End

// Example+03 Start
const auto EXAMPLE_TITLE = "Mc03";
// Example+03 End

// Example Start
struct Example
{
    Application *app;

    typedef std::map<std::string, std::string> Parameters;

    Example(const Parameters &parameters)
    {
        this->app = new Application(EXAMPLE_TITLE);

// Example End
        // Example+Scene Start
        this->setupScene();
        
        // Example+Scene End
        // Example+TileTheme Start
        this->setupTileTheme();
        
        // Example+TileTheme End
        // Example+TileThemeTest Start
        this->setupTileThemeTest();
        
        // Example+TileThemeTest End
        // Example+RemoteLayoutTheme Start
        this->setupRemoteLayoutTheme(parameters);
        
        // Example+RemoteLayoutTheme End
        // Example+VBO Start
        this->setupSceneVBO();
        
        // Example+VBO End
// Example Start
    }
    ~Example()
    {

// Example End
        // Example+TileTheme Start
        this->tearTileThemeDown();
        
        // Example+TileTheme End
// Example Start
        delete this->app;
    }

// Example End
    // Example+RemoteLayoutTheme Start
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
        void loadRemoteLayout(const std::string &url)
        {
            MC_MAIN_EXAMPLE_LOG("Loading layout from '%s'", url.c_str());
            auto success = [&](std::string response) {
                this->parseLayoutResponse(response);
    
                /*
                // Direct stream approach.
                std::istringstream in(response);
                layout::Layout layout;
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
                */
            };
            auto failure = [&](std::string reason) {
                MC_MAIN_EXAMPLE_LOG(
                    "ERROR Could not load layout: %s",
                    reason.c_str()
                );
            };
            this->app->httpClient->get(url, success, failure);
        }
        void parseLayoutResponse(const std::string &response)
        {
            const char *dat0 = response.data();
            auto dat1 = const_cast<char *>(dat0);
            if (!dat1)
            {
                MC_MAIN_EXAMPLE_LOG("ERROR Could not convert const char to char");
                return;
            }
            auto dat = reinterpret_cast<unsigned char *>(dat1);
            if (!dat)
            {
                MC_MAIN_EXAMPLE_LOG("ERROR Could not convert char to unsigned char");
                return;
            }
            resource::Resource layoutRes(
                "layout-remote",
                "todo_urlhere",
                dat,
                response.length()
            );
            layout::Layout layout;
            resource::ResourceStreamBuffer buf(layoutRes);
            std::istream in(&buf);
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
            MC_MAIN_EXAMPLE_LOG("Loading theme from '%s'", url.c_str());
            auto success = [&](std::string response) {
                // Set texture to materials.
                std::istringstream in(response);
                auto texture = resource::createTexture(in);
                this->normalMaterial->setTextureAttributeAndModes(0, texture);
                this->selectedMaterial->setTextureAttributeAndModes(0, texture);
                MC_MAIN_EXAMPLE_LOG("Successfully loaded theme");
            };
            auto failure = [&](std::string reason) {
                MC_MAIN_EXAMPLE_LOG(
                    "ERROR Could not load theme: %s",
                    reason.c_str()
                );
            };
            this->app->httpClient->get(url, success, failure);
        }
        void createTiles(const layout::Layout &layout)
        {
            for (auto pos : layout.positions)
            {
                float z = pos.x();
                float y = pos.y() * 1.5;
                float x = pos.z();
                auto tile = new osg::Geode(*this->tile, osg::CopyOp::DEEP_COPY_ALL);
                auto node = new osg::MatrixTransform;
                node->addChild(tile);
                this->tileScene->addChild(node);
                scene::setSimplePosition(node, {x, y, z});
            }
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
    // Example+RemoteLayoutTheme End
    // Example+Scene Start
    private:
        osg::ref_ptr<osg::MatrixTransform> scene;
        const std::string sceneSetupCallbackName = "SceneSetup";
    
        void setupScene()
        {
            this->scene = new osg::MatrixTransform;
    
            // Provide scene to application after the first frame
            // to let other components configure scene prior that event.
            this->app->frameReporter.addCallback(
                [&] {
                    this->app->setScene(this->scene);
                    // Unsubscribe from the rest of frame reports.
                    this->app->frameReporter.removeCallback(
                        this->sceneSetupCallbackName
                    );
                },
                this->sceneSetupCallbackName
            );
        }
    // Example+Scene End
    // Example+TileTheme Start
    private:
        render::TileTheme *tileTheme;
        const osg::Vec2 textureSize = {1024, 2048};
        const osg::Vec2 tileFaceSize = {160, 240};
        const render::TileTheme::Indices faceIndices = {15, 23, 16, 17};
    
        void setupTileTheme()
        {
            this->tileTheme =
                new render::TileTheme(
                    this->textureSize,
                    this->tileFaceSize,
                    this->faceIndices
                );
        }
        void tearTileThemeDown()
        {
            delete this->tileTheme;
        }
    // Example+TileTheme End
    // Example+TileThemeTest Start
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
    // Example+TileThemeTest End
    // Example+VBO Start
    private:
        void setupSceneVBO()
        {
            // Do nothing for an empty scene.
            if (!this->scene.valid())
            {
                return;
            }
            // Use VBO and EBO instead of display lists.
            // CRITICAL for:
            // * mobile
            // * web (Emscripten) to skip FULL_ES2 emulation flag
            render::VBOSetupVisitor vbo;
            this->scene->accept(vbo);
        }
    // Example+VBO End
// Example Start
};
// Example End

} // namespace main
} // namespace mc

#endif // MAHJONG_COMPONENTS_MAIN_H

