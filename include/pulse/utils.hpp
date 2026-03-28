#pragma once

#include <string>
#include <optional>

std::string formatBytes(unsigned long long bytes, bool isSpeed = true);
std::string getCurrentHourKey();

struct PulseConfig{
    std::optional<std::string> interface = std::nullopt;
    bool showHelp = false;
    bool showStats = false;
};

PulseConfig parseArguments(int argc, char* argv[]);