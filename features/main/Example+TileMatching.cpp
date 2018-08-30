FEATURE main.h/Impl
private:
    bool tilesMatch = false;
    core::Reporter tilesMatchChanged;

    void setupTileMatching()
    {
        this->selectedTilesChanged.addCallback(
            [&] {
                // Only perform matching once two tiles have been selected.
                if (this->selectedTiles.size() == 2)
                {
                    this->matchTiles();
                }
            }
        );
    }
    void matchTiles()
    {
        auto nodeTile = this->selectedTiles.begin();
        auto tile1 = nodeTile->tile;
        ++nodeTile;
        auto tile2 = nodeTile->tile;

        this->tilesMatch = this->game->tilesMatch(tile1, tile2);
        this->tilesMatchChanged.report();
    }
