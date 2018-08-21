FEATURE mahjong.h/Impl
bool operator==(const TilePosition &pos1, const TilePosition &pos2)
{
    return
        (pos1.field == pos2.field) &&
        (pos1.row == pos2.row) &&
        (pos1.column == pos2.column)
        ;
}
