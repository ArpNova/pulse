<h1 align="center">
  <br>
  pulse
  <br>
</h1>

<h4 align="center">A minimalist, native real-time network interface monitor for Linux.</h4>

<p align="center">
  <a href="https://github.com/arpnova/pulse/stargazers"><img src="https://img.shields.io/github/stars/arpnova/pulse?style=flat-square&color=white&labelColor=white&logo=github&logoColor=black" alt="Stars" /></a>
  <a href="https://github.com/arpnova/pulse/network/members"><img src="https://img.shields.io/github/forks/arpnova/pulse?style=flat-square&color=white&labelColor=white&logo=github&logoColor=black" alt="Forks" /></a>
  <a href="https://isocpp.org/"><img src="https://img.shields.io/badge/C++-17-00599C?style=flat-square&logo=c%2B%2B&logoColor=white" alt="C++17" /></a>
  <a href="https://www.linux.org/"><img src="https://img.shields.io/badge/Linux-FCC624?style=flat-square&logo=linux&logoColor=black" alt="Linux" /></a>
  <a href="https://github.com/arpnova/pulse/blob/main/LICENSE"><img src="https://img.shields.io/github/license/arpnova/pulse?style=flat-square&color=a6e22e" alt="License" /></a>
  <a href="https://github.com/arpnova/pulse/releases/latest"><img src="https://img.shields.io/github/v/release/arpnova/pulse?style=flat-square" alt="Release" /></a>
</p>

<p align="center">
  <a href="#overview">Overview</a> •
  <a href="#features">Features</a> •
  <a href="#installation">Installation</a> •
  <a href="#usage">Usage</a> •
  <a href="#architecture">Architecture</a> •
  <a href="#license">License</a>
</p>

---

## 📌 Overview

**pulse** is a lightweight, strictly native, command-line tool built in C++ that effortlessly tracks and displays real-time Receive (Rx) and Transmit (Tx) bandwidth for Linux network interfaces.

Bypassing bloatware and external dependencies entirely, pulse maps directly into the Linux kernel's SysFS (`/sys/class/net/`) architecture to derive zero-overhead packet statistics. Designed for stability and endurance, `pulse` acts as a highly accurate data monitor that you can confidently leave running in the background.

---

## ✨ Key Features

- **Zero External Dependencies**: Operates natively reading direct kernel stats. No `libpcap`, external wrappers, or heavy runtimes are required.
- **Universal Static Binary**: Shipped as a fully statically linked executable guaranteeing bulletproof compatibility out-of-the-box across all Linux distributions.
- **Smart Auto-Discovery**: Automatically scans and binds to physical network hardware, intelligently filtering out virtual routing bridges like Docker, VMs, or VPNs when no specific target is provided.
- **Daemon Viewer Concurrency**: Run `pulse -d` to track statistics silently in the background. Launching standard `pulse` natively detects the background POSIX `flock()` lockfile and smoothly drops into a realtime Read-Only GUI viewer to track metrics without database clashing.
- **Crash-Resilient Durability**: Hourly data bandwidth is physically flushed to disk (`fsync()`) resolving to an atomic backup database (`~/.pulse_data.csv`).
- **Signal-Safe Mathematics**: Implements advanced threshold bounding to recover phantom bytes lost during kernel integer bounds wrapping, backed by async-signal-safe POSIX routines guaranteeing flawless operation through `kill -9` or connection resets.
- **Dynamic Scale Formatting**: Tracks usage dynamically from basic `B/s` all the way to `GB/s` as well as interchangeable metric flags (`-b`) for bits visualization (`b/s` to `Gb/s`).

---

## 🚀 Installation

### Option 1: One-Line Install (Recommended)

The easiest way to get started is by pulling the latest pre-compiled production binary straight to your environment.

```bash
sudo wget -O /usr/local/bin/pulse https://github.com/arpnova/pulse/releases/download/v0.2.0/pulse && sudo chmod +x /usr/local/bin/pulse
```

> **Note**: You can run `pulse` from anywhere in your terminal once installed. The command above targets the latest **v0.2.0** stable release.

### Option 2: Build from Source

**Prerequisites**:
- Any modern Linux-based operating system.
- `g++` (Compiler supporting C++17).
- `make` (GNU Make).

```bash
# 1. Clone the repository
git clone https://github.com/arpnova/pulse.git
cd pulse

# 2. Compile using GNU Make
make
```
*This statically links and places the `pulse` executable inside the newly generated `build/` directory.*

---

## 💻 Usage

Regardless of how you installed pulse, usage remains identical. If no arguments are passed, `pulse` falls back to auto-discovering the best active connection.

### Core Flags

| Flag | Long Flag | Description |
| :--- | :--- | :--- |
| `-i` | `--interface <name>` | Manually specify a target network interface to analyze. |
| `-s` | `--stats` | Print the historically logged data bandwidth aggregations. |
| `-b` | `--bits` | Display speeds and statistics using bits (`b/s`) instead of Bytes (`B/s`). |
| `-d` | `--daemon` | Start pulse in detached daemon mode, securely tracking bandwidth in the background. |
| `-h` | `--help` | Render the usage matrix and tool help menu. |

### Quick Start Examples

```bash
# Auto-detect and monitor the active interface natively
pulse

# Specify a custom interface manually
pulse -i wlan0

# Monitor your interface using standard bits (b/s) instead of Bytes
pulse -b

# Start the pulse daemon in the background to log stats silently
pulse -d

# Show long-term collected aggregated data usage
pulse --stats
```

---

## 📊 Output Examples

### Live Real-Time Monitoring
Tracking bandwidth in standard notation.

```text
Scanning for active connections... 
Auto-discovered: wlan0
Starting pulse... Monitoring wlan0.
Press Ctrl+C to stop.

Rx: 1.25 MB/s (12.50 MB) |  Tx: 450.32 KB/s (4.50 MB)          
```

### Daemon Viewer Mode
Running `pulse` while `pulse -d` is already active in the background contextually opens Read-Only Viewer Mode safely locking the database file against corruption.

```text
[Note] pulse daemon detected in background
Starting in read-only viewer mode

Monitoring wlan0 (Viewer mode).
Press Ctrl+C to stop.

Rx: 5.60 MB/s (55.40 MB) |  Tx: 1.10 MB/s (12.00 MB)          
```

### Historical Statistics
Evaluating bandwidth totals using `pulse -s`.

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

pulse maintains a deeply decoupled, modular C++ design ensuring maximal maintainability and zero architectural debt.

- **`include/pulse/`**
  - **`network.hpp`**: Hardware interface binding mapping internal kernel `/sys/` states to application memory.
  - **`storage.hpp`**: Secure atomic CSV disk flushing logic and statistical parsing.
  - **`utils.hpp`**: Memory-safe string serialization, arithmetic bounds recovery, and dynamic text formatters.
- **`src/`** 
  - **`main.cpp`**: Primary infinite runtime loops, POSIX hooks (`SIGINT`/`SIGTERM`), and top-level daemon logic.
  - **`network.cpp`**: Low-overhead abstractions mapping `/sys/class/net/`.
  - **`storage.cpp`**: Safely synchronized database writing enforced heavily by `flock()`.
  - **`utils.cpp`**: Implementation logic spanning dynamic scale representations.

---

## 🧹 Maintenance

Use the provided build tools to clear intermediate builds securely:

```bash
make clean
```

---

## 📜 License

This project is open-source software licensed under the **[MIT License](LICENSE)**.

Copyright © 2026 Arpan Biswas
