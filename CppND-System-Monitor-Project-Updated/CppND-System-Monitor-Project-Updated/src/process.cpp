#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "linux_parser.h"

#include "process.h"

using std::string;
using std::to_string;
using std::vector;
Process::Process(int pid){
pid_=pid;

long seconds = LinuxParser::UpTime() - LinuxParser::UpTime(pid_);
long totaltime = LinuxParser::ActiveJiffies(pid);
CpuUtilization_=(totaltime*1.0/sysconf(_SC_CLK_TCK)/(seconds*1.0));
command_= LinuxParser::Command(pid);
Ram_=LinuxParser::Ram(pid);
Uid_=LinuxParser::Uid(pid);
User_=LinuxParser::User(pid);
UpTime_=LinuxParser::UpTime(pid);

}
// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const { return CpuUtilization_; }

// TODO: Return the command that generated this process
string Process::Command() const { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() const{ return Ram_; }

// TODO: Return the user (name) that generated this process
string Process::User() const { return User_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() const { return LinuxParser::UpTime()-UpTime_; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    return CpuUtilization()  < a.CpuUtilization() ; 

    }