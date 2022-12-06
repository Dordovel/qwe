#pragma once
#include "../utils/process.hpp"

namespace waybar::wnd::display
{
    class Display
    {
        public:
            static void show(const utils::ProcessTree::Process&);
    };
};
