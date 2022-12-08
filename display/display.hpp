#pragma once
#include "../utils/process.hpp"

namespace wnd::display
{
    class Display
    {
        private:
            long _cpuTick;

            void show(const wnd::utils::ProcessTree::Process& process, int depth);
        public:
            void show(const utils::ProcessTree::Process&);
    };
};
