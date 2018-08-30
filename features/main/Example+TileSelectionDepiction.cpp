FEATURE main.h/Setup
this->setupTileSelectionDepiction();

FEATURE main.h/Impl
private:
    std::map<osg::Node *, mahjong::Tile> depictedTiles;

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
        for (auto it : this->depictedTiles)
        {
            auto node = it.first;
            this->depictNodeSelection(node, false);
        }

        // Depict currently selected tiles.
        for (auto it : this->selectedTiles)
        {
            auto node = it.first;
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
