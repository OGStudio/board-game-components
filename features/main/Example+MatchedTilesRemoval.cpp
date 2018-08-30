FEATURE main.h/Setup
this->setupMatchedTilesRemoval();

FEATURE main.h/Impl
private:
    core::Reporter removedTiles;

    void setupMatchedTilesRemoval()
    {
        this->tilesMatchChanged.addCallback(
            [&] {
                // Make sure there was a match.
                if (!this->tilesMatch)
                {
                    return;
                }

                this->removeMatchedTiles();
            }
        );
    }
    void removeMatchedTiles()
    {
        auto nodeTile1 = this->selectedTiles[0];
        auto nodeTile2 = this->selectedTiles[1];

        // Deselect tiles.
        this->selectedTiles.clear();
        this->selectedTilesChanged.report();

        // Remove tiles from Solitaire (logical representation).
        this->game->removeTiles(nodeTile1.tile, nodeTile2.tile);

        // Remove items from visual-to-logical correspondence.
        {
            auto it = this->tileNodes.find(nodeTile1.node);
            this->tileNodes.erase(it);
        }
        {
            auto it = this->tileNodes.find(nodeTile2.node);
            this->tileNodes.erase(it);
        }

        // Remove nodes from the scene.
        this->tileScene->removeChild(nodeTile1.node);
        this->tileScene->removeChild(nodeTile2.node);
        
        // Report.
        this->removedTiles.report();
    }
