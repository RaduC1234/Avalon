#ifndef AVALON_UTILS_HPP
#define AVALON_UTILS_HPP

#include <chrono>
#include <fstream>

class Time {
public:
    static std::chrono::time_point<std::chrono::high_resolution_clock> timeStarted;

    static void init() {
        timeStarted = std::chrono::high_resolution_clock::now();
    }

    static float getTime() {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = now - timeStarted;
        return elapsed.count();
    }
};

std::chrono::time_point<std::chrono::high_resolution_clock> Time::timeStarted = std::chrono::high_resolution_clock::now();

std::string readFile(const char* file_path) {
    std::ifstream file(file_path, std::ios::binary);
    std::string contents;
    file.seekg(0, std::ios::end);
    contents.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(&contents[0], contents.size());
    file.close();
    return contents;
}
#endif //AVALON_UTILS_HPP
