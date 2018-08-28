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
