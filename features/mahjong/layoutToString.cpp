FEATURE mahjong.h/Include
#include "format.h"

FEATURE mahjong.h/Impl
std::string layoutToString(const Layout &layout)
{
    std::string header =
        format::printfString(
            "Layout(width: '%d' height: '%d' depth: '%d' positions: [\n",
            layout.width,
            layout.height,
            layout.depth
        );
    std::string positions;
    for (auto pos : layout.positions)
    {
        positions +=
            format::printfString("p(%d, %d, %d)\n", pos.field, pos.row, pos.column);
    }

    return
        header +
        positions +
        "]\n)"
        ;
}
