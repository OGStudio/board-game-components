FEATURE main.h/Setup
this->setupTileSelectionDepiction();

FEATURE main.h/Impl
private:
    std::vector<NodeTile> depictedTiles;

    void setupTileSelectionDepiction()
    {
        this->selectedTilesChanged.addCallback(
            [&] {
                this->depictSelectedTiles();
            }
        );
    }
    void depictSelectedTiles()
    {
        // Remove depiction of previously selected tiles.
        for (auto nodeTile : this->depictedTiles)
        {
            auto node = nodeTile.node;
            this->depictNodeSelection(node, false);
        }

        // Depict currently selected tiles.
        for (auto nodeTile : this->selectedTiles)
        {
            auto node = nodeTile.node;
            this->depictNodeSelection(node, true);
        }

        // Keep depicted tiles.
        this->depictedTiles = this->selectedTiles;
    }
    void depictNodeSelection(osg::Node *node, bool isSelected)
    {
        osg::StateSet *material = 
            isSelected ? 
            this->themeMaterialSelected :
            0;
        node->setStateSet(material);
    }
