FEATURE mahjong.h/Include
#include <iostream>

FEATURE mahjong.h/Impl
bool parseLayout(std::istream &in, Layout &layout)
{
    // Collect lines.
    std::vector<std::string> lines;
    std::string ln;
    while (std::getline(in, ln))
    {
        lines.push_back(ln);
    }

    // Parse them.
    KMahjonggLayout kmlayout;
    auto success = linesToKMahjonggLayout(lines, kmlayout);

    // Provide layout if parsing was successful.
    if (success)
    {
        layout = kmlayout;
    }
    
    return success;
}
