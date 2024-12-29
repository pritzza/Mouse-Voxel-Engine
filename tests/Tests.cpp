#include "VoxelTests.h"

#include <iostream>
#include <string>

bool runTests()
{
    bool success = true;

    // List of test functions and their names
    struct Test {
        std::string name;
        bool (*testFunc)();
    };

    Test tests[] = {
        {"testFullySolid1x1x1", testFullySolid1x1x1},
        {"testFullySolid3x1x1", testFullySolid3x1x1},
        {"testFullySolid1x3x1", testFullySolid1x3x1},
        {"testFullySolid1x1x3", testFullySolid1x1x3},
        {"testFullySolid3x3x3", testFullySolid3x3x3},
        {"testSparse3x3x3", testSparse3x3x3}
    };

    std::cout << "============================== Running Tests ==============================\n";

    // Iterate through each test
    for (const auto& test : tests) {
        std::cout << "Running " << test.name << "...\n";

        bool result = test.testFunc(); // Run the test function

        // Print the result of the test
        if (result) {
            std::cout << "\033[32m[PASS]\033[0m " << test.name << " passed!\n"; // Green color for pass
        }
        else {
            std::cerr << "\033[31m[FAIL]\033[0m " << test.name << " failed!\n"; // Red color for fail
            success = false; // Mark failure
        }

        std::cout << "\n\n";
    }

    // Final output depending on the results
    std::cout << "\n==============================================================================\n";
    if (success) {
        std::cout << "\033[32mAll tests passed.\033[0m\n"; // Green for success
    }
    else {
        std::cerr << "\033[31mSome tests failed.\033[0m\n"; // Red for failure
    }

    return success;
}
