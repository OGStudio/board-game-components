FEATURE main.h/Setup
this->setupTileSelection();

FEATURE main.h/Impl
private:
    std::map<osg::Node *, mahjong::Tile> selectedTiles;
    core::Reporter selectedTilesChanged;
    const std::string tileSelectionCallbackName = "TileSelection";

    void setupTileSelection()
    {
        this->selectedNodeChanged.addCallback(
            [&] {
                this->selectTile();
            },
            this->tileSelectionCallbackName
        );
    }
    void tearTileSelectionDown()
    {
        this->selectedNodeChanged.removeCallback(
            this->tileSelectionCallbackName
        );
    }
    void selectTile()
    {
        auto node = this->selectedNode;
        auto tile = this->tileNodes[node];

        // Make sure tile is selectable.
        if (!this->game->isTileSelectable(tile))
        {
            return;
        }

        this->selectedTiles[node] = tile;
        this->selectedTilesChanged.report();
    }
