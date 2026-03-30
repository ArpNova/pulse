#pragma once
#include <map>
#include <string>

struct NetworkStats {
  unsigned long long rx = 0;
  unsigned long long tx = 0;
};

class StorageManager {
private:
  std::string dbPath;
  std::map<std::string, NetworkStats> usageData;

public:
  StorageManager();

  void load();
  void save();
  void addUsage(const std::string &timeKey, unsigned long long rx_bytes,
                unsigned long long tx_bytes);
  void printStats(bool useBits = false);
  bool isEmpty()const {return usageData.empty();}
};