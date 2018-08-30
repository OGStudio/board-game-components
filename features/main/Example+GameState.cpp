FEATURE main.h/Impl
private:
    bool isGameVictorious = false;
    core::Reporter finishedGame;
    void setupGameState()
    {
        this->removedTiles.addCallback(
            [&] {
                this->detectGameState();
            }
        );
    }
    void detectGameState()
    {
        if (!this->game->hasTurns())
        {
            this->isGameVictorious = !this->game->hasTiles();
            this->finishedGame.report();
        }
    }
