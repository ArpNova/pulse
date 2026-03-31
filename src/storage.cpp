#include "pulse/storage.hpp"
#include "pulse/utils.hpp"
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>

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
        usageData[timeKey].rx += std::stoull(rxStr);
        usageData[timeKey].tx += std::stoull(txStr);
      } catch (const std::exception &) {
        std::cerr << "Warning: Skipping corrupted data line in database.\n";
      }
    }
  }
}

void StorageManager::save() {
  std::string tmpPath = dbPath + ".tmp";
  FILE *file = std::fopen(tmpPath.c_str(), "w");

  if (!file) {
    std::cerr << "\nError: Could not save data to " << tmpPath << "\n";
    return;
  }

  for (const auto &pair : usageData) {
    std::fprintf(file, "%s,%llu,%llu\n", pair.first.c_str(), pair.second.rx,
                 pair.second.tx);
  }

  std::fflush(file);
  fsync(fileno(file));
  std::fclose(file);

  std::rename(tmpPath.c_str(), dbPath.c_str());
}

void StorageManager::addUsage(const std::string &timeKey,
                              unsigned long long rx_bytes,
                              unsigned long long tx_bytes) {
  usageData[timeKey].rx += rx_bytes;
  usageData[timeKey].tx += tx_bytes;
}

void StorageManager::printStats(bool useBits) {
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

    if (timeKey.size() >= 10 && timeKey.substr(0, 10) == todayPrefix) {
      today.rx += stats.rx;
      today.tx += stats.tx;
    }
    if (timeKey.size() >= 7 && timeKey.substr(0, 7) == monthPrefix) {
      month.rx += stats.rx;
      month.tx += stats.tx;
    }
  }

  std::cout << "\n pulse Network Statistics \n\n";
  std::cout << "Today (" << todayPrefix << "):\n";
  std::cout << " DL: " << formatBytes(today.rx, false, useBits)
            << " | UL: " << formatBytes(today.tx, false, useBits) << "\n\n";

  std::cout << "This month (" << monthPrefix << "):\n";
  std::cout << "  DL: " << formatBytes(month.rx, false, useBits)
            << "  |  UL: " << formatBytes(month.tx, false, useBits) << "\n\n";

  std::cout << "All-Time:\n";
  std::cout << "  DL: " << formatBytes(allTime.rx, false, useBits)
            << "  |  UL: " << formatBytes(allTime.tx, false, useBits) << "\n\n";
}