FEATURE main.h/Include
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

FEATURE main.h/Setup
this->testLayout();

FEATURE main.h/Impl
private:
    void testLayout()
    {
        resource::Resource res(
            "layouts",
            "cat.layout",
            cat_layout,
            cat_layout_len
        );
        resource::ResourceStreamBuffer buf(res);
        std::istream in(&buf);
        layout::Layout layout;
        if (layout::parseLayout(in, layout))
        {
            MC_MAIN_EXAMPLE_LOG("Parsed layout:");
            auto msg = layout::layoutToString(layout);
            log::log(msg.c_str());
        }
        else
        {
            MC_MAIN_EXAMPLE_LOG("Failed to parse cat layout");
        }

        /*
        resource::Resource layout2(
            "layouts",
            "X_shaped.layout",
            X_shaped_layout,
            X_shaped_layout_len
        );
        */

        auto node = scene::createSphere(1);

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
        auto material = scene->getOrCreateStateSet();
        material->setAttribute(prog);
        // Set color.
        osg::Vec3 color(0.6, 0.4, 0.4);
        material->addUniform(new osg::Uniform("color", color));

        this->scene->addChild(node);
    }
