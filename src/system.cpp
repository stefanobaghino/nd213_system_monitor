#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System() : kernel_(LinuxParser::Kernel()), operating_system_(LinuxParser::OperatingSystem()) {
}

Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() {
    auto pids = LinuxParser::Pids();
    processes_.clear();
    for (auto pid : pids) {
        Process process(pid);
        processes_.push_back(process);
    }
    std::sort(processes_.begin(), processes_.end());
    std::reverse(processes_.begin(), processes_.end());
    return processes_;
}

std::string System::Kernel() const { return kernel_; }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() const { return operating_system_; }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }