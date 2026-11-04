//
// Created by sahoo on 27-06-2026.
//

#ifndef CHESS_ENGINE_RAYTABLE_H
#define CHESS_ENGINE_RAYTABLE_H

#include <vector>
#include "Enums.h"

class RayTable {
private:
    // stores each ray from the nearest square outward, excluding its source square
    std::vector<int> rays[squareCount][directionCount];

    // precomputes rays for every square in every direction
    void initializeRays();
public:
    // ctor
    RayTable();

    // returns squares along a direction from nearest to farthest
    const std::vector<int>& getRay(int square, Direction direction) const;

    // dtor
    ~RayTable() = default;
};

namespace AttackTables {
    extern RayTable rayTable;
}

#endif //CHESS_ENGINE_RAYTABLE_H
