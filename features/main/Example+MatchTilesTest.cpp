FEATURE main.h/Setup
this->setupMatchTilesTest();

FEATURE main.h/Impl
private:
    //osg::Node *selectedTileNode = 0;
    osg::Node *previouslySelectedTileNode = 0;
    core::Reporter tilesMatch;
    std::vector<osg::Node *> nodesToRemove;
    core::Reporter removedTiles;

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
