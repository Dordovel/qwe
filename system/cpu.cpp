#include "cpu.hpp"
#include <cstdio>
#include <sstream>
#include <unistd.h>
#include <fstream>
#include <string>
#include <algorithm>

#ifdef __linux__

namespace wnd::system
{
    long Cpu::get_cpu_total()
    {
        std::ifstream stream("/proc/stat");

        if(stream.is_open())
        {
            std::string buffer;
            std::string type;
            unsigned value;
            unsigned total = 0;
            std::stringstream sBuffer;

            while(std::getline(stream, buffer))
            {
                sBuffer << buffer;
                sBuffer >> type;
                if(type == "cpu")
                {
                    for (; sBuffer >> value; total += value);
                    return total;
                }
            }

            stream.close();
        }

        return 0;
    }
};
#endif
