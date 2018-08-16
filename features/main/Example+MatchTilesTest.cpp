FEATURE main.h/Include
#include "mahjong.h"

FEATURE main.h/Setup
this->setupMatchTilesTest();

FEATURE main.h/TearDown
this->tearMatchTilesTestDown();

FEATURE main.h/Impl
private:
    mahjong::Solitaire *game;
    void setupMatchTilesTest()
    {
        this->game = new mahjong::Solitaire();
    }
    void tearMatchTilesTestDown()
    {
        delete this->game;
    }
