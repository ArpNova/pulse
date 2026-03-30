#pragma once

#include <optional>
#include <string>

std::string formatBytes(unsigned long long bytes, bool isSpeed = true,
                        bool useBits = false);
std::string getCurrentHourKey();

struct PulseConfig {
  std::optional<std::string> interface = std::nullopt;
  bool showHelp = false;
  bool showStats = false;
  bool useBits = false;
};

PulseConfig parseArguments(int argc, char *argv[]);