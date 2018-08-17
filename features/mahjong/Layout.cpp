FEATURE mahjong.h/Impl
//! Layout representation.
struct Layout
{
    typedef std::vector<TilePosition> Positions;

    int width;
    int height;
    int depth;
    Positions positions;
};
