#include <cassert>
#include <iostream>
#include <string>

#include "format.h"
#include "linux_parser.h"
#include "processor.h"
#include "process.h"

void format_test(long input, std::string expected) {
    assert(Format::ElapsedTime(input) == expected);
}

void meminfo_test() {
    auto memory_utilization = LinuxParser::MemoryUtilization();
    assert(memory_utilization > 0.0 && memory_utilization <= 1.0);
}

void uptime_test() {
    auto uptime = LinuxParser::UpTime();
    assert(uptime > 0);
}

void total_processes_test() {
    auto total_processes = LinuxParser::TotalProcesses();
    assert(total_processes > 0);
}

void running_processes_test() {
    auto running_processes = LinuxParser::RunningProcesses();
    assert(running_processes > 0);
}

void command_test() {
    assert(!LinuxParser::Command(1).empty());
}

void ram_test() {
    assert(!LinuxParser::Ram(1).empty());
}

void uid_test() {
    assert(!LinuxParser::Uid(1).empty());
}

void user_test() {
    assert(LinuxParser::User(1) == "root");
}

void process_cpu_utilization_test() {
    auto pid = 1;
    auto p = LinuxParser::ReadProcStat(pid);
    auto u = LinuxParser::CpuUtilization(pid);
    assert(u.size() == 5);
    assert(p[LinuxParser::ProcStat::kProcStatUtime_] == u[LinuxParser::CPUTime::kUtime_]);
    assert(p[LinuxParser::ProcStat::kProcStatStime_] == u[LinuxParser::CPUTime::kStime_]);
    assert(p[LinuxParser::ProcStat::kProcStatCutime_] == u[LinuxParser::CPUTime::kCutime_]);
    assert(p[LinuxParser::ProcStat::kProcStatCstime_] == u[LinuxParser::CPUTime::kCstime_]);
    assert(p[LinuxParser::ProcStat::kProcStatStarttime_] == u[LinuxParser::CPUTime::kStarttime_]);
}

void process_uptime_test() {
    auto uptime = LinuxParser::UpTime(1);
    assert(uptime >= 0 && uptime <= 1);
}

void kernel_version_test() {
    auto kernel = LinuxParser::Kernel();
    assert(!kernel.empty() && kernel != "version");
}

void cpu_utilization_test() {
    auto user_t1 = LinuxParser::CpuUtilization()[0];
    assert(user_t1 > 0);
    auto user_t2 = LinuxParser::CpuUtilization()[0];
    assert(user_t1 <= user_t2);
}

void processor_test() {
    Processor processor;
    float utilization_t1 = processor.Utilization();
    float utilization_t2 = processor.Utilization();
    assert(utilization_t1 >= 0 && utilization_t1 <= 1);
    assert(utilization_t2 >= 0 && utilization_t2 <= 1);
}

void process_test() {
    Process init(1);
    assert(init.Pid() == 1);
    assert(init.User() == "root");
    assert(init.UpTime() >= 0);
    assert(!init.Command().empty());
    assert(!init.Ram().empty());
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

    running_processes_test();

    command_test();

    ram_test();

    uid_test();

    user_test();

    process_cpu_utilization_test();

    process_uptime_test();

    kernel_version_test();

    cpu_utilization_test();

    processor_test();

    process_test();

    std::cout << "No failed assertion, all tests pass.\n";
}
