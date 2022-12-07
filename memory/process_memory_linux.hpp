#pragma once
#include <string_view>
#include "../utils/memory.hpp"

namespace waybar::wnd
{
    class ProcessMemory
    {
        public:
            static waybar::wnd::Memory get_memory_for_process(std::string_view pid);
    };
};
