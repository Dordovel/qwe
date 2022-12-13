#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
#include <unistd.h>

using namespace std::chrono_literals;

struct ProcInfo
{
        std::string tcomm;
        std::string state;
        int pid;
        int ppid;
        int pgrp;
        int sid;
        int tty_nr;
        int tty_pgrp;
        int flags;
        int min_flt;
        int cmin_flt;
        int maj_flt;
        int cmaj_flt;
        int utime;
        int stime;
        int cutime;
        int cstime;
};

ProcInfo get_proc_info(int pid)
{
    ProcInfo proc;

    std::stringstream path;
    path << "/proc/" << pid << "/stat";

    std::ifstream file;
    file.open(path.str());
    if (file.is_open())
    {
        std::stringstream buffer;
        buffer << file.rdbuf();
        buffer >> proc.pid >> proc.tcomm >> proc.state >> proc.ppid >>
            proc.pgrp >> proc.sid >> proc.tty_nr >> proc.tty_pgrp >>
            proc.flags >> proc.min_flt >> proc.cmin_flt >> proc.maj_flt >>
            proc.cmaj_flt >> proc.utime >> proc.stime >> proc.cutime >>
            proc.cstime;
    }

    return proc;
}

struct CpuTime
{
        unsigned int user;
        unsigned int nice;
        unsigned int system;
        unsigned int idle;
        unsigned int iowait;
        unsigned int irq;
        unsigned int softirq;
        unsigned int steal;
        unsigned int guest;
        unsigned int guest_nice;
};

unsigned long get_cpu_total(CpuTime time)
{
    return time.user + time.nice + time.system + time.idle + time.iowait +
           time.irq + time.softirq + time.steal + time.guest + time.guest_nice;
}

CpuTime get_cpu_info()
{
    CpuTime cpu;
    std::string path("/proc/stat");

    std::ifstream file;
    file.open(path);
    if (file.is_open())
    {
        std::string line;
        getline(file, line);

        std::stringstream buffer;
        buffer << line;
        std::string type;
        buffer >> type >> cpu.user >> cpu.nice >> cpu.system >> cpu.idle >>
            cpu.iowait >> cpu.irq >> cpu.softirq >> cpu.steal >> cpu.guest >>
            cpu.guest_nice;
    }

    return cpu;
}

int main(int argc, char *argv[])
{
    int currentPid = 23765;
    int oldCpu, oldUse, oldTotal;
    oldCpu = oldUse = oldTotal = 0;

    while (true)
    {
        int vmSize, vmRss, shared, trs, lrs, drs, dirtyPages;

        std::stringstream path;
        path << "/proc/" << currentPid << "/statm";

        std::ifstream file;
        file.open(path.str());
        if (file.is_open())
        {
            std::stringstream buffer;
            buffer << file.rdbuf();
            buffer >> vmSize >> vmRss >> shared >> trs >> lrs >> drs >>
                dirtyPages;
        }

#ifdef _SC_PAGESIZE
        // recommended POSIX
        int page_size = sysconf(_SC_PAGESIZE);
#elif _SC_PAGE_SIZE
        // alias
        int page_size = sysconf(_SC_PAGE_SIZE);
#endif

        ProcInfo procInfo = get_proc_info(currentPid);
        CpuTime cpuInfo = get_cpu_info();
        unsigned long cpu_time_total = get_cpu_total(cpuInfo);
        std::cout << "%Cpu: "
                  << ((procInfo.utime - oldUse) + (procInfo.stime - oldCpu)) *
                         100 / (float)(cpu_time_total - oldTotal)
                  << "  %Mem: " << (vmRss * page_size) / (1024 * 1024)
                  << std::endl;
        oldUse = procInfo.utime;
        oldTotal = cpu_time_total;
        oldCpu = procInfo.stime;

        std::this_thread::sleep_for(1s);
    }
    return 0;
}

/*
int main (int argc, char *argv[])
{
    int pid = 2844;
    int vmSize, vmRss, shared, trs, lrs, drs, dirtyPages;

    std::stringstream path;
    path << "/proc/" << pid << "/statm";

    std::ifstream file;
    file.open(path.str());
    if(file.is_open())
    {
        std::stringstream buffer;
        buffer << file.rdbuf();
        buffer >> vmSize >> vmRss >> shared >> trs >> lrs >> drs >> dirtyPages;
    }

#ifdef _SC_PAGESIZE
    // recommended POSIX
    int page_size = sysconf(_SC_PAGESIZE);
#elif _SC_PAGE_SIZE
    // alias
    int page_size = sysconf(_SC_PAGE_SIZE);
#endif

    std::cout<<vmSize * page_size <<" "<< vmRss * page_size << " " << shared *
page_size << " " << trs * page_size << " " << lrs * page_size << " " << drs *
page_size << " " << dirtyPages * page_size<<std::endl;

    return 0;
}*/
