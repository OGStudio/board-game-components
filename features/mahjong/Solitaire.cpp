FEATURE mahjong.h/Impl
class Solitaire
{
    public:
        Solitaire() { }

        typedef std::vector<Tile> Tiles;

        bool hasTiles()
        {
            return !this->positionTiles.empty();
        }

        bool hasTurns()
        {
            auto tiles = this->selectableTiles();
            for (auto tile1 : tiles)
            {
                for (auto tile2 : tiles)
                {
                    // Ignore the same item.
                    if (tile1.position == tile2.position)
                    {
                        continue;
                    }

                    // We have turns if there is at least one matching pair
                    // of selectable tiles.
                    if (tile1.matchId == tile2.matchId)
                    {
                        return true;
                    }
                }
            }
            return false;
        }

        bool isTileSelectable(const Tile &tile)
        {
            // See if tile is blocked at the left and the right.
            bool left = this->tileHasNeighbours(tile.position, 0, -2);
            bool right = this->tileHasNeighbours(tile.position, 0, 2);
            if (left && right)
            {
                return false;
            }

            // See if tile is blocked at the top.
            for (int columnOffset = -1; columnOffset <= 1; ++columnOffset)
            {
                if (this->tileHasNeighbours(tile.position, 1, columnOffset))
                {
                    return false;
                }
            }

            // Tile is not blocked.
            return true;
        }

        void removeTiles(const Tile &tile1, const Tile &tile2)
        {
            this->removeTile(tile1);
            this->removeTile(tile2);
        }

        void setTiles(const Tiles &tiles)
        {
            for (auto tile : tiles)
            {
                // Keep tiles in `position->tile` map for lookup by position.
                int position = tilePositionToInt(tile.position);
                this->positionTiles[position] = tile;
            }
        }

        bool tilesMatch(const Tile &tile1, const Tile &tile2)
        {
            return (tile1.matchId == tile2.matchId);
        }

    private:
        // Position -> Tile.
        std::map<int, Tile> positionTiles;

        void removeTile(const Tile &tile)
        {
            // Erase tile from positionTiles.
            auto position = tilePositionToInt(tile.position);
            auto it = this->positionTiles.find(position);
            this->positionTiles.erase(it);
        }

        Tiles selectableTiles()
        {
            Tiles tiles;
            for (auto it : this->positionTiles)
            {
                auto tile = it.second;
                if (this->isTileSelectable(tile))
                {
                    tiles.push_back(tile);
                }
            }
            return tiles;
        }

        bool tileHasNeighbours(
            const TilePosition &tilePosition,
            int fieldOffset,
            int columnOffset
        ) {
            for (int rowOffset = -1; rowOffset <= 1; ++rowOffset)
            {
                // Construct neighbour position.
                auto neighbourPosition =
                    TilePosition(
                        tilePosition.field + fieldOffset,
                        tilePosition.row + rowOffset,
                        tilePosition.column + columnOffset
                    );
                auto position = tilePositionToInt(neighbourPosition);
                // See if it's occupied by the neighbour.
                auto it = this->positionTiles.find(position);
                if (it != this->positionTiles.end())
                {
                    return true;
                }
            }

            // Found no neighbours.
            return false;
        }

};
