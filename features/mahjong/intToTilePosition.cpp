FEATURE mahjong.h/Impl
TilePosition intToTilePosition(int value)
{
    int fieldId = value / 1000000;
    int row = (value - fieldId * 1000000) / 1000;
    int column = value - fieldId * 1000000 - row * 1000;
    return TilePosition(fieldId, row, column);
}
