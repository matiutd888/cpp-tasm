#include "computer.h"
#include <array>
#include <iostream>

/*
 * Tests by Wojciech Przytuła
 * All rights left
 * */

// tests that should compile AND ofc not result in infinite loop:
using test_finite_loop = Program<
Label<Id("loop")>,
Dec<Mem<Lea<Id("MEM0")>>>,
Js<Id("LOOP")>,
D<Id("mem0"), Num<INT32_MIN + 100>>
>;
constexpr auto test_finite_loop_res = test_machine::boot<test_finite_loop>();

int main() {
    test_machine::boot<test_finite_loop>();
}
