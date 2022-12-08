#include "process_memory_linux.hpp"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <fstream>
#include <string>

#ifdef __linux__

namespace
{

#ifdef _SC_PAGESIZE
        const static int PAGE_SIZE = sysconf(_SC_PAGESIZE);
#elif _SC_PAGE_SIZE
        const static int PAGE_SIZE = sysconf(_SC_PAGE_SIZE);
#endif

    wnd::Memory get_process_memory(std::string_view pid)
    {
        std::ifstream stream;
        wnd::Memory info = {0};
        std::stringstream buffer;

        stream.open("/proc/" + std::string(pid) + "/statm");
        if(stream.is_open())
        {
            buffer.clear();
            buffer << stream.rdbuf();
            stream.close();

            buffer >> info.vmSize >> info.vmRss >> info.shared >> info.trs >> info.lrs >> info.drs >> info.dirtyPages;
        }

        return info;
    }
}

namespace wnd
{
    wnd::Memory ProcessMemory::get_memory_for_process(std::string_view pid)
    {
        wnd::Memory memory = get_process_memory(pid);

        memory.vmSize = memory.vmSize * PAGE_SIZE;
        memory.vmRss = memory.vmRss * PAGE_SIZE;
        memory.drs = memory.drs * PAGE_SIZE;
        memory.trs = memory.trs * PAGE_SIZE;

        return memory;
    }
};
#endif
