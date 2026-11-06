//
// Created by devrupamsahoo on 05-11-2026.
//

#ifndef CHESS_ENGINE_MANUALTESTUTILS_H
#define CHESS_ENGINE_MANUALTESTUTILS_H


#include <string>

namespace ManualTests {
    // tracks the total number of failed manual test expectations
    extern int failures;

    // checks a test condition and reports the result using the test name
    void expect(bool condition, const std::string &testName);
}


#endif //CHESS_ENGINE_MANUALTESTUTILS_H
