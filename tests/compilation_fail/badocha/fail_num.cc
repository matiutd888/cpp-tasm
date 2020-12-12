#include "computer.h"
#include <array>
#include <iostream>

int main() {
    using test_machine = Computer<4, int>;

    // Test for passing non instruction
    using fail_num = Program<Num<0>>;

    test_machine::boot<fail_num>();
}
