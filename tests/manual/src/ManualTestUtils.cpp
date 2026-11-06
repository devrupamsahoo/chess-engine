//
// Created by devrupamsahoo on 05-11-2026.
//

#include "../include/ManualTestUtils.h"

#include <iostream>

namespace ManualTests {

    // tracks the total number of failed expectations across all manual tests
    int failures = 0;

    // checks a test condition and records a failure with its name when the condition is false
    void expect(bool condition, const std::string &testName) {
        if (!condition) {
            std::cerr << "FAIL: " << testName << '\n';
            ++failures;
        }
    }

}
