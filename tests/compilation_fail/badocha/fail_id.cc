#include "computer.h"
#include <array>
#include <iostream>

int main() {
    using test_machine = Computer<4, int>;

    // Test for passing non instruction
    using fail_id = Program<Id("a")>;

    test_machine::boot<fail_id>();
}
