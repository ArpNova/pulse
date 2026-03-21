<h1 align="center">pulse</h1>

---

<p align="center">
  <em>A lightweight real-time network interface monitor for Linux</em>
</p>

<p align="center">
  <a href="https://github.com/arpnova/pulse/stargazers"><img src="https://img.shields.io/github/stars/arpnova/pulse?style=flat&color=white&labelColor=white&logo=github&logoColor=black" alt="Stars" /></a>
  <a href="https://github.com/arpnova/pulse/network/members"><img src="https://img.shields.io/github/forks/arpnova/pulse?style=flat&color=white&labelColor=white&logo=github&logoColor=black" alt="Forks" /></a>
  <a href="https://github.com/arpnova/pulse/watchers"><img src="https://img.shields.io/github/watchers/arpnova/pulse?style=flat&color=white&labelColor=white&logo=github&logoColor=black" alt="Watchers" /></a>
</p>

<p align="center">
  <a href="https://github.com/arpnova/pulse/blob/main/LICENSE"><img src="https://img.shields.io/github/license/arpnova/pulse?style=flat&color=a6e22e" alt="License" /></a>
  <a href="https://isocpp.org/"><img src="https://img.shields.io/badge/C++-00599C?style=flat&logo=c%2B%2B&logoColor=white" alt="C++" /></a>
  <a href="https://www.linux.org/"><img src="https://img.shields.io/badge/Linux-FCC624?style=flat&logo=linux&logoColor=black" alt="Linux" /></a>
</p>

<p align="center">
  <a href="https://github.com/arpnova/pulse/issues"><img src="https://img.shields.io/github/issues/arpnova/pulse?style=flat&color=f39c12" alt="Issues" /></a>
  <a href="https://github.com/arpnova/pulse/pulls"><img src="https://img.shields.io/github/issues-pr/arpnova/pulse?style=flat&color=a6e22e" alt="Pull Requests" /></a>
  <a href="https://github.com/arpnova/pulse/commits/main"><img src="https://img.shields.io/github/last-commit/arpnova/pulse?style=flat&color=f39c12" alt="Last Commit" /></a>
</p>

---

<p align="center">
  <a href="#features">Features</a> •
  <a href="#prerequisites">Prerequisites</a> •
  <a href="#installation">Installation</a> •
  <a href="#usage">Usage</a> •
  <a href="#license">License</a>
</p>

## 📌 Overview

**pulse** is a minimalist, efficient command-line tool written in C++ that continuously tracks and displays the Receive (Rx) and Transmit (Tx) speeds of a specified network interface on Linux. It directly reads the kernel's network statistics to provide instantaneous and accurate throughput measurements without the need for external, heavy dependencies.

## ✨ Features

- **Real-Time Monitoring**: Instantaneous tracking of Rx and Tx data rates.
- **Dynamic Formatting**: Automatically scales bandwidth units (B/s, KB/s, MB/s, GB/s) for readability.
- **Lightweight**: Minimal CPU and memory footprint, making it ideal for background monitoring or low-resource environments.
- **Dependency-Free**: Uses standard Linux SysFS (`/sys/class/net/`) to read packet statistics directly from the kernel.
- **Fault-Tolerant**: Handles counter wrap-arounds and network stat-read errors gracefully.

## 🛠️ Prerequisites

- A Linux-based operating system.
- `g++` (Standard C++ compiler).
- `make` (For building the project).

## 🚀 Installation

1. Clone or download the repository to your local machine:
   ```bash
   git clone https://github.com/arpnova/pulse.git
   cd pulse
   ```

2. Compile the source code using `make`:
   ```bash
   make
   ```
   This will create a `pulse` executable inside the `build/` directory.

## 💻 Usage

Run the compiled executable. By default, Pulse listens to the `wlan0` interface. To monitor a different interface, simply pass its name as an argument.

```bash
# Run with the default interface (wlan0)
./build/pulse

# Specify a custom interface (e.g., eth0, enp3s0, lo)
./build/pulse eth0
```

Alternatively, you can build and run it in a single command using:

```bash
make run
```

### 📊 Example Output

```text
Starting pulse... Monitoring wlan0.
Press Ctrl+C to stop.

Rx: 1.25 MB/s  |  Tx: 450.32 KB/s          
```

## 🧹 Clean Up

To remove the compiled binaries and the build directory, run:

```bash
make clean
```

## 📜 License

This project is licensed under the [MIT License](LICENSE). 

Copyright (c) 2026 Arpan Biswas.
