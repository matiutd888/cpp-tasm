#include "computer.h"
#include <array>
#include <iostream>

int main() {
    using test_machine = Computer<4, int>;

    // LValue tests
    using fail_l_mov = Program<Mov<Num<0>, Num<0>>>;

    test_machine::boot<fail_l_mov>();
}
