#include "computer.h"
#include <array>
#include <iostream>

/*
 * Tests by Wojciech Przytuła
 * All rights left
 * */
using test_machine = Computer<4, int>;
// tests that should compile AND ofc not result in infinite loop:
using test_id1 = Program<
        D<Id("a"), Num<1>>,
        Label<Id("a")>,
        Dec<Mem<Num<0>>>,
        Js<Id("end")>,
        Label<Id("A")>,
        Jz<Id("A")>,
        Label<Id("loop")>,
        Jmp<Id("loop")>,
        Label<Id("end")>
>;
constexpr auto test_id1_res = test_machine::boot<test_id1>();

int main() {
    test_machine::boot<test_id1>();
}
