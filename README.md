<h1 align="center">
  <br>
  pulse
  <br>
</h1>

<h4 align="center">A minimalist, native real-time network interface monitor for Linux.</h4>

<p align="center">
  <a href="https://github.com/arpnova/pulse/stargazers"><img src="https://img.shields.io/github/stars/arpnova/pulse?style=flat&color=white&labelColor=white&logo=github&logoColor=black" alt="Stars" /></a>
  <a href="https://github.com/arpnova/pulse/network/members"><img src="https://img.shields.io/github/forks/arpnova/pulse?style=flat&color=white&labelColor=white&logo=github&logoColor=black" alt="Forks" /></a>
  <a href="https://isocpp.org/"><img src="https://img.shields.io/badge/C++-17-00599C?style=flat&logo=c%2B%2B&logoColor=white" alt="C++17" /></a>
  <a href="https://www.linux.org/"><img src="https://img.shields.io/badge/Linux-FCC624?style=flat&logo=linux&logoColor=black" alt="Linux" /></a>
  <a href="https://github.com/arpnova/pulse/blob/main/LICENSE"><img src="https://img.shields.io/github/license/arpnova/pulse?style=flat&color=a6e22e" alt="License" /></a>
</p>

<p align="center">
  <a href="#overview">Overview</a> •
  <a href="#features">Features</a> •
  <a href="#installation">Installation</a> •
  <a href="#usage">Usage</a> •
  <a href="#license">License</a>
</p>

---

## 📌 Overview

**pulse** is a lightweight, efficient command-line tool written in C++ that continuously tracks and displays the Receive (Rx) and Transmit (Tx) speeds of a specified network interface. 

Built exclusively for Linux, it completely bypasses external dependencies by mapping directly into the kernel's strictly formatted SysFS (`/sys/class/net/`) packet statistics. This provides instantaneous and highly-accurate throughput measurements with a virtually non-existent CPU and memory footprint. 

---

## ✨ Features

* **Zero External Dependencies**: Operates natively reading direct kernel stats. No `libpcap` or third-party wrappers required.
* **Auto-Discovery**: Automatically scans and monitors the first active network hardware interface if no target is provided.
* **Smart Data Persistence**: Hourly downloaded and uploaded data is safely committed to a background atomic database (`~/.pulse_data.csv`).
* **Crash-Resilient Design**: Fault-tolerant implementations elegantly handle interface resets, integer wrap-arounds, and malformed files.
* **Real-Time Bandwidth**: Instant tracking of incoming (Rx) and outgoing (Tx) data speeds, dynamically scaling units (B/s to GB/s) for ultimate readability.
* **Long-Term Statistics**: Track your data bandwidth usage across your day, your month, and all-time utilizing the built-in analytical flags.
* **Graceful Termination**: Intercepts `SIGINT` (`Ctrl+C`) to cleanly finalize current monitoring sessions and commit final analytics before exiting.

---

## 🚀 Installation

### Option 1: One-Line Install (Recommended)

Install the pre-compiled binary directly to your system with a single command:

```bash
sudo wget -O /usr/local/bin/pulse https://github.com/arpnova/pulse/releases/download/v0.1.0/pulse && sudo chmod +x /usr/local/bin/pulse
```

Once installed, you can run `pulse` from anywhere in your terminal — no build step required.

### Option 2: Build from Source

**Prerequisites**
- Any modern Linux-based operating system.
- `g++` (Compiler supporting C++17).
- `make` (GNU Make).

1. Clone the repository:
   ```bash
   git clone https://github.com/arpnova/pulse.git
   cd pulse
   ```

2. Compile using `make`:
   ```bash
   make
   ```
   *This creates a `pulse` executable inside the `build/` directory.*

---

Run the compiled executable. If no specific arguments are provided, pulse will automatically locate and bind to your active network interface.

> **Note:** If you installed via the one-line installer (Option 1), use `pulse` directly. If you built from source (Option 2), use `./build/pulse`.

### Options

| Flag | Long Flag | Description |
| :--- | :--- | :--- |
| `-i` | `--interface <name>` | Manually specify a network interface to monitor |
| `-s` | `--stats` | Display aggregated data usage history |
| `-h` | `--help` | Display the application help menu |

### Command Examples

```bash
# ── If installed via one-line installer (Option 1) ──

# Auto-detect and monitor the active interface
pulse

# Specify a custom interface
pulse -i wlan0

# Show long-term data usage
pulse --stats

# Show help
pulse --help


# ── If built from source (Option 2) ──

./build/pulse
./build/pulse -i wlan0
./build/pulse --stats
./build/pulse --help
```

---

## 📊 Output Examples

### Live Monitoring Example
Tracking incoming data speeds in real-time.

```text
Scanning for active connections... 
Auto-discovered: wlan0
Starting pulse... Monitoring wlan0.
Press Ctrl+C to stop.

Rx: 1.25 MB/s (12.50 MB) |  Tx: 450.32 KB/s (4.50 MB)          
```

### Statistics Example (`pulse -s`)
Printing total historical bandwidth usage.

```text
 pulse Network Statistics 

Today (2026-03-28):
 DL: 12.50 MB | UL: 4.50 MB

This month (2026-03):
  DL: 45.20 GB  |  UL: 12.10 GB

All-Time:
  DL: 150.75 GB  |  UL: 42.80 GB
```

---

## 🏗️ Architecture

The codebase is strictly modularized for maintainability:

- `include/pulse/`
  - `network.hpp` - Hardware interface discovery and sysfs I/O bridging.
  - `storage.hpp` - Robust CSV persistence and statistical aggregation.
  - `utils.hpp` - Intelligent data formatting algorithms.
- `src/` 
  - `main.cpp` - Target execution lifecycle and runtime loop.
  - `network.cpp` - Implementation of the sysfs polling routines.
  - `storage.cpp` - Fault-tolerant atomic persistence implementation.
  - `utils.cpp` - Implementation of argument mapping and string parsers.

---

## 🧹 Maintenance

To cleanly purge the compiled binaries and reset the build environment, run:

```bash
make clean
```

---

## 📜 License

This project is officially licensed under the [MIT License](LICENSE). 

Copyright © 2026 Arpan Biswas.
