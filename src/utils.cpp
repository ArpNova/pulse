#include "pulse/utils.hpp"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

std::string formatBytes(unsigned long long bytes, bool isSpeed) {
  double value = static_cast<double>(bytes);

  const std::string speedUnits[] = {"B/s", "KB/s", "MB/s", "GB/s"};
  const std::string volumeUnits[] = {"B", "KB", "MB", "GB"};
  int unitIndex = 0;

  while (value >= 1024.0 && unitIndex < 3) {
    value /= 1024.0;
    unitIndex++;
  }
  std::stringstream formattedOutput;
  formattedOutput << std::fixed << std::setprecision(2) << value << " "
                  << (isSpeed ? speedUnits[unitIndex] : volumeUnits[unitIndex]);
  return formattedOutput.str();
}

std::string getCurrentHourKey() {
  auto now = std::chrono::system_clock::now();
  std::time_t time_now = std::chrono::system_clock::to_time_t(now);

  std::stringstream ss;
  ss << std::put_time(std::localtime(&time_now), "%Y-%m-%d-%H");
  return ss.str();
}