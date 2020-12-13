#include "computer.h"
#include <array>
#include <iostream>

// Operator == dla std::array jest constexpr dopiero od C++20.
template<class T, std::size_t N>
constexpr bool compare(std::array<T, N> const& arg1, std::array<T, N> const& arg2) {
    for (size_t i = 0; i < N; ++i)
        if (arg1[i] != arg2[i]) return false;
    return true;
}

/*
 * Tests by Wojciech Przytuła
 * All rights left
 * */

using test_machine = Computer<4, int>;

using test_jumps = Program<
        Dec<Mem<Num<0>>>,
        Jz<Id("XD")>,
        Inc<Mem<Num<0>>>,
        Jz<Id("XD")>,
        Inc<Mem<Num<1>>>,
        Label<Id("xd")>
>;
constexpr std::array<int, 4> test_jumps_res = {0, 0, 0, 0};
static_assert(compare(test_machine::boot<test_jumps>(), test_jumps_res));

int main() {

}
