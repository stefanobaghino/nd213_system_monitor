#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {
  command_ = LinuxParser::Command(pid_);
  cpu_utilization_ = ComputeCpuUtilization();
  ram_ = LinuxParser::Ram(pid_);
  user_ = LinuxParser::User(pid_);
  uptime_ = LinuxParser::UpTime(pid_);
}

int Process::Pid() const { return pid_; }

float Process::CpuUtilization() { return cpu_utilization_; }

string Process::Command() const { return command_; }

string Process::Ram() const { return ram_; }

string Process::User() const { return user_; }

long int Process::UpTime() const { return uptime_; }

bool Process::operator<(Process const& a) const { return cpu_utilization_ < a.cpu_utilization_; }

float Process::ComputeCpuUtilization() {
  auto proc_stat = LinuxParser::CpuUtilization(pid_);

  float utime = proc_stat[LinuxParser::CPUTime::kUtime_];
  float stime = proc_stat[LinuxParser::CPUTime::kStime_];
  float cutime = proc_stat[LinuxParser::CPUTime::kCutime_];
  float cstime = proc_stat[LinuxParser::CPUTime::kCstime_];
  float starttime = proc_stat[LinuxParser::CPUTime::kStarttime_];
  float uptime = LinuxParser::UpTime();
  float hz = sysconf(_SC_CLK_TCK);

  float total_time = utime + stime + cutime + cstime;
  float starttime_seconds = starttime / hz;
  float seconds = uptime - starttime_seconds;

  return (total_time / hz) / seconds;
}
