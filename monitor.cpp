#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>

long long getRxBytes(const std::string &interfacename)
{
    std::string path = "/sys/class/net/" + interfacename + "/statistics/rx_bytes";
    std::ifstream file(path);
    std::string line;

    if (file.is_open())
    {
        std::getline(file, line);
        file.close();
        return std::stoll(line);
    }
    else
    {
        std::cerr << "Error: Could not read from " << path << std::endl;
        return -1;
    }
}

int main(int argc, char *argv[])
{
    std::string interface = "wlan0";

    if (argc > 1)
    {
        interface = argv[1];
    }
    else
    {
        std::cout << "Tip: You can specify an interface (e.g., ./pulse eth0)\n";
    }

    long long previousBytes = getRxBytes(interface);
    if (previousBytes == -1)
        return 1;
    std::cout << "starting pulse...";

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        long long currentBytes = getRxBytes(interface);
        long long speedBytes = currentBytes - previousBytes;

        std::cout << "\rcurrent download speed: " << speedBytes << "Bytes/s         " << std::flush;
        previousBytes = currentBytes;
    }

    return 0;
}