
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
        fieldId(0),
        row(0),
        column(0)
    { }

    TilePosition(int fieldId, int row, int column) :
        fieldId(fieldId),
        row(row),
        column(column)
    { }

    int fieldId;
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
// Solitaire Start
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
// Solitaire End

// intToTilePosition Start
TilePosition intToTilePosition(int value)
{
    int fieldId = value / 1000000;
    int row = (value - fieldId * 1000000) / 1000;
    int column = value - fieldId * 1000000 - row * 1000;
    return TilePosition(fieldId, row, column);
}
// intToTilePosition End
// tilePositionToInt Start
int tilePositionToInt(const TilePosition &position)
{
    return position.fieldId * 1000000 + position.row * 1000 + position.column;
}
// tilePositionToInt End

} // namespace mahjong
} // namespace mc

#endif // MAHJONG_COMPONENTS_MAHJONG_H

