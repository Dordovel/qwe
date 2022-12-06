#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace waybar::wnd::utils
{
    class ProcessChilds
    {
        public:
            struct Process
            {
                std::string name;
                std::string pid;
                std::string ppid;
                std::vector<Process> child;
            };
            static Process get_process(std::string_view pid);
            static Process get_process(int pid);
    };
};
