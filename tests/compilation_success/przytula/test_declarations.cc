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

using test_declarations = Program<
D<Id("a"), Num<-3>>,
D<Id("b"), Num<-2>>,
D<Id("c"), Num<-1>>,
D<Id("a"), Num<-1>>
>;
constexpr std::array<int, 4> test_declarations_res = {-3, -2, -1, -1};
static_assert(compare(test_machine::boot<test_declarations>(), test_declarations_res));

int main() {

}
