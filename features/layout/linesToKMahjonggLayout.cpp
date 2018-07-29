FEATURE layout.h/Include
#include "format.h"

FEATURE layout.h/Impl
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
