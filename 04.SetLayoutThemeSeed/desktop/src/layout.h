
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
#define MC_LAYOUT_LOG_PREFIX "layout %s"
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
// linesToKMahjonggLayout Start
bool linesToKMahjonggLayout(
    const std::vector<std::string> &lines,
    KMahjonggLayout &layout
) {
    KMahjonggLayout layoutDraft;
    // Setup defaults.
    // KMahjongg layouts of version 1.0 have 32x16 field size.
    // So we simply set these values as defaults.
    layoutDraft.width = 32;
    layoutDraft.height = 16;
    // Depth is counted for version 1.0. So set it to zero.
    layoutDraft.depth = 0;

    // Prefixes.
    const std::string PREFIX_COMMENT = "#";
    const std::string PREFIX_VERSION = "kmahjongg-layout-v";
    const std::string PREFIX_DEPTH = "d";
    const std::string PREFIX_WIDTH = "w";
    const std::string PREFIX_HEIGHT = "h";

    Fields fields;
    Field field;
    // Count field lines to detect end-of-field.
    int fieldLineId = 0;

    for (auto ln : lines)
    {
        // Ignore comment.
        if (format::stringStartsWith(ln, PREFIX_COMMENT))
        {
            continue;
        }
        // Remove spaces.
        auto sln = format::trimmedString(ln);
        // Version.
        if (format::stringStartsWith(sln, PREFIX_VERSION))
        {
            layoutDraft.version = sln.substr(PREFIX_VERSION.length());
            // Explicitely support expected versions.
            if (
                layoutDraft.version != "1.0" &&
                layoutDraft.version != "1.1"
            ) {
                return false;
            }
        }
        // Depth.
        else if (format::stringStartsWith(sln, PREFIX_DEPTH))
        {
            auto depth = sln.substr(PREFIX_DEPTH.length());
            layoutDraft.depth = atoi(depth.c_str());
        }
        // Width.
        else if (format::stringStartsWith(sln, PREFIX_WIDTH))
        {
            auto width = sln.substr(PREFIX_WIDTH.length());
            layoutDraft.width = atoi(width.c_str());
        }
        // Height.
        else if (format::stringStartsWith(sln, PREFIX_HEIGHT))
        {
            auto height = sln.substr(PREFIX_HEIGHT.length());
            layoutDraft.height = atoi(height.c_str());
        }
        else
        {
            // Add line to current field.
            field.push_back(sln);
            ++fieldLineId;
            // Once we have collected enough lines, add current
            // field to the list of fields.
            if (fieldLineId >= layoutDraft.height)
            {
                fields.push_back(field);
                // Reset field buffer for the next field lines.
                field.clear();
                fieldLineId = 0;
            }
        }
    }
    // Make sure depth is equal to the number of fields
    // if depth was specified in layout.
    if (layoutDraft.depth)
    {
        if (fields.size() != layoutDraft.depth)
        {
            MC_LAYOUT_LOG(
                "ERROR Specified layout depth (%d) is not equal to actual one (%d)",
                layoutDraft.depth,
                fields.size()
            );
            return false;
        }
    }
    // Provide layout depth if it was counted.
    else
    {
        layoutDraft.depth = fields.size();
    }
    // Positions.
    layoutDraft.positions =
        kmahjonggLayoutFieldsToPositions(
            fields,
            layoutDraft.width,
            layoutDraft.height
        );

    // Provide cooked layout.
    layout = layoutDraft;
    return true;
}
// linesToKMahjonggLayout End
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

