#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();

 private:
  unsigned long prev_idle{0};
  unsigned long prev_total{0};
  float prev_utilization{0};
};

#endif