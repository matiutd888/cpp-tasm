#include "computer.h"
#include <array>
#include <iostream>

int main() {
    using test_machine = Computer<4, int>;

    using fail_not_pvalue = Program<Add<Add<Mem<Num<0>>, Num<69>>, Num<42>>>;
    test_machine::boot<fail_not_pvalue>();
}
