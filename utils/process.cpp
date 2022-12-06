#include "./process.hpp"
#include <cstdio>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <fstream>
#include <string>
#include <algorithm>
#include <string_view>
#include <utility>

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

    waybar::wnd::utils::ProcessTree::Process find_process(std::string_view pid)
    {
        waybar::wnd::utils::ProcessTree::Process process;

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
                    process.memory = waybar::wnd::ProcessMemory::get_memory_for_process(process.pid);
                    break;
                }
            }

            stream.close();
        }

        return process;
    }

    void find_childs_for_process(std::string_view pid, std::vector<waybar::wnd::utils::ProcessTree::Process>& ref)
    {
        std::string root = "/proc/";

        std::filesystem::path p(root);

        for(const auto& pathIterator : std::filesystem::directory_iterator(p))
        {
            std::string id = pathIterator.path().filename();
            waybar::wnd::utils::ProcessTree::Process info = find_process(id);

            if(pid == info.ppid)
            {
                find_childs_for_process(info.pid, info.child);
                ref.push_back(info);
            }
        }
    }
}

namespace waybar::wnd::utils
{
    ProcessTree::Process ProcessTree::get_tree_for_process(std::string_view pid)
    {
        waybar::wnd::utils::ProcessTree::Process head = find_process(pid);
        find_childs_for_process(pid, head.child);
        return head;
    }
};
