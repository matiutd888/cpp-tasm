#include "computer.h"
#include <array>
#include <iostream>

int main() {
    using test_machine = Computer<4, int>;

    using too_many = Program<D<Id("a"), Num<42>>, D<Id("b"), Num<42>>, D<Id("c"), Num<42>>, D<Id("d"), Num<42>>, D<Id("e"), Num<42>>>;
    test_machine::boot<too_many>();
}
