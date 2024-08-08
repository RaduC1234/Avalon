#pragma once

#include "Core.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

class Log {

private:
    static std::shared_ptr<spdlog::logger> coreLogger;
    //static std::shared_ptr<spdlog::logger> clientLogger;

public:
    static void init();

    inline static std::shared_ptr<spdlog::logger> &getCoreLogger() { return coreLogger; }

    //inline static std::shared_ptr<spdlog::logger> &getClientLogger() { return clientLogger; }

};


// core log macros
#define AV_CORE_FATAL(...) {::Log::getCoreLogger()->error(__VA_ARGS__); exit(1); }
#define AV_CORE_ERROR(...) ::Log::getCoreLogger()->error(__VA_ARGS__)
#define AV_CORE_WARN(...)  ::Log::getCoreLogger()->warn(__VA_ARGS__)
#define AV_CORE_INFO(...)  ::Log::getCoreLogger()->info(__VA_ARGS__)
#define AV_CORE_TRACE(...) ::Log::getCoreLogger()->trace(__VA_ARGS__)

/*
// client log macros
#define AV_FATAL(...) ::Log::getClientLogger()->fatal(__VA_ARGS__)
#define AV_ERROR(...) ::Log::getClientLogger()->error(__VA_ARGS__)
#define AV_WARN(...)  ::Log::getClientLogger()->warn(__VA_ARGS__)
#define AV_INFO(...)  ::Log::getClientLogger()->info(__VA_ARGS__)
#define AV_TRACE(...) ::Log::getClientLogger()->trace(__VA_ARGS__)
*/
