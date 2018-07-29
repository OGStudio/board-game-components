
/*
This file is part of Mahjong components:
  https://github.com/OGStudio/mahjong-components

Copyright (C) 2018 Opensource Game Studio

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef MAHJONG_COMPONENTS_LAYOUT_H
#define MAHJONG_COMPONENTS_LAYOUT_H

// Layout Start
#include <osg/Vec3>

// Layout End

// MC_LAYOUT_LOG Start
#include "log.h"
#include "format.h"
#define MC_LAYOUT_LOG_PREFIX "mc::layout %s"
#define MC_LAYOUT_LOG(...) \
    log::logprintf( \
        MC_LAYOUT_LOG_PREFIX, \
        format::printfString(__VA_ARGS__).c_str() \
    )
// MC_LAYOUT_LOG End

namespace mc
{
namespace layout
{

// Layout Start
//! Layout representation.
struct Layout
{
    // Each osg::Vec3 item contains:
    // * x: field id
    // * y: row
    // * z: column
    typedef std::vector<osg::Vec3i> Positions;

    int width;
    int height;
    int depth;
    Positions positions;
};
// Layout End
// KMahjonggLayout Start
//! KMahjongg layout representation to use only during parsing.
struct KMahjonggLayout : Layout
{
    std::string version;
};
// KMahjonggLayout End

// kmahjonggLayoutFieldsToPositions Start
typedef std::vector<std::string> Field;
typedef std::vector<Field> Fields;

KMahjonggLayout::Positions kmahjonggLayoutFieldsToPositions(
    const Fields &fields,
    int width,
    int height
) {
    KMahjonggLayout::Positions positions;
    for (int fieldId = 0; fieldId < fields.size(); ++fieldId)
    {
        auto field = fields[fieldId];
        for (int row = 0; row < height - 1; ++row)
        {
            for (int column = 0; column < width - 1; ++column)
            {
                // Detect tile.
                if (
                    (field[row][column] == '1') &&
                    (field[row][column + 1] == '2') &&
                    (field[row + 1][column] == '4') &&
                    (field[row + 1][column + 1] == '3')
                ) {
                    positions.push_back({fieldId, row, column});
                }
            }
        }
    }
    return positions;
}
// kmahjonggLayoutFieldsToPositions End
// parseLayout Start
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
// parseLayout End

} // namespace layout
} // namespace mc

#endif // MAHJONG_COMPONENTS_LAYOUT_H

