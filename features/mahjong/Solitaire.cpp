FEATURE mahjong.h/Impl
class Solitaire
{
    public:
        Solitaire() { }

        void setTiles(const std::vector<Tile> &tiles)
        {
            int id = 0;
            for (auto tile : tiles)
            {
                // Keep tiles in `id->tile` map for lookup by id.
                this->idTiles[id++] = tile;

                // Keep tiles in `position->tile` map for lookup by position.
                int position = tilePositionToInt(tile.position);
                this->positionTiles[position] = tile;
            }
        }

        bool isTileSelectable(int id)
        {
            // Make sure tile is valid.
            auto tile = this->tile(id);
            if (!tile)
            {
                return false;
            }

            // See if tile is blocked at the left and the right.
            bool left = this->tileHasNeighbours(tile->position, 0, -2);
            bool right = this->tileHasNeighbours(tile->position, 0, 2);
            if (left && right)
            {
                return false;
            }

            // See if tile is blocked at the top.
            for (int columnOffset = -1; columnOffset <= 1; ++columnOffset)
            {
                if (this->tileHasNeighbours(tile->position, 1, columnOffset))
                {
                    return false;
                }
            }

            // Tile is not blocked.
            return true;
        }

        bool tilesMatch(int id1, int id2)
        {
            // Make sure tiles are valid.
            Tile *tile1 = this->tile(id1);
            Tile *tile2 = this->tile(id2);
            if (!tile1 || !tile2)
            {
                return false;
            }

            // Match tiles.
            return (tile1->matchId == tile2->matchId);
        }

        void removeTiles(int id1, int id2)
        {
            this->removeTile(id1);
            this->removeTile(id2);
        }

    private:
        // Id -> Tile.
        std::map<int, Tile> idTiles;
        // Position -> Tile.
        std::map<int, Tile> positionTiles;

        void removeTile(int id)
        {
            // Make sure tile exists.
            auto it = this->idTiles.find(id);
            if (it == this->idTiles.end())
            {
                return;
            }
            auto tile = it->second;

            // Erase tile from idTiles.
            this->idTiles.erase(it);

            // Erase tile from positionTiles.
            auto position = tilePositionToInt(tile.position);
            auto positionIt = this->positionTiles.find(position);
            this->positionTiles.erase(positionIt);
        }

        Tile *tile(int id)
        {
            auto it = this->idTiles.find(id);
            if (it != this->idTiles.end())
            {
                return &it->second;
            }
            return 0;
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
