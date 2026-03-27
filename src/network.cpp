#include "pulse/network.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>

std::optional<std::string> autoDiscoverInterface(){
  std::string netPath = "/sys/class/net";

  try{
    for(const auto& entry : std::filesystem::directory_iterator(netPath)){
      std::string ifaceName = entry.path().filename().string();

      if(ifaceName == "lo"){
        continue;
      }

      std::string statePath = entry.path().string() + "/operstate";
      std::ifstream stateFile(statePath);
      std::string state;

      if(stateFile.is_open() && std::getline(stateFile, state)){
        if(state == "up"){
          return ifaceName;
        }
      }
    }
  }catch(const std::filesystem::filesystem_error& e){
    std::cerr << "Error scaning network interface: " << e.what() << "\n";
  }
  return std::nullopt;
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
