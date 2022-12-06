#include "./get_childs.hpp"
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

    waybar::wnd::utils::ProcessChilds::Process find_process(std::string_view pid)
    {
        waybar::wnd::utils::ProcessChilds::Process process;

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

    void find_childs_for_process(std::string_view pid, std::vector<waybar::wnd::utils::ProcessChilds::Process>& ref)
    {
        std::string root = "/proc/";

        std::filesystem::path p(root);

        for(const auto& pathIterator : std::filesystem::directory_iterator(p))
        {
            std::string id = pathIterator.path().filename();
            waybar::wnd::utils::ProcessChilds::Process info = find_process(id);

            if(pid == info.ppid)
            {
                ref.push_back(info);
                find_childs_for_process(info.pid, info.child);
            }
        }
    }
}

namespace waybar::wnd::utils
{

    ProcessChilds::Process ProcessChilds::get_process(int pid)
    {
        return ProcessChilds::get_process(std::to_string(pid));
    }

    ProcessChilds::Process ProcessChilds::get_process(std::string_view pid)
    {
        waybar::wnd::utils::ProcessChilds::Process head = find_process(pid);
        find_childs_for_process(pid, head.child);
        return head;
    }
};
