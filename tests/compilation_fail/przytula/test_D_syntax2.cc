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

using test_D_syntax2 = Program<
D<Id("1"), Num<1>>,
D<Id("a"), Lea<Id("1")>>>;
constexpr auto test_D_syntax2_res = test_machine::boot<test_D_syntax1>();

int main() {

}
