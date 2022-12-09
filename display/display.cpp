#include "./display.hpp"
#include <cmath>
#include <fmt/core.h>
#include <iostream>

constexpr float b_to_m = 1024 * 1024;

namespace
{
    void show(const wnd::utils::ProcessTree::Process& process, int depth, std::string_view format, std::string& out)
    {
        for(int i = 0; i < depth; ++i)
        {
            out += " ";
        }

        if(depth > 0)
        {
            out += "└";
        }

        out += fmt::format(format, fmt::arg("app", process.name),
                                                fmt::arg("pid", process.pid),
                                                fmt::arg("ppid", process.ppid),
                                                fmt::arg("vmRss", fmt::format("{:.2f}", process.memory.vmRss / b_to_m)),
                                                fmt::arg("vmSize", fmt::format("{:.2f}", process.memory.vmSize / b_to_m)),
                                                fmt::arg("trs", fmt::format("{:.2f}", process.memory.trs / b_to_m)),
                                                fmt::arg("drs", fmt::format("{:.2f}", process.memory.drs / b_to_m)),
                                                fmt::arg("cpu", fmt::format("{:.1f}", process.p_cpu)));
        out += "\n\r";

        for(const wnd::utils::ProcessTree::Process& var : process.child)
        {
            show(var, depth + 1, format, out);
        }
    }
};

namespace wnd::display
{
    Display::Display (std::string format):_format(std::move(format)){}

    std::string Display::show(const utils::ProcessTree::Process& process)
    {
        int depth = 0;
        std::string out;
        ::show(process, depth, this->_format, out);

        return out;
    }
};
