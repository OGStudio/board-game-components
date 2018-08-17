FEATURE mahjong.h/Impl
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
