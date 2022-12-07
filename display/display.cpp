#include "./display.hpp"
#include <ios>
#include <iostream>
#include "memory_format.hpp"

namespace 
{
    void show(const waybar::wnd::utils::ProcessTree::Process& process, int depth)
    {
        for(int i = 0; i < depth; ++i)
        {
            std::cout<<" ";
        }

        if(depth > 0)
        {
            std::cout<<"└";
        }

        std::cout<<process.pid<< " "<<process.name<< " Mem: "<< waybar::wnd::ProcessMemoryInfoFormat::format(process.memory.vmRss, waybar::wnd::ProcessMemoryInfoFormat::Format::Mb) << " Mb"<<std::endl;

        for(const waybar::wnd::utils::ProcessTree::Process& var : process.child)
        {
            show(var, depth + 1);
			depth = 0;
        }
    }
};

namespace waybar::wnd::display
{
    void Display::show(const utils::ProcessTree::Process & process)
    {
        int depth = 0;
        ::show(process, depth);
    }
};
