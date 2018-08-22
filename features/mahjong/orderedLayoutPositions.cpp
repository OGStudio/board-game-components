FEATURE mahjong.h/Include
#include <random>

FEATURE mahjong.h/Impl
Layout::Positions orderedLayoutPositions(
    Layout::Positions positions,
    unsigned int seed
) {
    // Initialize random number generator.
    std::mt19937 random(seed);

    Layout::Positions orderedPositions;

    // Pick positions according to random number generator.
    while (auto positionsLeft = positions.size())
    {
        auto id = random() % positionsLeft;
        auto position = positions[id];
        positions.erase(positions.begin() + id);
        orderedPositions.push_back(position);
    }

    return orderedPositions;
}
