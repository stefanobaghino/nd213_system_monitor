#include <string>
#include <sstream>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
    std::ostringstream oss;
    long minutes = seconds / 60;
    long hours = minutes / 60;
    seconds = seconds % 60;
    minutes = minutes % 60;
    if (hours < 10) oss << '0';
    oss << hours << ':';
    if (minutes < 10) oss << '0';
    oss << minutes << ':';
    if (seconds < 10) oss << '0';
    oss << seconds;
    return oss.str();
}
