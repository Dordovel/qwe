#include "./process.hpp"
#include <iostream>

#ifdef __linux__

#include <cstdio>
#include <sstream>
#include <filesystem>
#include <fstream>
#include <string>
#include <algorithm>
#include <string_view>
#include <utility>

#else

#include <kvm.h>
#include <sys/param.h>
#include <sys/sysctl.h>
#include <sys/user.h>
#include <paths.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define MEM_PATH "/dev/null"

#endif

namespace
{
#ifdef __linux__
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
#else
    waybar::wnd::utils::ProcessTree::Process create_process_struct(kinfo_proc* kp)
	{
		waybar::wnd::utils::ProcessTree::Process process;

		std::size_t page_size = getpagesize();

		process.name = kp->ki_comm;
		process.pid = std::to_string(static_cast<int>(kp->ki_pid));
		process.ppid = std::to_string(static_cast<int>(kp->ki_ppid));

		waybar::wnd::ProcessMemory::Memory memory = {0};
		memory.vmSize = kp->ki_size;
		memory.vmRss = kp->ki_rssize * page_size;
		memory.trs = kp->ki_tsize;
		memory.drs = kp->ki_dsize + kp->ki_ssize;

		process.memory = memory;

		return process;
	}

    waybar::wnd::utils::ProcessTree::Process find_process(std::string_view pid)
    {
	int pidi = std::stoi(std::string(pid));

	char errbuf[_POSIX2_LINE_MAX];
	kvm_t* kd = kvm_open(NULL, MEM_PATH, NULL, O_RDONLY, errbuf);
	if(NULL == kd)
	{
		std::cout<<"Init kvm_t error: "<<errbuf<<std::endl;
		return {0};
	}

	int cnt;
	kinfo_proc* kp = kvm_getprocs(kd, KERN_PROC_PID, pidi, &cnt);
	waybar::wnd::utils::ProcessTree::Process process = create_process_struct(kp);
	kvm_close(kd);

        return process;
    }

    void find_childs_for_process(std::string_view pid, kinfo_proc* kinfo, int cnt, std::vector<waybar::wnd::utils::ProcessTree::Process>& ref)
    {
	int pidi = std::stoi(std::string(pid));

	for(int i = 0; i < cnt; ++i)
	{
		kinfo_proc &kp = kinfo[i];
		if(kp.ki_ppid == pidi)
		{
			waybar::wnd::utils::ProcessTree::Process info = create_process_struct(&kp);
			find_childs_for_process(info.pid, kinfo, cnt, info.child);
			ref.push_back(info);
		}
	}
    }

    void find_childs_for_process(std::string_view pid, std::vector<waybar::wnd::utils::ProcessTree::Process>& ref)
    {
	char errbuf[_POSIX2_LINE_MAX] = {0};
	kvm_t* kd = kvm_open(NULL, MEM_PATH, NULL, O_RDONLY, errbuf);
	if(NULL == kd)
	{
		std::cout<<"Init kvm_t error: "<<errbuf<<std::endl;
		return;
	}

	int cnt;
	kinfo_proc* kinfo = kvm_getprocs(kd, KERN_PROC_PROC, 0, &cnt);

	find_childs_for_process(pid, kinfo, cnt, ref);

	kvm_close(kd);
    }
}
#endif


namespace waybar::wnd::utils
{
    ProcessTree::Process ProcessTree::get_tree_for_process(std::string_view pid)
    {
        waybar::wnd::utils::ProcessTree::Process head = find_process(pid);
        find_childs_for_process(pid, head.child);
        return head;
    }
};
