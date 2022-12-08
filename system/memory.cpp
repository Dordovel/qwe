#include "memory.hpp"
#include <cstdio>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <fstream>
#include <string>
#include <algorithm>

#ifdef __linux__

namespace wnd::system
{
    long Memory::get_total_memory()
    {
        std::ifstream stream("/proc/meminfo");

        if(! stream.is_open()) return 0;
        std::string buffer;
        std::string type;
        unsigned value;
        std::stringstream sBuffer;

        while(std::getline(stream, buffer))
        {
            sBuffer << buffer;
            sBuffer >> type >> value;
            sBuffer.clear();

            auto pos = type.find(":");
            if(std::string::npos == pos)
            {
                continue;
            }

            type = std::string(type.substr(0, pos));
            if(type == "MemTotal")
            {
                stream.close();
                return value;
            }
        }

        stream.close();

        return 0;
    }
};
#endif
