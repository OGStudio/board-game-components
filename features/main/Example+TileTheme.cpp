FEATURE main.h/Include
#include "render.h"

FEATURE main.h/Setup
this->setupTileTheme();

FEATURE main.h/TearDown
this->tearTileThemeDown();

FEATURE main.h/Impl
private:
    render::TileTheme *tileTheme;
    const osg::Vec2 textureSize = {1024, 2048};
    const osg::Vec2 tileFaceSize = {200, 300};

    void setupTileTheme()
    {
        // TODO Specify start/end indices based on model. Or just indices.
        this->tileTheme = new render::TileTheme(textureSize, tileFaceSize);
    }
    void tearTileThemeDown()
    {
        delete this->tileTheme;
    }
