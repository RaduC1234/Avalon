#pragma once

#include "avalon/core/Core.hpp"

class FileDialogs {

    static std::string openFile(const char *filter);
    static std::string saveFile(const char* filter);
};


class Time {
public:

    static double getTime();
};
