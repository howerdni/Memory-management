#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid();                               // TODO: See src/process.cpp
  std::string User() const;                      // TODO: See src/process.cpp
  std::string Command() const;                   // TODO: See src/process.cpp
  float CpuUtilization ()const;                  // TODO: See src/process.cpp
  std::string Ram ()const;                       // TODO: See src/process.cpp
  long int UpTime ()const;                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
  int pid_;
  float CpuUtilization_;
  std::string command_;
  std::string Ram_;
  std::string Uid_;
  std::string User_;
  long int UpTime_;
};

#endif