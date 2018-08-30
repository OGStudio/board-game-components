FEATURE main.h/Setup
this->setupUnmatchedTilesDeselection();

FEATURE main.h/Impl
private:
    void setupUnmatchedTilesDeselection()
    {
        this->tilesMatchChanged.addCallback(
            [&] {
                // Make sure there was no match.
                if (this->tilesMatch)
                {
                    return;
                }

                this->deselectUnmatchedTiles();
            }
        );
    }
    void deselectUnmatchedTiles()
    {
        // Deselect the first unmatched tile only.
        this->selectedTiles.erase(this->selectedTiles.begin());
        this->selectedTilesChanged.report();
    }
