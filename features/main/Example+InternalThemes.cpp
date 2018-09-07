FEATURE main.h/Include
#include "numbers-theme.png.h"

#include "resource.h"

FEATURE main.h/Impl
private:
    resource::Pool *internalThemes;
    void setupInternalThemes()
    {
        // Create pool.
        this->internalThemes = new resource::Pool;

        // Register internal themes.
        {
            resource::Resource res(
                "themes",
                "numbers-theme.png",
                numbers_theme_png,
                numbers_theme_png_len
            );
            this->internalThemes->addResource(res);
        }
    }
    void tearInternalThemesDown()
    {
        delete this->internalThemes;
    }
