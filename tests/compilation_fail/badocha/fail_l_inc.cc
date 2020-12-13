#include "computer.h"
#include <array>
#include <iostream>

int main() {
    using test_machine = Computer<4, int>;

    // LValue tests
    using fail_l_inc = Program<Inc<Num<0>>>;

    test_machine::boot<fail_l_inc>();
}
