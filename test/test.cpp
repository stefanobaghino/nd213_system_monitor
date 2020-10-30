#include <cassert>
#include <iostream>
#include <string>

#include "format.h"
#include "linux_parser.h"

void format_test(long input, std::string expected) {
    assert(Format::ElapsedTime(input) == expected);
}

void meminfo_test() {
    auto memory_utilization = LinuxParser::MemoryUtilization();
    assert(memory_utilization > 0.0 && memory_utilization < 100.0);
}

void uptime_test() {
    auto uptime = LinuxParser::UpTime();
    assert(uptime > 0);
}

void total_processes_test() {
    auto total_processes = LinuxParser::TotalProcesses();
    assert(total_processes > 0);
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

    meminfo_test();

    uptime_test();

    total_processes_test();

    std::cout << "No failed assertion, all tests pass.\n";
}
