#include "computer.h"
#include <array>

// Operator == dla std::array jest constexpr dopiero od C++20.
template<class T, std::size_t N>
constexpr bool compare(std::array<T, N> const& arg1, std::array<T, N> const& arg2) {
    for (size_t i = 0; i < N; ++i)
        if (arg1[i] != arg2[i]) return false;
    return true;
}

using tmpasm_jump = Program<
        Inc<Mem<Num<0>>>,
        Jmp<Id("stop")>,
        Inc<Mem<Num<0>>>>;

int main() {
    static_assert(compare(
            Computer<1, int>::boot<tmpasm_jump>(),
            std::array<int, 1>({1})),
            "Failed [tmpasp_jump].");
}
