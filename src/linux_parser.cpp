#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <limits>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::unordered_map;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string kernel;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, kernel);
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  string name;
  long value;
  unordered_map<string, float> mem_info;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  while (stream >> name >> value) {
    mem_info.insert({name, value});
    stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  float total_memory = mem_info[kMemInfoTotal];
  float total_used_memory = total_memory - mem_info[kMemInfoFree];
  float cached_memory = mem_info[kMemInfoCached] + mem_info[kMemInfoReclaimable] - mem_info[kMemInfoShared];
  float non_cache_buffers_memory = mem_info[kMemInfoBuffers] + cached_memory;
  float net_used_memory = total_used_memory - non_cache_buffers_memory;
  return net_used_memory / total_memory;
}

long LinuxParser::UpTime() {
  long uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return uptime;
}

std::vector<unsigned long> LinuxParser::CpuUtilization(int pid) {
  auto proc_stat = ReadProcStat(pid);
  auto utime = proc_stat[kProcStatUtime_];
  auto stime = proc_stat[kProcStatStime_];
  auto cutime = proc_stat[kProcStatCutime_];
  auto cstime = proc_stat[kProcStatCstime_];
  auto starttime = proc_stat[kProcStatStarttime_];
  return {utime, stime, cutime, cstime, starttime};
}

vector<unsigned long> LinuxParser::CpuUtilization() {
  return ReadProcStat()[kProcStatCpu];
}

unordered_map<string, vector<unsigned long>> LinuxParser::ReadProcStat() {
  string line;
  unordered_map<string, vector<unsigned long>> stats;
  std::ifstream stream(kProcDirectory + kStatFilename);
  while (stream.is_open() && std::getline(stream, line)) {
    std::istringstream linestream(line);
    string name;
    unsigned long value;
    linestream >> name;
    vector<unsigned long> values;
    while (linestream >> value) {
      values.push_back(value);
    }
    stats.insert({name, std::move(values)});
  }
  return stats;
}

std::vector<unsigned long> LinuxParser::ReadProcStat(int pid) {
  string line;
  vector<unsigned long> values;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  while (stream.is_open() && std::getline(stream, line)) {
    std::istringstream linestream(line);

    string skip;
    unsigned long value;
    linestream >> value >> skip >> skip;
    values = {value, 0, 0};

    while (linestream >> value) {
      values.push_back(value);
    }
  }
  return values;
}

int LinuxParser::TotalProcesses() {
  return ReadProcStat()[kProcStatProcesses][0];
}

int LinuxParser::RunningProcesses() {
  return ReadProcStat()[kProcStatProcsRunning][0];
}

string LinuxParser::Command(int pid) {
  string cmdline;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, cmdline);
  }
  return cmdline;
}

string LinuxParser::Ram(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  while (stream.is_open() && std::getline(stream, line)) {
    std::istringstream linestream(line);
    string name;
    linestream >> name;
    if (name == kProcStatusVmSize) {
      unsigned int mem_kb;
      linestream >> mem_kb;
      return std::to_string(mem_kb / 1024);
    }
  }
  return "";
}

string LinuxParser::Uid(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  while (stream.is_open() && std::getline(stream, line)) {
    std::istringstream linestream(line);
    string name;
    linestream >> name;
    if (name == kProcStatusUid) {
      string uid;
      linestream >> uid;
      return uid;
    }
  }
  return "";
}

string LinuxParser::User(int pid) {
  string uid = Uid(pid);
  string line;
  std::ifstream stream(kPasswordPath);
  while (stream.is_open() && std::getline(stream, line)) {
    std::istringstream linestream(line);
    string name;
    string uid_for_name;
    for (int i = 0; i < 3; i++) {
      if (i == 0) {
        std::getline(linestream, name, ':');
      } else if (i == 2) {
        std::getline(linestream, uid_for_name, ':');
        if (uid_for_name == uid) {
          return name;
        }
      } else {
        string skip;
        std::getline(linestream, skip, ':');
      }
    }
  }
  return "";
}

long LinuxParser::UpTime(int pid) {
  auto proc_stat = ReadProcStat(pid);
  auto utime = proc_stat[kUtime_];
  auto stime = proc_stat[kStime_];
  auto cutime = proc_stat[kCutime_];
  auto cstime = proc_stat[kCstime_];
  return (utime + stime + cutime + cstime) / sysconf(_SC_CLK_TCK);
}