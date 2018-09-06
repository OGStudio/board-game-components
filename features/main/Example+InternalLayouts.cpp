FEATURE main.h/Include
#include "X_shaped.layout.h"
#include "short-loss.layout.h"
#include "short-victory.layout.h"

#include "resource.h"

FEATURE main.h/Impl
private:
    resource::Pool *internalLayouts;
    void setupInternalLayouts()
    {
        // Create pool.
        this->internalLayouts = new resource::Pool;

        // Register internal layouts.
        {
            resource::Resource res(
                "layouts",
                "X_shaped.layout",
                X_shaped_layout,
                X_shaped_layout_len
            );
            this->internalLayouts->addResource(res);
        }
        {
            resource::Resource res(
                "layouts",
                "short-loss.layout",
                short_loss_layout,
                short_loss_layout_len
            );
            this->internalLayouts->addResource(res);
        }
        {
            resource::Resource res(
                "layouts",
                "short-victory.layout",
                short_victory_layout,
                short_victory_layout_len
            );
            this->internalLayouts->addResource(res);
        }
    }
    void tearInternalLayoutsDown()
    {
        delete this->internalLayouts;
    }
