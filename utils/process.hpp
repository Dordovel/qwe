#pragma once

#include <string>
#include <string_view>
#include <vector>
#include "memory.hpp"

namespace wnd::utils
{
    class ProcessTree
    {
        public:
            struct Process
            {
                std::string name;
                std::string pid;
                std::string ppid;

                Memory memory;
                Memory memory_old;

                long cpuTime;
                long cpuTime_old;

                std::vector<Process> child;
            };
            Process get_tree_for_process(std::string_view pid);

        private:
            std::vector<struct Process> _processes;
    };
};
