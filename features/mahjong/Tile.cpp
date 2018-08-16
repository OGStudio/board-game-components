FEATURE mahjong.h/Impl
struct Tile
{
    Tile() : matchId(0) { }

    // NOTE Try not to use id since it's an external representation.
    //int id;
    int matchId;
    TilePosition position;
};
