#include "display/display.hpp"
#include "utils/process.hpp"
#include "utils/process.hpp"
#include <chrono>
#include <iostream>
#include <thread>

int main (int argc, char *argv[])
{
    while(true)
    {
        waybar::wnd::utils::ProcessTree::Process processTree = waybar::wnd::utils::ProcessTree::get_tree_for_process("704");
        waybar::wnd::display::Display::show(processTree);
    }
    return 0;
}
