#include <string>

#include "format.h"
#include <iomanip>
using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
  std::ostringstream stream;
  int hours, minutes;
  hours = seconds / 3600;
  seconds = seconds % 3600;
  minutes = seconds / 60;
  seconds = seconds % 60;

  stream << std::setw(2) << std::setfill('0') << hours << ":" 
     << std::setw(2) << std::setfill('0') << minutes << ":"
     << std::setw(2) << std::setfill('0') << seconds;
  return stream.str();
}