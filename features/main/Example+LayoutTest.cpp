FEATURE main.h/Include
#include "log.h"
#include "mahjong.h"
#include "scene.h"

#include "resource.h"
// Layouts.
#include "cat.layout.h"
#include "X_shaped.layout.h"
// Shaders.
#include "ppl-color.vert.h"
#include "ppl-color.frag.h"

FEATURE main.h/Setup
this->testLayout();

FEATURE main.h/Impl
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
        mahjong::Layout layout;
        if (!this->loadLayout(cat, layout))
        {
            MAIN_EXAMPLE_LOG("Could not load layout");
            return;
        }
        osg::Vec3 color(0.7, 0.5, 0.3);
        this->setupLayoutScene(color);
        this->createSpheres(layout);
    }
    void createSpheres(const mahjong::Layout &layout)
    {
        for (auto pos : layout.positions)
        {
            float x = pos.column;
            float y = pos.row;
            float z = pos.field;
            auto node = scene::createSphere(1);
            this->layoutScene->addChild(node);
            scene::setSimplePosition(node, {x, y, z});
        }
    }
    bool loadLayout(
        const resource::Resource &layoutResource,
        mahjong::Layout &layout
    ) {
        resource::ResourceStreamBuffer buf(layoutResource);
        std::istream in(&buf);
        return mahjong::parseLayout(in, layout);
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
