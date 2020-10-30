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
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
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
  return net_used_memory / total_memory * 100;
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

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

unordered_map<string, vector<unsigned int>> ReadProcStat() {
  string line;
  unordered_map<string, vector<unsigned int>> stats;
  std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  while (stream.is_open() && std::getline(stream, line)) {
    std::istringstream linestream(line);
    string name;
    unsigned int value;
    linestream >> name;
    vector<unsigned int> values;
    while (linestream >> value) {
      values.push_back(value);
    }
    stats.insert({name, std::move(values)});
  }
  return stats;
}

int LinuxParser::TotalProcesses() {
  return ReadProcStat()[kProcStatProcesses][0];
}

int LinuxParser::RunningProcesses() {
  return ReadProcStat()[kProcStatProcsRunning][0];
}

string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  while (stream.is_open() && std::getline(stream, line)) {
    std::istringstream linestream(line);
    string name;
    linestream >> name;
    if (name == kProcStatusName) {
      string command_name;
      linestream >> command_name;
      return command_name;
    }
  }
  return "";
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

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }