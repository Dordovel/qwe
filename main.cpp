#include "display/display.hpp"
#include "utils/process.hpp"
#include <chrono>
#include <iostream>
#include <thread>

int main (int argc, char *argv[])
{
    wnd::display::Display display = wnd::display::Display();
    wnd::utils::ProcessTree processTree = wnd::utils::ProcessTree();

    while(true)
    {
        wnd::utils::ProcessTree::Process process = processTree.get_tree_for_process("170852");
        display.show(process);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::system("clear");
    }
    return 0;
}
