#pragma once

#include <optional>
#include <string>

std::optional<std::string> autoDiscoverInterface();
std::optional<unsigned long long> getBytes(const std::string &interfaceName, const std::string &type);
