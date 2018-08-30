FEATURE main.h/Setup
this->setupMatchTilesTest();

FEATURE main.h/Impl
private:
    //osg::Node *selectedTileNode = 0;
    osg::Node *previouslySelectedTileNode = 0;
    core::Reporter tilesMatch;
    std::vector<osg::Node *> nodesToRemove;
    core::Reporter removedTiles;

    void setupMatchTilesTest()
    {
        this->setupTileMatching();
        this->setupTileRemoval();
        this->setupGameStateDetection();
    }
    void setupTileMatching()
    {
        this->selectedTile.addCallback(
            [&] {
                auto nodeNow = this->selectedTileNode;
                auto tileNow = this->tileNodes[nodeNow];

                // Make sure tile is selectable.
                if (!this->game->isTileSelectable(tileNow))
                {
                    return;
                }

                // Depict selection.
                this->setNodeSelected(nodeNow, true);

                auto nodeWas = this->previouslySelectedTileNode;

                // Keep previously selected tile node.
                this->previouslySelectedTileNode = nodeNow;

                // Make sure different nodes were selected.
                if (!nodeWas || (nodeNow == nodeWas))
                {
                    return;
                }

                // Remove selection of previously selected node.
                this->setNodeSelected(nodeWas, false);

                // Match tiles.
                auto tileWas = this->tileNodes[nodeWas];
                bool match = this->game->tilesMatch(tileWas, tileNow);
                // Report matching.
                if (match)
                {
                    this->nodesToRemove.push_back(nodeWas);
                    this->nodesToRemove.push_back(nodeNow);
                    this->tilesMatch.report();
                }
            }
        );
    }
    void setNodeSelected(osg::Node *node, bool state)
    {
        osg::StateSet *material = 
            state ? 
            this->themeMaterialSelected :
            0;
        node->setStateSet(material);
    }
    void setupTileRemoval()
    {
        this->tilesMatch.addCallback(
            [&] {
                this->removeTiles();
            }
        );
    }
    void removeTiles()
    {
        // Find out nodes and tiles to remove.
        auto nodeWas = this->nodesToRemove[0];
        auto nodeNow = this->nodesToRemove[1];
        this->nodesToRemove.clear();
        auto tileWas = this->tileNodes[nodeWas];
        auto tileNow = this->tileNodes[nodeNow];

        // Remove tiles from Solitaire (logical) representation.
        this->game->removeTiles(tileWas, tileNow);
        // Remove nodes from tileNodes.
        {
            auto it = this->tileNodes.find(nodeWas);
            this->tileNodes.erase(it);
        }
        {
            auto it = this->tileNodes.find(nodeNow);
            this->tileNodes.erase(it);
        }
        // Deselect both tile nodes.
        this->setNodeSelected(nodeWas, false);
        this->setNodeSelected(nodeNow, false);
        // Remove nodes from tileScene.
        this->tileScene->removeChild(nodeWas);
        this->tileScene->removeChild(nodeNow);
        // Remove selection.
        this->selectedTileNode = 0;
        this->previouslySelectedTileNode = 0;
        
        // Report.
        this->removedTiles.report();
    }
    void setupGameStateDetection()
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
            if (this->game->hasTiles())
            {
                OMC_MAIN_EXAMPLE_LOG("Game over: LOSS");
            }
            else
            {
                OMC_MAIN_EXAMPLE_LOG("Game over: VICTORY");
            }
        }
    }
