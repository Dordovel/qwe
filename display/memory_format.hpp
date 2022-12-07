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
};
