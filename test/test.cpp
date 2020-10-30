#include <cassert>
#include <iostream>
#include <string>

#include "format.h"

void format_test(long input, std::string expected) {
    assert(Format::ElapsedTime(input) == expected);
}

int main() {

    format_test(   0, "00:00:00");
    format_test(   1, "00:00:01");
    format_test(  10, "00:00:10");
    format_test(  60, "00:01:00");
    format_test(  61, "00:01:01");
    format_test(  70, "00:01:10");
    format_test(3600, "01:00:00");
    format_test(3601, "01:00:01");
    format_test(3610, "01:00:10");
    format_test(3660, "01:01:00");
    format_test(3661, "01:01:01");
    format_test(3670, "01:01:10");
    format_test(8519, "02:21:59");

    std::cout << "No failed assertion, all tests pass.\n";
}
