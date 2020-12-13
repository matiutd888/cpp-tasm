#include "computer.h"
#include <array>
#include <iostream>

/*
 * Tests by Wojciech Przytuła
 * All rights left
 * */
using test_machine = Computer<4, int>;
// tests that should compile AND ofc not result in infinite loop:
using test_underflow = Program<
D<Id("mem0"), Num<INT32_MIN>>,
Dec<Mem<Lea<Id("MEM0")>>>>;
constexpr auto test_underflow_res = test_machine::boot<test_underflow>();

int main() {
    test_machine::boot<test_underflow>();
}
