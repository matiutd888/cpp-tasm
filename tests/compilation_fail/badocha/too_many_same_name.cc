#include "computer.h"
#include <array>
#include <iostream>

int main() {
    using test_machine = Computer<4, int>;

    using too_many_same_name = Program<D<Id("a"), Num<42>>, D<Id("a"), Num<42>>, D<Id("a"), Num<42>>, D<Id("a"), Num<42>>, D<Id("a"), Num<42>>>;
    test_machine::boot<too_many_same_name>();
}
