FEATURE mahjong.h/Impl
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
