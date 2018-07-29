FEATURE layout.h/Include
#include "format.h"

FEATURE layout.h/Impl
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
            format::printfString("p(%d, %d, %d)\n", pos.x(), pos.y(), pos.z());
    }

    return
        header +
        positions +
        "]\n)"
        ;
}
