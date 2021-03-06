#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>
#include <unordered_map>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/sys/kernel/osrelease"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// Memory info keys
const std::string kMemInfoTotal{"MemTotal:"};
const std::string kMemInfoFree{"MemFree:"};
const std::string kMemInfoAvailable{"MemAvailable:"};
const std::string kMemInfoBuffers{"Buffers:"};
const std::string kMemInfoCached{"Cached:"};
const std::string kMemInfoReclaimable{"SReclaimable:"};
const std::string kMemInfoShared{"Shmem"};

// Helpers
std::unordered_map<std::string, std::vector<unsigned long>> ReadProcStat();
std::vector<unsigned long> ReadProcStat(int pid);

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU (system)
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
std::vector<unsigned long> CpuUtilization();

// /proc/[pid]/stat adjusted for 0-based indexing
enum ProcStat {
  kProcStatUtime_ = 13,
  kProcStatStime_ = 14,
  kProcStatCutime_ = 15,
  kProcStatCstime_ = 16,
  kProcStatStarttime_ = 21
};

// CPU (process)
enum CPUTime {
  kUtime_ = 0,
  kStime_,
  kCutime_,
  kCstime_,
  kStarttime_
};
std::vector<unsigned long> CpuUtilization(int pid);

// Processes
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);

const std::string kProcStatCpu{"cpu"};
const std::string kProcStatProcesses{"processes"};
const std::string kProcStatProcsRunning{"procs_running"};
const std::string kProcStatusName{"Name:"};
const std::string kProcStatusUid{"Uid:"};
const std::string kProcStatusVmSize{"VmSize:"};
};  // namespace LinuxParser

#endif