#include <chrono>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

std::string formatSpeed(long long bytes) {
  double speed = bytes;

  const std::string units[] = {"B/s", "KB/s", "MB/s", "GB/s"};
  int unitIndex = 0;

  while (speed >= 1024.0 && unitIndex < 3) {
    speed /= 1024.0;
    unitIndex++;
  }
  std::stringstream formattedSpeed;
  formattedSpeed << std::fixed << std::setprecision(2) << speed << " "
                 << units[unitIndex];
  return formattedSpeed.str();
}

long long getRxBytes(const std::string &interfaceName) {
  std::string path = "/sys/class/net/" + interfaceName + "/statistics/rx_bytes";
  std::ifstream file(path);
  std::string line;

  if (file.is_open()) {
    std::getline(file, line);
    file.close();
    return std::stoll(line);
  } else {
    std::cerr << "Error: Could not read from " << path << std::endl;
    return -1;
  }
}

int main(int argc, char *argv[]) {
  std::string interface = "wlan0";

  if (argc > 1) {
    interface = argv[1];
  } else {
    std::cout << "Tip: You can specify an interface (e.g., ./pulse eth0)\n";
  }

  long long previousBytes = getRxBytes(interface);
  if (previousBytes == -1)
    return 1;
  std::cout << "starting pulse...";

  while (true) {
    std::this_thread::sleep_for(std::chrono::seconds(1));

    long long currentBytes = getRxBytes(interface);
    long long speedBytes = currentBytes - previousBytes;

    std::string formattedSpeed = formatSpeed(speedBytes);

    std::cout << "\rcurrent download speed: " << formattedSpeed << "         "
              << std::flush;
    previousBytes = currentBytes;
  }

  return 0;
}