#include "pulse/utils.hpp"
#include <iomanip>
#include <sstream>

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
