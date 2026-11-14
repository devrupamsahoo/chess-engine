#include "manual/include/ManualTestUtils.h"
#include "manual/include/RunManualTests.h"
#include "move-generation/include/RunMoveGenerationTests.h"
#include "evaluation/include/RunEvaluationTests.h"

#include <iostream>

// runs the manual and move-generation test suites
int main() {
    // runManualTests();

    EvaluationTests::runEvaluationTests();

    // MoveGenerationTests::runMoveGenerationTests();

    using namespace ManualTests;

    if (failures == 0) {
        std::cout << '\n';
        std::cout << "All tests passed!\n";
    } else {
        std::cout << failures << " test(s) failed.\n";
    }

    return failures == 0 ? 0 : 1;
}