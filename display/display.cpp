#include "./display.hpp"
#include <cmath>
#include <ios>
#include <iostream>
#include "memory_format.hpp"

namespace wnd::display
{
    void Display::show(const wnd::utils::ProcessTree::Process& process, int depth)
    {
        for(int i = 0; i < depth; ++i)
        {
            std::cout<<" ";
        }

        if(depth > 0)
        {
            std::cout<<"└";
        }

        std::cout << process.pid << " "
                    << process.name
                    << " Mem: "
                    << wnd::ProcessMemoryInfoFormat::format(process.memory.vmRss, wnd::ProcessMemoryInfoFormat::Format::Mb)
                    << " Mb "
                    << " %Cpu "
                    <<process.p_cpu
                    << std::endl;

        for(const wnd::utils::ProcessTree::Process& var : process.child)
        {
            show(var, depth + 1);
        }
    }

    void Display::show(const utils::ProcessTree::Process& process)
    {
        int depth = 0;
        show(process, depth);
    }
};
