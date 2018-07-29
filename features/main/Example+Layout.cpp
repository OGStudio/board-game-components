FEATURE main.h/Include
#include "layout.h"
#include "log.h"
#include "scene.h"

#include "resource.h"
#include "cat.layout.h"
#include "X_shaped.layout.h"

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

        this->scene->addChild(scene::createSphere(1));
    }
