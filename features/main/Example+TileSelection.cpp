FEATURE main.h/Impl
private:
    struct NodeTile
    {
        osg::Node *node;
        mahjong::Tile tile;
    };
    std::vector<NodeTile> selectedTiles;
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

        // Make sure the tile has not yet been selected.
        for (auto nodeTile : this->selectedTiles)
        {
            if (nodeTile.node == node)
            {
                return;
            }
        }

        // Select and report.
        this->selectedTiles.push_back({node, tile});
        this->selectedTilesChanged.report();
    }
