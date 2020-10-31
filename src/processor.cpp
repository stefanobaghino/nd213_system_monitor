#include <vector>

#include "processor.h"
#include "linux_parser.h"

float Processor::Utilization() {
    std::vector<unsigned long> cpu_info = LinuxParser::CpuUtilization();

    unsigned long user = cpu_info[LinuxParser::CPUStates::kUser_];
    unsigned long nice = cpu_info[LinuxParser::CPUStates::kNice_];
    unsigned long system = cpu_info[LinuxParser::CPUStates::kSystem_];
    unsigned long idle = cpu_info[LinuxParser::CPUStates::kIdle_];
    unsigned long io_wait = cpu_info[LinuxParser::CPUStates::kIOwait_];
    unsigned long irq = cpu_info[LinuxParser::CPUStates::kIRQ_];
    unsigned long soft_irq = cpu_info[LinuxParser::CPUStates::kSoftIRQ_];
    unsigned long steal = cpu_info[LinuxParser::CPUStates::kSteal_];

    unsigned long new_idle = idle + io_wait;
    unsigned long new_total = new_idle + user + nice + system + irq + soft_irq + steal;

    float delta_idle = new_idle - prev_idle;
    float delta_total = new_total - prev_total;

    if (delta_total == 0) {
        return prev_utilization;
    }

    const float cpu_utilization = (delta_total - delta_idle) / delta_total;

    prev_idle = new_idle;
    prev_total = new_total;
    prev_utilization = cpu_utilization;

    return cpu_utilization;
}
