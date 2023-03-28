#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::stol;
using std::stoi;
using std::string;
using std::to_string;
using std::vector;

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
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line,key,value;
  float MemTotal,MemFree;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream>>key>>value;
      if (key=="MemTotal:"){
        MemTotal=stof(value);
      }
      else if(key=="MemFree:"){
        MemFree=stof(value);
      }
    }
  }
  return (MemTotal-MemFree)/MemTotal; }

// TODO: Read and return the system uptime
long int LinuxParser::UpTime() {   
  string line,value;
  long int Time;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if(filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream>>value;
    Time=stol(value);
  }
  return Time; }



// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return ActiveJiffies()+ IdleJiffies(); }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  string line,value;
  vector<string> pidstat;  
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
    if (filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
      while(linestream>>value){
        pidstat.push_back(value);
      }
    }
  long utime,stime,cutime,cstime,total_time;
  utime = stol(pidstat[13]);
  stime = stol(pidstat[14]);
  cutime = stol(pidstat[15]);
  cstime = stol(pidstat[16]);
  total_time =utime+stime+cutime+cstime;

  return total_time; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  vector<string> source = CpuUtilization();
  vector<long> longsource(source.size());
  std::transform(source.begin(), source.end(), longsource.begin(),
        [](string const& val) {return stol(val);});
  return longsource[CPUStates::kUser_]+longsource[CPUStates::kSystem_]+longsource[CPUStates::kNice_]+
  longsource[CPUStates::kIRQ_]+longsource[CPUStates::kSoftIRQ_]+longsource[CPUStates::kSteal_];
  }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<string> source = CpuUtilization();
  vector<long> longsource(source.size());
  std::transform(source.begin(), source.end(), longsource.begin(),
        [](string const& val) {return stol(val);});
  return longsource[CPUStates::kIdle_]+longsource[CPUStates::kIOwait_];

}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line,cpu,time;
  vector<string> timeseries;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream>>cpu;
    while(linestream>>time){
      timeseries.push_back(time);
    }
  }
  return timeseries; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line,key,value;
  int RunP;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
         linestream>>key>>value;
      if (key == "processes"){
         RunP=stoi(value);
      }
    }

  }

  return RunP; }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line,key,value;
  int RunP;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
         linestream>>key>>value;
      if (key == "procs_running"){
         RunP=stoi(value);
      }
    }

  }

  return RunP; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string command,line,command_r; 
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream>>command;
    if (command.size()>40){
      command_r=command.substr(0,40);
    }

  }
  return command_r; }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {  
  string line,key,value,Ramst;
  long Ram;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream>>key>>value;
      if(key=="VmSize:"){
      Ram=stol(value)/1024;
      Ramst=std::to_string(Ram);
      break;
      }
    }
  }
  return Ramst; }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line,key,value,uid;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream>>key>>value;
      if(key=="Uid:"){
      uid=value;
      break;
      }
    }
  }
  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string line,username,password,id;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream>>username>>password>>id;
      if(id==Uid(pid)){
      break;
       }
     }
   return username;
  }else
  {
    return "0";
  }
  
}


// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line,value;
  float uptime;
  vector<string> pidstat;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while(linestream>>value){
      pidstat.push_back(value);
    }

  }
  uptime=stof(pidstat[21])/sysconf(_SC_CLK_TCK);
  return static_cast<long>(uptime); }
