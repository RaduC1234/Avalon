#include "Log.hpp"


std::shared_ptr<spdlog::logger> Log::coreLogger;
//std::shared_ptr<spdlog::logger> Log::clientLogger;

void Log::init() {

    // https://github.com/gabime/spdlog

    spdlog::set_pattern("%^[%T] %n: %v%$");

    coreLogger = spdlog::stdout_color_mt("AV");
    coreLogger->set_level(spdlog::level::trace);

    //clientLogger = spdlog::stdout_color_mt("APP");
    //clientLogger->set_level(spdlog::level::trace);
}

