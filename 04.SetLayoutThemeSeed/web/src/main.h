
/*
This file is part of OGS Mahjong components:
  https://github.com/OGStudio/ogs-mahjong-components

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

#ifndef OGS_MAHJONG_COMPONENTS_MAIN_H
#define OGS_MAHJONG_COMPONENTS_MAIN_H

// Application+frame+Reporting Start
#include "core.h"

// Application+frame+Reporting End
// Application+handleEvent-web Start
#include <SDL2/SDL.h>

// Application+handleEvent-web End

// Application+HTTPClient Start
#include "network.h"

// Application+HTTPClient End
// Application+Logging Start
#include "log.h"

// Application+Logging End
// Application+Rendering Start
#include "render.h"

#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>

// Application+Rendering End

// Example+Scene Start
#include <osg/MatrixTransform>

// Example+Scene End
// Example+Theme Start
#include "render.h"
#include "resource.h"

#include "ppl-theme.frag.h"
#include "ppl-theme.vert.h"
#include "tile-low.osgt.h"

// Example+Theme End
// Example+VBO Start
#include "render.h"

// Example+VBO End

// Example+createTiles Start
#include "mahjong.h"
#include "scene.h"
#include <osg/MatrixTransform>

// Example+createTiles End


// Example+StaticPluginOSG Start
#include <osgDB/Registry>

// Reference (statically) plugins to read `osgt` file.
USE_OSGPLUGIN(osg2)
USE_SERIALIZER_WRAPPER_LIBRARY(osg)
// Example+StaticPluginOSG End
// Example+StaticPluginPNG Start
#include <osgDB/Registry>

// Reference (statically) plugins to read `png` file.
// Apple platforms use ImageIO. All others use libpng.
#ifdef __APPLE__
    USE_OSGPLUGIN(imageio)
#else
    USE_OSGPLUGIN(png)
#endif
// Example+StaticPluginPNG End

namespace omc
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
            // Application+HTTPClient Start
            this->setupHTTPClient();
            
            // Application+HTTPClient End
            // Application+HTTPClientProcessor Start
            this->setupHTTPClientProcessor();
            
            // Application+HTTPClientProcessor End
// Application Start
        }
        ~Application()
        {

// Application End
            // Application+HTTPClientProcessor Start
            this->tearHTTPClientProcessorDown();
            
            // Application+HTTPClientProcessor End
            // Application+HTTPClient Start
            this->tearHTTPClientDown();
            
            // Application+HTTPClient End
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
                this->fingerEventsDetected = true;
            }
            // Handle mouse events unless finger events are detected.
            if (!this->fingerEventsDetected)
            {
                return this->handleMouseEvent(e, queue);
            }
            // Handle finger events.
            return this->handleFingerEvent(e, queue);
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

    // Application+HTTPClient Start
    public:
        network::HTTPClient *httpClient;
    private:
        void setupHTTPClient()
        {
            this->httpClient = new network::HTTPClient;
        }
        void tearHTTPClientDown()
        {
            delete this->httpClient;
        }
    // Application+HTTPClient End
    // Application+HTTPClientProcessor Start
    public:
        network::HTTPClientProcessor *httpClientProcessor;
    private:
        const std::string httpClientProcessorCallbackName = "HTTPClientProcessor";
    
        void setupHTTPClientProcessor()
        {
            this->httpClientProcessor = new network::HTTPClientProcessor(this->httpClient);
            // Subscribe processor to be processed each frame.
            this->frameReporter.addCallback(
                [&] {
                    this->httpClientProcessor->process();
                },
                this->httpClientProcessorCallbackName
            );
        }
        void tearHTTPClientProcessorDown()
        {
            this->frameReporter.removeCallback(this->httpClientProcessorCallbackName);
            delete this->httpClientProcessor;
        }
    // Application+HTTPClientProcessor End
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
const auto EXAMPLE_TITLE = "OMC-03: Match tiles";
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
        // Example+Theme Start
        this->setupTheme();
        
        // Example+Theme End
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
        // Example+Theme Start
        this->tearThemeDown();
        
        // Example+Theme End
// Example Start
        delete this->app;
    }

// Example End
    // Example+RemoteLayoutTheme Start
    private:
        void setupRemoteLayoutTheme(const Parameters &parameters)
        {
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
            auto success = [&](std::string response) {
                this->parseLayoutResponse(response, url);
            };
            auto failure = [&](std::string reason) {
                MAIN_EXAMPLE_LOG(
                    "ERROR Could not load layout: %s",
                    reason.c_str()
                );
            };
            MAIN_EXAMPLE_LOG("Loading layout from '%s'", url.c_str());
            this->app->httpClient->get(url, success, failure);
        }
        void parseLayoutResponse(
            const std::string &response,
            const std::string &url
        ) {
            mahjong::Layout layout;
            std::istringstream in(response);
            if (mahjong::parseLayout(in, layout))
            {
                auto tileScene = this->createTiles(layout.positions);
                // Apply normal state material to the whole scene.
                tileScene->setStateSet(this->themeMaterial);
                // Rotate the tile scene to have a better view.
                scene::setSimpleRotation(tileScene, {60, 0, 0});
                // Add tile scene to the scene.
                this->scene->addChild(tileScene);
                // Reset the scene.
                this->app->setScene(this->scene);
                MAIN_EXAMPLE_LOG("Successfully loaded layout");
            }
            else
            {
                MAIN_EXAMPLE_LOG("ERROR Could not parse loaded layout");
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
                MAIN_EXAMPLE_LOG(
                    "ERROR Could not load theme: %s",
                    reason.c_str()
                );
            };
            MAIN_EXAMPLE_LOG("Loading theme from '%s'", url.c_str());
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
            this->themeMaterial->setTextureAttributeAndModes(0, texture);
            this->themeMaterialSelected->setTextureAttributeAndModes(0, texture);
            MAIN_EXAMPLE_LOG("Successfully loaded theme");
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
    // Example+Theme Start
    private:
        render::TileTheme *theme;
        // Theme texture configuration.
        const osg::Vec2 textureSize = {1024, 2048};
        const osg::Vec2 tileFaceSize = {160, 240};
        // Theme model.
        const render::TileTheme::Indices faceIndices = {15, 23, 16, 17};
        osg::ref_ptr<osg::Geode> tileModel;
        // Materials for normal and selected states of tiles.
        osg::ref_ptr<osg::StateSet> themeMaterial;
        osg::ref_ptr<osg::StateSet> themeMaterialSelected;
    
        void setupTheme()
        {
            this->theme =
                new render::TileTheme(
                    this->textureSize,
                    this->tileFaceSize,
                    this->faceIndices
                );
            this->setupTileModel();
            this->setupMaterials();
        }
        void tearThemeDown()
        {
            delete this->theme;
        }
        void setupTileModel()
        {
            resource::Resource tileResource(
                "models",
                "tile-low.osgt",
                tile_low_osgt,
                tile_low_osgt_len
            );
            auto node = resource::node(tileResource);
            auto model = reinterpret_cast<osg::Geode *>(node.get());
            // Make sure model is valid.
            if (!model)
            {
                MAIN_EXAMPLE_LOG(
                    "ERROR Could not setup tile model '%s/%s'",
                    tileResource.group.c_str(),
                    tileResource.name.c_str()
                );
                return;
            }
            
            this->tileModel = model;
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
    
            // Create shader program.
            auto prog =
                render::createShaderProgram(
                    resource::string(shaderVert),
                    resource::string(shaderFrag)
                );
    
            // Create normal state material.
            auto normal = new osg::StateSet;
            normal->setAttribute(prog);
            normal->addUniform(new osg::Uniform("image", 0));
            normal->addUniform(new osg::Uniform("isSelected", false));
            this->themeMaterial = normal;
    
            // Create selected state material.
            auto selected = new osg::StateSet;
            selected->setAttribute(prog);
            selected->addUniform(new osg::Uniform("image", 0));
            selected->addUniform(new osg::Uniform("isSelected", true));
            this->themeMaterialSelected = selected;
        }
    // Example+Theme End
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
 
    // Example+createTiles Start
    // Make sure positions' count is equal to matchIds' one.
    osg::MatrixTransform* createTiles(
        const mahjong::Layout::Positions &positions,
        mahjong::MatchIds &matchIds
    ) {
        // Create scene to host tile nodes.
        osg::ref_ptr<osg::MatrixTransform> tileScene = new osg::MatrixTransform;
        // Correct the height for `tile-low.osgt` model.
        const float MODEL_HEIGHT_FACTOR = 1.5;
    
        int id = 0;
        // Generate tile nodes.
        for (auto pos : positions)
        {
            // Clone tile.
            auto tile = new osg::Geode(*this->tileModel, osg::CopyOp::DEEP_COPY_ALL);
            // Create node for it.
            auto node = new osg::MatrixTransform;
            node->addChild(tile);
            // Set node's position.
            float x = pos.column;
            float y = pos.row * MODEL_HEIGHT_FACTOR;
            float z = pos.field;
            scene::setSimplePosition(node, {x, y, z});
            // Set match id.
            int matchId = matchIds[id++];
            this->theme->setFaceId(matchId, tile);
    
            // Add tile node to the scene.
            tileScene->addChild(node);
        }
    
        return tileScene.release();
    }
    // Example+createTiles End
// Example Start
};
// Example End

} // namespace main
} // namespace omc

#endif // OGS_MAHJONG_COMPONENTS_MAIN_H

