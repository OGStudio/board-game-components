
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
    // Application+run Start
    public:
        void run()
        {
            while (!this->viewer->done())
            {
                this->frame();
            }
        }
    // Application+run End
    // Application+setupWindow-desktop Start
    public:
        void setupWindow(
            const std::string &title,
            int x,
            int y,
            int width,
            int height
        ) {
            osg::GraphicsContext *gc =
                render::createGraphicsContext(title, x, y, width, height);
            // Configure viewer's camera with FOVY and window size.
            osg::Camera *cam = this->viewer->getCamera();
            render::setupCamera(cam, gc, 30, width, height);
        }
    // Application+setupWindow-desktop End

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
        // Example+RemoteLayoutTheme Start
        this->setupRemoteLayoutTheme(parameters);
        
        // Example+RemoteLayoutTheme End
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
    
        void createTiles(const layout::Layout &layout)
        {
            for (auto pos : layout.positions)
            {
                float z = pos.x();
                float y = pos.y() * 1.5 /* Factor depends on the model */;
                float x = pos.z();
                auto tile = new osg::Geode(*this->tile, osg::CopyOp::DEEP_COPY_ALL);
                auto node = new osg::MatrixTransform;
                node->addChild(tile);
                this->tileScene->addChild(node);
                scene::setSimplePosition(node, {x, y, z});
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
// Example Start
};
// Example End

} // namespace main
} // namespace mc

#endif // MAHJONG_COMPONENTS_MAIN_H

