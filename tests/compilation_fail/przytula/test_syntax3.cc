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

using test_syntax3 = Program<
Mem<Num<0>>>;
constexpr auto test_syntax3_res = test_machine::boot<test_syntax3>();

int main() {

}
