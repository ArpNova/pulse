#include "pulse/network.hpp"
#include "pulse/storage.hpp"
#include "pulse/utils.hpp"
#include <atomic>
#include <chrono>
#include <csignal>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>

std::atomic<bool> keepRunning(true);
std::atomic<bool> createdLockFile(false);

void handleExit([[maybe_unused]] int signum) {
  keepRunning = false;
  if (createdLockFile) {
    std::remove("/tmp/pulse.lock");
  }
}

int main(int argc, char *argv[]) {
  const std::string lockFilePath = "/tmp/pulse.lock";
  PulseConfig config = parseArguments(argc, argv);

  if (config.showHelp) {
    std::cout << "Pulse - Network Monitor\n";
    std::cout << "Usage: ./pulse [OPTIONS]\n";
    std::cout << "  -i, --interface <name>   Specify network interface\n";
    std::cout << "  -s, --stats              Show data usage history\n";
    std::cout << "  -b, --bits               Display output in bits (b/s)\n";
    std::cout << "  -d, --daemon             Run silently in the background\n";
    std::cout << "  -h, --help               Show this menu\n";
    return 0;
  }

  signal(SIGINT, handleExit);
  signal(SIGTERM, handleExit);

  StorageManager storage;
  storage.load();

  if (config.showStats) {
    if (storage.isEmpty()) {
      std::cout << "No data recorded yet. Run Pulse without flags to start "
                   "monitoring!\n";
    } else {
      storage.printStats(config.useBits);
    }
    return 0;
  }

  std::string interface;
  if (config.interface) {
    interface = *config.interface;
  } else {
    std::cout << "Scanning for active connections... \n";
    auto discovered = autoDiscoverInterface();
    if (discovered) {
      interface = *discovered;
      std::cout << "Auto-discovered: " << interface << "\n";
    } else {
      std::cerr << "Error: Could not find interface. Use -i to specify one.\n";
      return 1;
    }
  }

  auto previousRxOpt = getBytes(interface, "rx_bytes");
  auto previousTxOpt = getBytes(interface, "tx_bytes");

  if (!previousRxOpt || !previousTxOpt) {
    std::cerr << "Error: Could not find or read interface '"
              << interface << "'\n";
    return 1;
  }

  unsigned long long previousRxBytes = *previousRxOpt;
  unsigned long long previousTxBytes = *previousTxOpt;

  std::ifstream checkLock(lockFilePath);
  bool lockExists = checkLock.good();
  checkLock.close();

  if (lockExists) {
    if (config.runDaemon) {
      std::cerr << "Error: pulse is already running in the background.\n";
      return 1;
    } else {
      std::cout << "[Note] pulse instance detected in background\n";
      std::cout << "Starting in read-only viewer mode\n\n";
      config.isReadOnly = true;
    }
  } else {
    std::ofstream lockFile(lockFilePath);
    lockFile << "Pulse monitoring\n";
    lockFile.close();
    createdLockFile = true;
  }

  if (config.runDaemon) {
    std::cout << "Starting pulse in background daemon mode...\n";
    std::cout << "Monitoring interface: " << interface << "\n";
    std::cout << "To stop the background process later, run: killall pulse\n";

    if (daemon(0, 0) < 0) {
      std::cerr << "Error: Failed to start daemon mode.\n";
      if (createdLockFile) {
        std::remove(lockFilePath.c_str());
      }
      return 1;
    }
  } else {
    if (!config.isReadOnly) {
      std::cout << "Starting pulse... Monitoring "
                << interface << ".\nPress Ctrl+C to stop.\n\n";
    } else {
      std::cout << "Monitoring " << interface << " (Viewer mode).\n"
                << "Press Ctrl+C to stop.\n\n";
    }
  }

  unsigned long long totalRxSession = 0;
  unsigned long long totalTxSession = 0;

  int saveTimer = 0;

  auto lastTime = std::chrono::steady_clock::now();

  while (keepRunning) {
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto currentRxOpt = getBytes(interface, "rx_bytes");
    auto currentTxOpt = getBytes(interface, "tx_bytes");

    if (!currentRxOpt || !currentTxOpt) {
      std::cout << "\rError reading interface stats. Retrying...          "
                << std::flush;
      continue;
    }

    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = currentTime - lastTime;

    unsigned long long currentRxBytes = *currentRxOpt;
    unsigned long long currentTxBytes = *currentTxOpt;

    // handle potential counter wrap-around or reset
    unsigned long long rxDiff = (currentRxBytes >= previousRxBytes)
                                    ? (currentRxBytes - previousRxBytes)
                                    : currentRxBytes;
    unsigned long long txDiff = (currentTxBytes >= previousTxBytes)
                                    ? (currentTxBytes - previousTxBytes)
                                    : currentTxBytes;

    totalRxSession += rxDiff;
    totalTxSession += txDiff;

    unsigned long long speedRxBytes =
        static_cast<unsigned long long>(rxDiff / elapsed.count());
    unsigned long long speedTxBytes =
        static_cast<unsigned long long>(txDiff / elapsed.count());

    std::string currentHour = getCurrentHourKey();
    storage.addUsage(currentHour, rxDiff, txDiff);

    saveTimer++;
    if (saveTimer >= 60 && !config.isReadOnly) {
      storage.save();
      saveTimer = 0;
    }

    if (!config.runDaemon) {
      std::string speedRX = formatBytes(speedRxBytes, true, config.useBits);
      std::string speedTX = formatBytes(speedTxBytes, true, config.useBits);

      std::string totRx = formatBytes(totalRxSession, false, config.useBits);
      std::string totTx = formatBytes(totalTxSession, false, config.useBits);

      std::cout << "\rRx: " << speedRX << " (" << totRx
                << ") |  Tx: " << speedTX << " (" << totTx << ")          "
                << std::flush;
    }

    previousRxBytes = currentRxBytes;
    previousTxBytes = currentTxBytes;
    lastTime = currentTime;
  }

  std::cout << "\n\nSaving session data to disk... Goodbye!\n";

  if (!config.isReadOnly) {
    storage.save();
  }

  return 0;
}
