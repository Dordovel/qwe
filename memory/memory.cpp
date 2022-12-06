#include "memory.hpp"
#include "../utils/get_childs.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <unistd.h>
#include <vector>
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
    waybar::wnd::ProcessMemoryInfo::Memory get_process_memory(std::string_view pid)
    {
        std::ifstream stream;
        waybar::wnd::ProcessMemoryInfo::Memory info;
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

    void accumulate_memory(const waybar::wnd::utils::ProcessChilds::Process& process, waybar::wnd::ProcessMemoryInfo::Memory& total)
    {
        waybar::wnd::ProcessMemoryInfo::Memory memory = get_process_memory(process.pid);

        total.vmSize += memory.vmSize;
        total.vmRss += memory.vmRss;
        total.shared += memory.shared;
        total.trs += memory.trs;
        total.drs += memory.drs;
        total.dirtyPages += memory.dirtyPages;

        for(const waybar::wnd::utils::ProcessChilds::Process& var : process.child)
        {
            accumulate_memory(var, total);
        }
    }

    waybar::wnd::ProcessMemoryInfo::Memory accumulate_memory(const waybar::wnd::utils::ProcessChilds::Process& process)
    {
        waybar::wnd::ProcessMemoryInfo::Memory total = {};

        accumulate_memory(process, total);

        return total;
    }
}

namespace waybar::wnd
{
    ProcessMemoryInfoFormat ProcessMemoryInfo::get_memory(int pid)
    {
        utils::ProcessChilds::Process process = utils::ProcessChilds::get_process(pid);
        
        if(process.child.empty()) return ProcessMemoryInfoFormat(0);

        waybar::wnd::ProcessMemoryInfo::Memory total = accumulate_memory(process);

        return ProcessMemoryInfoFormat(total.vmRss * PAGE_SIZE);
    }
};
