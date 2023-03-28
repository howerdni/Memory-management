#include "processor.h"
#include "linux_parser.h"
#include <unistd.h>
Processor::Processor(): Nonidle_(0),Idel_(0),Total_(0){};
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
//long Nonidle = LinuxParser::ActiveJiffies();
//long Idel= LinuxParser::IdleJiffies();
//long Total = LinuxParser::Jiffies();
//Processor::Nonidle_ = Nonidle;
//Processor::Idel_ = Idel;
//Processor::Total_ = Total;
//sleep(1);
long prevTotal, prevIdel;
prevTotal = Processor::Total_;
prevIdel = Processor::Idel_;
long currTotal, currIdel,currNonidel;
currTotal = LinuxParser::Jiffies();
currIdel = LinuxParser::IdleJiffies();
currNonidel = LinuxParser::ActiveJiffies();
long Totald = currTotal - prevTotal;
long Ideld = currIdel - prevIdel;
float usage_rate=(Totald-Ideld)*1.0/Totald;
Processor::Total_=currTotal;
Processor::Idel_=currIdel;
Processor::Nonidle_=currNonidel;
return usage_rate;}
 //return 5;}