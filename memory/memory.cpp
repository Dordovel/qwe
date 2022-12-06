#include "memory.hpp"
#include <sstream>
#include <unistd.h>
#include <fstream>

namespace {
#ifdef _SC_PAGESIZE
        const static int PAGE_SIZE = sysconf(_SC_PAGESIZE);
#elif _SC_PAGE_SIZE
        const static int PAGE_SIZE = sysconf(_SC_PAGE_SIZE);
#endif
};

namespace 
{
    waybar::wnd::ProcessMemory::Memory get_process_memory(std::string_view pid)
    {
        std::ifstream stream;
        waybar::wnd::ProcessMemory::Memory info;
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

namespace waybar::wnd
{
    waybar::wnd::ProcessMemory::Memory ProcessMemory::get_memory_for_process(std::string_view pid)
    {
        waybar::wnd::ProcessMemory::Memory memory = get_process_memory(pid);

        memory.vmSize = memory.vmSize * PAGE_SIZE;
        memory.vmRss = memory.vmRss * PAGE_SIZE;
        memory.drs = memory.drs * PAGE_SIZE;
        memory.trs = memory.trs * PAGE_SIZE;

        return memory;
    }
};
