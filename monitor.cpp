#include <chrono>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <optional>

std::string formatSpeed(unsigned long long bytes) {
  double speed = static_cast<double>(bytes);

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

std::optional<unsigned long long> getBytes(const std::string &interfaceName, const std::string &type) {
  std::string path = "/sys/class/net/" + interfaceName + "/statistics/" + type;
  std::ifstream file(path);
  std::string line;

  if (file.is_open() && std::getline(file, line)) {
    try {
      return std::stoull(line);
    } catch (const std::exception&) {
      // handles std::invalid_argument and std::out_of_range
      return std::nullopt;
    }
  }
  return std::nullopt;
}

int main(int argc, char *argv[]) {
  std::string interface = "wlan0";

  if (argc > 1) {
    interface = argv[1];
  } else {
    std::cout << "Tip: You can specify an interface (e.g., ./pulse eth0)\n";
  }

  auto previousRxOpt = getBytes(interface, "rx_bytes");
  auto previousTxOpt = getBytes(interface, "tx_bytes");

  if (!previousRxOpt || !previousTxOpt) {
    std::cerr << "Error: Could not find or read interface '" << interface << "'\n";
    return 1;
  }

  unsigned long long previousRxBytes = *previousRxOpt;
  unsigned long long previousTxBytes = *previousTxOpt;

  std::cout << "Starting pulse... Monitoring " << interface << ".\nPress Ctrl+C to stop.\n\n";

  auto lastTime = std::chrono::steady_clock::now();

  while (true) {
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto currentRxOpt = getBytes(interface, "rx_bytes");
    auto currentTxOpt = getBytes(interface, "tx_bytes");

    if (!currentRxOpt || !currentTxOpt) {
      std::cout << "\rError reading interface stats. Retrying...          " << std::flush;
      continue;
    }

    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = currentTime - lastTime;

    unsigned long long currentRxBytes = *currentRxOpt;
    unsigned long long currentTxBytes = *currentTxOpt;
    
    // handle potential counter wrap-around or reset
    unsigned long long rxDiff = (currentRxBytes >= previousRxBytes) ? (currentRxBytes - previousRxBytes) : currentRxBytes;
    unsigned long long txDiff = (currentTxBytes >= previousTxBytes) ? (currentTxBytes - previousTxBytes) : currentTxBytes;

    unsigned long long speedRxBytes = static_cast<unsigned long long>(rxDiff / elapsed.count());
    unsigned long long speedTxBytes = static_cast<unsigned long long>(txDiff / elapsed.count());

    std::string formattedRx = formatSpeed(speedRxBytes);
    std::string formattedTx = formatSpeed(speedTxBytes);

    std::cout << "\rRx: " << formattedRx << "  |  Tx: " << formattedTx << "          " << std::flush;

    previousRxBytes = currentRxBytes;
    previousTxBytes = currentTxBytes;
    lastTime = currentTime;
  }

  return 0;
}