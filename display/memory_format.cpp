#include "memory_format.hpp"
#include <cmath>

namespace waybar::wnd
{
    constexpr float b_to_m = 1024 * 1024;
    constexpr float b_to_g = b_to_m * 1024;

    bool ProcessMemoryInfoFormat::is_valid(long value)
    {
        return value > 0;
    }
    
    float ProcessMemoryInfoFormat::format(long value, ProcessMemoryInfoFormat::Format format)
    {
        if(!ProcessMemoryInfoFormat::is_valid(value)) return 0;

        switch (format)
        {
            case ProcessMemoryInfoFormat::Format::Gb:
                {
                    return std::round((value / b_to_g) * 100) / 100;
                }
            break;

            case ProcessMemoryInfoFormat::Format::Mb:
                {
                    return std::round((value / b_to_m) * 100) / 100;
                }
            break;

            case ProcessMemoryInfoFormat::Format::Kb:
                {
                    return std::round((value / 1024.f) * 100) / 100;
                }
            break;

            default:
                {
                    return 0;
                }
            break;
        }
    }
};
