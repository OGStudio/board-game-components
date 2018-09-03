FEATURE main.h/Setup
this->setupMatchTilesTest();

FEATURE main.h/Impl
private:
    void setupMatchTilesTest()
    {
        this->setupDefaultLayoutTheme();
        this->setupTiles();
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
    void tearMatchTilesDown()
    {
        this->tearNodeSelectionDown();
    }
