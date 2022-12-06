#include "memory.hpp"
#include <cmath>

namespace waybar::wnd
{
    constexpr float b_to_m = 1024 * 1024;
    constexpr float b_to_g = b_to_m * 1024;

    ProcessMemoryInfoFormat::ProcessMemoryInfoFormat(long memory): _memory(memory){}

    bool ProcessMemoryInfoFormat::is_valid(long value) const
    {
        return value > 0;
    }
    
    float ProcessMemoryInfoFormat::operator ()(ProcessMemoryInfoFormat::Format format) const
    {
        if(!this->is_valid(this->_memory)) return 0;

        switch (format)
        {
            case ProcessMemoryInfoFormat::Format::Gb:
                {
                    return std::round(this->_memory / (b_to_g));
                }
            break;

            case ProcessMemoryInfoFormat::Format::Mb:
                {
                    return std::round(this->_memory / (b_to_m));
                }
            break;

            case ProcessMemoryInfoFormat::Format::Kb:
                {
                    return std::round(this->_memory / 1024);
                }
            break;

            default:
                {
                    return this->_memory;
                }
            break;
        }
    }
};
