#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
 public:
  Process(int pid);
  int Pid() const;
  std::string User() const;
  std::string Command() const;
  float CpuUtilization();
  std::string Ram() const;
  long int UpTime() const;
  bool operator<(Process const& a) const;

 private:
  int pid_;
  float cpu_utilization_{0.0};
};

#endif