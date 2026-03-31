#include "pulse/utils.hpp"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

std::string formatBytes(unsigned long long bytes, bool isSpeed, bool useBits) {
  double value = static_cast<double>(bytes);

  if (useBits) {
    value *= 8;
  }

  const std::string speedBytes[] = {"B/s", "KB/s", "MB/s", "GB/s"};
  const std::string volumeBytes[] = {"B", "KB", "MB", "GB"};

  const std::string speedBits[] = {"b/s", "Kb/s", "Mb/s", "Gb/s"};
  const std::string volumeBits[] = {"b", "Kb", "Mb", "Gb"};
  int unitIndex = 0;

  double divisor = useBits ? 1000.0 : 1024.0;

  while (value >= divisor && unitIndex < 3) {
    value /= divisor;
    unitIndex++;
  }
  std::stringstream formattedOutput;
  formattedOutput << std::fixed << std::setprecision(2) << value << " ";

  if (useBits) {
    formattedOutput << (isSpeed ? speedBits[unitIndex] : volumeBits[unitIndex]);
  } else {
    formattedOutput << (isSpeed ? speedBytes[unitIndex]
                                : volumeBytes[unitIndex]);
  }

  return formattedOutput.str();
}

std::string getCurrentHourKey() {
  auto now = std::chrono::system_clock::now();
  std::time_t time_now = std::chrono::system_clock::to_time_t(now);

  std::stringstream ss;
  ss << std::put_time(std::localtime(&time_now), "%Y-%m-%d-%H");
  return ss.str();
}

PulseConfig parseArguments(int argc, char **argv) {
  PulseConfig config;

  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];

    if (arg == "-s" || arg == "--stats") {
      config.showStats = true;
    } else if (arg == "-b" || arg == "--bits") {
      config.useBits = true;
    } else if (arg == "-d" || arg == "--daemon") {
      config.runDaemon = true;
    } else if (arg == "-h" || arg == "--help") {
      config.showHelp = true;
    } else if (arg == "-i" || arg == "--interface") {
      if (i + 1 < argc) {
        config.interface = argv[++i];
      } else {
        std::cerr << "Error: -i requires a network interface name.\n";
        exit(1);
      }
    } else if (arg[0] != '-') {
      config.interface = arg;
    } else {
      std::cerr << "Warning: Unknown flag ignored -> " << arg << "\n";
    }
  }
  return config;
}