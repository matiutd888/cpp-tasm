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

using test_infinite_loop = Program<
Label<Id("loop")>,
Mov<Mem<Lea<Id("MEM0")>>, Num<0>>,
Jmp<Id("LOOP")>,
D<Id("mem0"), Num<-1000000>>
>;
constexpr auto test_infinite_loop_res = test_machine::boot<test_infinite_loop>();

int main() {

}
