#pragma once

namespace wnd
{
    struct Memory
    {
        long vmSize;
        long vmRss;
        long shared;
        long trs;
        long lrs;
        long drs;
        long dirtyPages;
    };
};
