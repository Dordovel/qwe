#pragma once

#include <fstream>

namespace waybar::wnd
{
    class ProcessMemoryInfoFormat
    {
        private:
            long _memory;
        public:
            enum class Format {Mb, Gb, Kb};
            ProcessMemoryInfoFormat(long memory);
            bool is_valid(long value) const;
            float operator ()(ProcessMemoryInfoFormat::Format format) const;
    };

    class ProcessMemoryInfo
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
            static ProcessMemoryInfoFormat get_memory(int pid);
    };
};
