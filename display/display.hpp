#pragma once
#include "../utils/process.hpp"

namespace wnd::display
{
    class Display
    {
        private:
            std::string _format;
        public:
            Display(std::string format);
            std::string show(const utils::ProcessTree::Process&);
    };
};
