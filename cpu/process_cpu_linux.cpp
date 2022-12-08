#include "process_cpu_linux.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>

namespace wnd
{
    long ProcessCpu::get_cpu_for_process(std::string_view pid)
    {
        std::string p_tcomm, p_state;

        std::stringstream path;
        path << "/proc/" << pid << "/stat";

        std::ifstream stream(path.str());
        if(false == stream.is_open()) return 0;

        std::vector<std::string> params;
        params.reserve(55);

        std::stringstream sBuffer;
        sBuffer << stream.rdbuf();

        for(std::string param; sBuffer >> param; params.push_back(std::move(param)));

        return std::stol(params[14]) + std::stol(params[15]);
    }

};
