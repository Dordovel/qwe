#include "process_memory_linux.hpp"

#include <unistd.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#ifdef __linux__

namespace
{

    wnd::Memory get_process_memory(std::string_view pid)
    {
        std::ifstream stream;
        wnd::Memory info = {0};
        std::stringstream buffer;

        int vmSize = 0, vmRss = 0, shared = 0, trs = 0, lrs = 0, drs = 0,
            dirtyPages = 0;

        stream.open("/proc/" + std::string(pid) + "/statm");
        if (stream.is_open())
        {
            buffer.clear();
            buffer << stream.rdbuf();
            stream.close();

            buffer >> vmSize >> vmRss >> shared >> trs >> lrs >> drs >>
                dirtyPages;

            info.vmSize = vmSize;
            info.vmRss = vmRss;
            info.trs = trs;
            info.drs = drs;
        }

        return info;
    }
} // namespace

namespace wnd
{
    wnd::Memory ProcessMemory::get_memory_for_process(std::string_view pid)
    {
        wnd::Memory memory = get_process_memory(pid);

#ifdef _SC_PAGESIZE
        const static int MEMORY_PAGE_SIZE = sysconf(_SC_PAGESIZE);
#elif _SC_PAGE_SIZE
        const static int MEMORY_PAGE_SIZE = sysconf(_SC_PAGE_SIZE);
#endif

        memory.vmSize = memory.vmSize * MEMORY_PAGE_SIZE;
        memory.vmRss = memory.vmRss * MEMORY_PAGE_SIZE;
        memory.drs = memory.drs * MEMORY_PAGE_SIZE;
        memory.trs = memory.trs * MEMORY_PAGE_SIZE;

        return memory;
    }
}; // namespace wnd
#endif
