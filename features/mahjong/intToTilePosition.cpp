FEATURE mahjong.h/Impl
TilePosition intToTilePosition(int value)
{
    int field = value / 1000000;
    int row = (value - field * 1000000) / 1000;
    int column = value - field * 1000000 - row * 1000;
    return TilePosition(field, row, column);
}
