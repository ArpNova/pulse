#include "pulse/storage.hpp"
#include "pulse/utils.hpp"
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <exception>
#include <fstream>
#include <iomanip>
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
    if (line.empty())
      continue;

    std::stringstream ss(line);
    std::string timeKey, rxStr, txStr;

    if (std::getline(ss, timeKey, ',') && std::getline(ss, rxStr, ',') &&
        std::getline(ss, txStr)) {
      try {
        usageData[timeKey].rx = std::stoull(rxStr);
        usageData[timeKey].tx = std::stoull(txStr);
      } catch (const std::exception &) {
        std::cerr << "Warning: Skipping corrupted data line in database.\n";
      }
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

void StorageManager::printStats() {
  auto now = std::chrono::system_clock::now();
  std::time_t time_now = std::chrono::system_clock::to_time_t(now);

  std::stringstream ssDay, ssMonth;
  ssDay << std::put_time(std::localtime(&time_now), "%Y-%m-%d");
  ssMonth << std::put_time(std::localtime(&time_now), "%Y-%m");

  std::string todayPrefix = ssDay.str();
  std::string monthPrefix = ssMonth.str();

  NetworkStats today{0, 0};
  NetworkStats month{0, 0};
  NetworkStats allTime{0, 0};

  for (const auto &pair : usageData) {
    std::string timeKey = pair.first;
    NetworkStats stats = pair.second;

    allTime.rx += stats.rx;
    allTime.tx += stats.tx;

    if (timeKey.substr(0, 10) == todayPrefix) {
      today.rx += stats.rx;
      today.tx += stats.tx;
    }
    if (timeKey.substr(0, 7) == monthPrefix) {
      month.rx += stats.rx;
      month.tx += stats.tx;
    }
  }

  std::cout << "\n pulse Network Statistics \n\n";
  std::cout << "Today (" << todayPrefix << "):\n";
  std::cout << " DL: " << formatBytes(today.rx, false)
            << " | UL: " << formatBytes(today.tx, false) << "\n\n";

  std::cout << "This month (" << monthPrefix << "):\n";
  std::cout << "  DL: " << formatBytes(month.rx, false)
            << "  |  UL: " << formatBytes(month.tx, false) << "\n\n";

  std::cout << "All-Time:\n";
  std::cout << "  DL: " << formatBytes(allTime.rx, false)
            << "  |  UL: " << formatBytes(allTime.tx, false) << "\n\n";
}