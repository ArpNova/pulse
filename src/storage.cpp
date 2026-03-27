#include "pulse/storage.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

StorageManager::StorageManager() {
  const char *homeDir = std::getenv("HOME");
  if (homeDir) {
    dbPath = std::string(homeDir) + "/.pulse_data.csv";
  } else {
    dbPath = ".pulse_data.csv";
  }
}

void StorageManager::load() {
  std::ifstream file(dbPath);
  std::string line;

  if (!file.is_open())
    return;

  while (std::getline(file, line)) {
    std::stringstream ss(line);
    std::string timeKey, rxStr, txStr;

    if (std::getline(ss, timeKey, ',') && std::getline(ss, rxStr, ',') &&
        std::getline(ss, txStr, ',')) {
      usageData[timeKey].rx = std::stoull(rxStr);
      usageData[timeKey].tx = std::stoull(txStr);
    }
  }
}

void StorageManager::save() {
  std::ofstream file(dbPath);
  if (!file.is_open()) {
    std::cerr << "\nError: Could not save data to " << dbPath << "\n";
    return;
  }

  for (const auto &pair : usageData) {
    file << pair.first << "," << pair.second.rx << "," << pair.second.tx
         << "\n";
  }
}

void StorageManager::addUsage(const std::string &timeKey,
                              unsigned long long rx_bytes,
                              unsigned long long tx_bytes) {
  usageData[timeKey].rx += rx_bytes;
  usageData[timeKey].tx += tx_bytes;
}