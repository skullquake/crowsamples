#include"./mon.hpp"
#include<unistd.h>
#include<ios>
#include<iostream>
#include<fstream>
#include<string>
void Mon::mem_usage(double&vm_usage,double&resident_set){
	vm_usage = 0.0;
	resident_set = 0.0;
	std::ifstream stat_stream("/proc/self/stat",std::ios_base::in); //get info from proc
	//directory
	//create some variables to get info
	std::string pid, comm, state, ppid, pgrp, session, tty_nr;
	std::string tpgid, flags, minflt, cminflt, majflt, cmajflt;
	std::string utime, stime, cutime, cstime, priority, nice;
	std::string O, itrealvalue, starttime;
	unsigned long vsize;
	long rss;
	stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
	>> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
	>> utime >> stime >> cutime >> cstime >> priority >> nice
	>> O >> itrealvalue >> starttime >> vsize >> rss; // don't care
	//about the rest
	stat_stream.close();
	long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // for x86-64 is configured
	//to use 2MB pages
	vm_usage = vsize / 1024.0;
	resident_set = rss * page_size_kb;
}
