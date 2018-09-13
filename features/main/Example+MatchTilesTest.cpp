FEATURE main.h/Setup
this->setupMatchTilesTest();

FEATURE main.h/TearDown
this->tearMatchTilesTestDown();

FEATURE main.h/Impl
private:
    void setupMatchTilesTest()
    {
        this->setupInternalLayouts();
        this->setupInternalThemes();
        this->setupDefaultLayoutTheme();
        this->setupTiles(time(0));
        this->setupNodeSelection();
        this->setupTileSelection();
        this->setupTileSelectionDepiction();
        this->setupTileMatching();
        this->setupUnmatchedTilesDeselection();
        this->setupMatchedTilesRemoval();
        this->setupGameState();

        // Report game result.
        this->finishedGame.addCallback(
            [&] {
                if (this->isGameVictorious)
                {
                    MAIN_EXAMPLE_LOG("Game over: VICTORY");
                }
                else
                {
                    MAIN_EXAMPLE_LOG("Game over: LOSS");
                }
            }
        );
    }
    void tearMatchTilesTestDown()
    {
        this->tearInternalLayoutsDown();
    }
