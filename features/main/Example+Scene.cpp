FEATURE main.h/Include
#include "scene.h"
#include <osg/MatrixTransform>

FEATURE main.h/Setup
this->setupScene();

FEATURE main.h/Impl
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
