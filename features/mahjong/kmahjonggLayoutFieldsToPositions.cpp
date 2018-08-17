FEATURE mahjong.h/Impl
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
