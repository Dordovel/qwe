#include "./process.hpp"

#ifdef __linux__

#include <cstdio>
#include <sstream>
#include <filesystem>
#include <fstream>
#include <string>
#include <algorithm>
#include <string_view>
#include <utility>
#include "../memory/process_memory_linux.hpp"
#include "../cpu/process_cpu_linux.hpp"

namespace
{
    static inline std::string ltrim(std::string s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));

        return s;
    }

    std::pair<std::string,std::string> parse_process(std::string_view line)
    {
        auto pos = line.find(":");
        if(std::string::npos == pos)
        {
            return {};
        }

        std::pair<std::string,std::string> result;

        std::string column = std::string(line.substr(0, pos));
        std::string value = ltrim(std::string(line.substr(pos + 1)));

        return std::make_pair(column, value);
    }

    wnd::utils::ProcessTree::Process read_process(std::string_view pid)
    {
        wnd::utils::ProcessTree::Process process;

        std::string root = "/proc/";
        std::ifstream stream(root + std::string(pid) + std::string("/status"));

        if(stream.is_open())
        {
            std::string buffer;

            while(std::getline(stream, buffer))
            {
                std::pair<std::string,std::string> line = parse_process(buffer);

                if("Name" == line.first)
                {
                    process.name = line.second;
                }
                else if("Pid" == line.first)
                {
                    process.pid = line.second;
                }
                else if("PPid" == line.first)
                {
                    process.ppid = line.second;
                    break;
                }
            }

            stream.close();
        }

        return process;
    }

    void find_childs_for_process(std::string_view pid,
                                 std::vector<wnd::utils::ProcessTree::Process>& processes,
                                 std::vector<wnd::utils::ProcessTree::Process>&& oldProcesses,
                                 std::vector<wnd::utils::ProcessTree::Process>& ref)
    {
        for(auto& process : processes)
        {
            if(process.ppid == pid)
            {
                find_childs_for_process(process.pid, processes, std::move(oldProcesses), process.child);
                process.memory = wnd::ProcessMemory::get_memory_for_process(process.pid);
                process.cpuTime = wnd::ProcessCpu::get_cpu_for_process(process.pid);
                
                auto oldState = std::find_if(oldProcesses.cbegin(), oldProcesses.cend(),
                                             [ &process ](const auto& old){return old.pid == process.pid;});

                if(oldState != oldProcesses.cend())
                {
                    process.cpuTime_old = oldState->cpuTime;
                    process.memory_old = std::move(oldState->memory);
                }
                
                ref.push_back(process);
            }
        }
    }

    std::vector<wnd::utils::ProcessTree::Process> get_all_processes()
    {
        std::vector<wnd::utils::ProcessTree::Process> processes;

        std::string root = "/proc/";

        std::filesystem::path p(root);

        for(const auto& pathIterator : std::filesystem::directory_iterator(p))
        {
            if(pathIterator.is_symlink()) continue;

            std::string id = pathIterator.path().filename();
            wnd::utils::ProcessTree::Process process = read_process(id);
            processes.push_back(std::move(process));
        }

        return processes;
    }
}

namespace wnd::utils
{
    ProcessTree::Process ProcessTree::get_tree_for_process(std::string_view pid)
    {
        std::vector<wnd::utils::ProcessTree::Process> processes = get_all_processes();
        
        wnd::utils::ProcessTree::Process head = read_process(pid);
        head.memory = wnd::ProcessMemory::get_memory_for_process(head.pid);
        head.cpuTime = wnd::ProcessCpu::get_cpu_for_process(head.pid);
        find_childs_for_process(pid, processes, std::move(this->_processes), head.child);

        this->_processes = std::move(processes);
        return head;
    }
};

#endif
