#include "computer.h"
#include <array>
#include <iostream>

int main() {
    using test_machine = Computer<4, int>;

    // Test for passing non instruction
    using fail_lea = Program<Lea<Id("a")>>;

    test_machine::boot<fail_lea>();
}
