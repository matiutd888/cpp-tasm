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

// I can't make this test fail, yet I believe it should.
using test_syntax6 = Program<
Label<4ULL>>;
constexpr auto test_syntax6_res = test_machine::boot<test_syntax6>();

int main() {

}
