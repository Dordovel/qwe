#pragma once
#include <string_view>

namespace waybar::wnd
{
    class ProcessMemoryInfoFormat
    {
        public:
            enum class Format {Mb, Gb, Kb};
            static bool is_valid(long value);
            static float format(long value, ProcessMemoryInfoFormat::Format format);
    };

    class ProcessMemory
    {
        public:
            struct Memory
            {
                long vmSize;
                long vmRss;
                long shared;
                long trs;
                long lrs;
                long drs;
                long dirtyPages;
            };
            static waybar::wnd::ProcessMemory::Memory get_memory_for_process(std::string_view pid);
    };
};
