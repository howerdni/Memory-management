#include "ncurses_display.h"
#include "system.h"
#include "linux_parser.h"
#include <string>
#include <vector>
using std::string;
#include<iostream>
using std::vector;

int main() {
  System system;
  NCursesDisplay::Display(system);
  vector<string> A = LinuxParser::CpuUtilization();
  for (string i: A)
    std::cout << i << ' ';
}