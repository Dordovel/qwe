#pragma once

#include <string>
#include <string_view>
#include <vector>
#include "../memory/memory.hpp"

namespace waybar::wnd::utils
{
    class ProcessTree
    {
        public:
            struct Process
            {
                std::string name;
                std::string pid;
                std::string ppid;

                ProcessMemory::Memory memory;

                std::vector<Process> child;
            };
            static Process get_tree_for_process(std::string_view pid);
    };
};
