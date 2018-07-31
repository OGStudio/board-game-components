
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
// Example+TileTheme Start
#include "ppl.frag.h"
#include "ppl.vert.h"
#include "tile-low.osgt.h"
#include "tile-theme.png.h"
#include "resource.h"

#include <osg/MatrixTransform>

// Example+TileTheme End

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

// Example+02 Start
const auto EXAMPLE_TITLE = "Mc02";
// Example+02 End

// Example Start
struct Example
{
    Application *app;

    Example()
    {
        this->app = new Application(EXAMPLE_TITLE);

// Example End
    // Example+Scene Start
    this->setupScene();
    
    // Example+Scene End
    // Example+TileTheme Start
    this->testTileTheme();
    
    // Example+TileTheme End
// Example Start
    }
    ~Example()
    {

// Example End
// Example Start
        delete this->app;
    }

// Example End
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
            // TODO Extract some entity to manage these specs.
            // NOTE These values are only valid for 1024x2048 texture with
            // NOTE 200x300 tile items.
            const float tileWidth = 0.19531;
            const float tileHeight = 0.14648;
    
            const float id = static_cast<float>(tileId);
    
            const osg::Vec2 topRight = {
                tileWidth * (id + 1),
                1.0f - tileHeight * id
            };
            const osg::Vec2 topLeft = {
                tileWidth * id,
                1.0f - tileHeight * id
            };
            const osg::Vec2 bottomLeft = {
                tileWidth * id,
                1.0f - tileHeight * (id + 1)
            };
            const osg::Vec2 bottomRight = {
                tileWidth * (id + 1),
                1.0f - tileHeight * (id + 1)
            };
    
            auto texCoordsCount = geom->getNumTexCoordArrays();
            if (!texCoordsCount)
            {
                MC_MAIN_EXAMPLE_LOG("ERROR Model has no texture coordinates");
                return;
            }
    
            // Take the first array.
            auto coords = geom->getTexCoordArray(0);
            auto coordsCount = coords->getNumElements();
            MC_MAIN_EXAMPLE_LOG("Coords count: '%d'", coordsCount);
            // Define start/end points of coords to change.
            auto coordsIdStart = coordsCount - 4;
            auto coordsIdEnd = coordsCount;
            for (int i = coordsIdStart; i < coordsIdEnd; ++i)
            {
                auto coord = reinterpret_cast<const osg::Vec2 *>(coords->getDataPointer(i));
                MC_MAIN_EXAMPLE_LOG("id: '%d' coord: '%f x %f'", i, coord->x(), coord->y());
            }
    
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
    // Example+TileTheme End
// Example Start
};
// Example End

} // namespace main
} // namespace mc

#endif // MAHJONG_COMPONENTS_MAIN_H

