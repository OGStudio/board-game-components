FEATURE mahjong.h/Include
#include <random>

FEATURE mahjong.h/Impl
Layout::Positions orderedLayoutPositions(
    Layout::Positions positions,
    std::mt19937 &randomNumberGenerator
) {
    Layout::Positions orderedPositions;

    // Pick positions according to random number generator.
    while (auto positionsLeft = positions.size())
    {
        auto id = randomNumberGenerator() % positionsLeft;
        auto position = positions[id];
        positions.erase(positions.begin() + id);
        orderedPositions.push_back(position);
    }

    return orderedPositions;
}
