#include "display/display.hpp"
#include "utils/process.hpp"
#include <chrono>
#include <iostream>
#include <thread>

int main (int argc, char *argv[])
{
    wnd::display::Display display = wnd::display::Display("{app} %cpu {cpu} mem {vmRss} Mb");
    wnd::utils::ProcessTree processTree = wnd::utils::ProcessTree();

    while(true)
    {
        wnd::utils::ProcessTree::Process process = processTree.get_tree_for_process("198228");
        std::cout<< display.show(process)<<std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    return 0;
}
