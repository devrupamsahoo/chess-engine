//
// Created by sahoo on 27-06-2026.
//

#ifndef CHESS_ENGINE_RAYTABLE_H
#define CHESS_ENGINE_RAYTABLE_H

#include <vector>
#include "Enums.h"

class RayTable {
private:
    std::vector<int> rays[squareCount][directionCount];

    // precomputes rays for every square in every direction
    void initializeRays();
public:
    // ctor
    RayTable();

    // returns the precomputed ray from the given square and direction
    const std::vector<int>& getRay(int square, Direction direction) const;

    // dtor
    ~RayTable() = default;
};

namespace AttackTables {
    extern RayTable rayTable;
}

#endif //CHESS_ENGINE_RAYTABLE_H
