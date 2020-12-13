#include "computer.h"
#include <array>
#include <iostream>

/*
 * Tests by Wojciech Przytuła
 * All rights left
 * */

using test_machine = Computer<4, int>;


/* tests that should not run as constexpr,
 * i.e. constexpr res = machine::boot<test>() should result in compile-time error.
 * (if they don't even compile then it's good either way) */

using test_size1 = Program<
Mov<Mem<Num<2137>>, Num<1>>>;
constexpr auto test_size1_res = test_machine::boot<test_size1>();

int main() {

}
