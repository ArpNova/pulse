#include "pulse/network.hpp"
#include "pulse/utils.hpp"
#include <chrono>
#include <iostream>
#include <thread>

int main(int argc, char *argv[]) {
  std::string interface;

  if (argc > 1) {
    interface = argv[1];
  } else {
    std::cout << "No interface specified. Scanning for active connections...\n";
    auto discovered = autoDiscoverInterface();

    if(discovered){
      interface = *discovered;
      std::cout << "Auto-discovered active interface: " << interface << "\n";
    }else{
      std::cerr << "Error: Could not automatically find an active interface (is your Wi-Fi connected?).\n";
      std::cerr << "Tip: You can specify one manually (e.g., ./pulse wlan0)\n";
      return 1;
    }
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

  unsigned long long totalRxSession = 0;
  unsigned long long totalTxSession = 0;

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

    totalRxSession += rxDiff;
    totalTxSession += txDiff;

    unsigned long long speedRxBytes = static_cast<unsigned long long>(rxDiff / elapsed.count());
    unsigned long long speedTxBytes = static_cast<unsigned long long>(txDiff / elapsed.count());

    std::string speedRX = formatBytes(speedRxBytes, true);
    std::string speedTX = formatBytes(speedTxBytes, true);

    std::string totRx = formatBytes(totalRxSession, false);
    std::string totTx = formatBytes(totalTxSession, false);

    std::cout << "\rRx: " << speedRX << " ("<<totRx << ") |  Tx: " << speedTX << " ("<<totTx << ")          " << std::flush;

    previousRxBytes = currentRxBytes;
    previousTxBytes = currentTxBytes;
    lastTime = currentTime;
  }

  return 0;
}
