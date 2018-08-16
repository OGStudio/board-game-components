FEATURE mahjong.h/Impl
class Solitaire
{
    public:
        Solitaire() { }

        void setTiles(const std::vector<Tile> &tiles)
        {
            // Convert list of tiles to map for faster lookup.
            int id = 0;
            for (auto tile : tiles)
            {
                this->tiles[id++] = tile;
            }
        }

        bool isTileSelectable(int id) const
        {

        }

        bool tilesMatch(int id1, int id2) const
        {

        }

        void removeTiles(int id1, int id2)
        {

        }

    private:
        // Id -> Tile.
        std::map<int, Tile> tiles;
};
