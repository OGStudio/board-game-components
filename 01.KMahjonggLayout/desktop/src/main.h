
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

// Example+Layout Start
#include "layout.h"
#include "log.h"
#include "scene.h"

#include "resource.h"
// Layouts.
#include "cat.layout.h"
#include "X_shaped.layout.h"
// Shaders.
#include "ppl-color.vert.h"
#include "ppl-color.frag.h"

// Example+Layout End
// Example+Scene Start
#include "scene.h"
#include <osg/MatrixTransform>

// Example+Scene End

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

// Example+01 Start
const auto EXAMPLE_TITLE = "Mc01";
// Example+01 End

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
    // Example+Layout Start
    this->testLayout();
    
    // Example+Layout End
// Example Start
    }
    ~Example()
    {

// Example End
// Example Start
        delete this->app;
    }

// Example End
    // Example+Layout Start
    private:
        osg::ref_ptr<osg::MatrixTransform> layoutScene;
        void testLayout()
        {
            // NOTE Test X_shaped.layout?
            resource::Resource cat(
                "layouts",
                "cat.layout",
                cat_layout,
                cat_layout_len
            );
            layout::Layout layout;
            if (!this->loadLayout(cat, layout))
            {
                MC_MAIN_EXAMPLE_LOG("Could not load layout");
                return;
            }
            osg::Vec3 color(0.7, 0.5, 0.3);
            this->setupLayoutScene(color);
            this->createSpheres(layout);
        }
        void createSpheres(const layout::Layout &layout)
        {
            for (auto pos : layout.positions)
            {
                float z = pos.x();
                float y = pos.y();
                float x = pos.z();
                auto node = scene::createSphere(1);
                this->layoutScene->addChild(node);
                scene::setSimplePosition(node, {x, y, z});
            }
        }
        bool loadLayout(
            const resource::Resource &layoutResource,
            layout::Layout &layout
        ) {
            resource::ResourceStreamBuffer buf(layoutResource);
            std::istream in(&buf);
            return layout::parseLayout(in, layout);
        }
        void setupLayoutScene(const osg::Vec3 &color)
        {
            this->layoutScene = new osg::MatrixTransform;
            // Rotate layout sceen for better depiction.
            scene::setSimpleRotation(this->layoutScene, {45, 0, 0});
            this->scene->addChild(this->layoutScene);
    
            // Create shader program.
            resource::Resource shaderVert(
                "shaders",
                "ppl-color.vert",
                ppl_color_vert,
                ppl_color_vert_len
            );
            resource::Resource shaderFrag(
                "shaders",
                "ppl-color.frag",
                ppl_color_frag,
                ppl_color_frag_len
            );
            auto prog =
                render::createShaderProgram(
                    resource::string(shaderVert),
                    resource::string(shaderFrag)
                );
            // Apply the program.
            auto material = this->layoutScene->getOrCreateStateSet();
            material->setAttribute(prog);
            // Set color.
            material->addUniform(new osg::Uniform("color", color));
        }
    // Example+Layout End
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
// Example Start
};
// Example End

} // namespace main
} // namespace mc

#endif // MAHJONG_COMPONENTS_MAIN_H

