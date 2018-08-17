
/*
This file is part of Mahjong components:
  https://github.com/OGStudio/mahjong-components

Copyright (C) 2018 Opensource Game Studio

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef MAHJONG_COMPONENTS_MAHJONG_H
#define MAHJONG_COMPONENTS_MAHJONG_H

namespace mc
{
namespace mahjong
{

// TilePosition Start
struct TilePosition
{
    TilePosition() :
        field(0),
        row(0),
        column(0)
    { }

    TilePosition(int field, int row, int column) :
        field(field),
        row(row),
        column(column)
    { }

    int field;
    int row;
    int column;
};
// TilePosition End
// Tile Start
struct Tile
{
    Tile() : matchId(0) { }

    // NOTE Try not to use id since it's an external representation.
    //int id;
    int matchId;
    TilePosition position;
};
// Tile End

// intToTilePosition Start
TilePosition intToTilePosition(int value)
{
    int field = value / 1000000;
    int row = (value - field * 1000000) / 1000;
    int column = value - field * 1000000 - row * 1000;
    return TilePosition(field, row, column);
}
// intToTilePosition End
// tilePositionToInt Start
int tilePositionToInt(const TilePosition &position)
{
    return position.field * 1000000 + position.row * 1000 + position.column;
}
// tilePositionToInt End

// Solitaire Start
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

        }

    private:
        // Id -> Tile.
        std::map<int, Tile> idTiles;
        // Position -> Tile.
        std::map<int, Tile> positionTiles;

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
// Solitaire End

} // namespace mahjong
} // namespace mc

#endif // MAHJONG_COMPONENTS_MAHJONG_H

