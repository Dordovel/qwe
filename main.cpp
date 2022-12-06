#include "memory/memory.hpp"
#include "utils/get_childs.hpp"
#include <chrono>
#include <iostream>
#include <thread>

int main (int argc, char *argv[])
{
    while(true)
        {
    waybar::wnd::ProcessMemoryInfoFormat memory = waybar::wnd::ProcessMemoryInfo::get_memory(1082);

    std::cout << memory(waybar::wnd::ProcessMemoryInfoFormat::Format::Mb)<<std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    return 0;
}
