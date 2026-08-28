
# ApexQuantum-HFT

> **ApexQuantum-HFT** is a modular, ultra-low latency hybrid High-Frequency Trading (HFT) engine designed for sub-microsecond order execution, real-time risk analytics, and institutional-grade order routing.

---

## Architecture Overview


```

+-------------------------------------------------------------------------+
|                        ApexQuantum-HFT Core                             |
+-------------------------------------------------------------------------+
|                                                                         |
|  +--------------------+   Shared Memory /   +------------------------+  |
|  | C++20 Exec Engine  | <=================> | Java Backend Service   |  |
|  | - Market Connectivity  |   Lock-Free SPSC    | - Order Lifecycle / OMS|  |
|  | - Fast Order Routing  |     Ring Buffers    | - System Orchestration |  |
|  +---------+----------+                     +-----------+------------+  |
|            |                                            |               |
|            +------------------+     +-------------------+               |
|                               v     v                                   |
|                     +-------------------+                               |
|                     |    Redis Cache    |                               |
|                     | - Order State Snapshot                            |
|                     +---------+---------+                               |
|                               |                                         |
|                               v                                         |
|                     +-------------------+                               |
|                     | Python Analytics  |                               |
|                     | - Alpha Models    |                               |
|                     | - Telemetry & Viz |                               |
|                     +-------------------+                               |
|                                                                         |
+-------------------------------------------------------------------------+

```

* **C++20 Execution Engine:** Handles raw order matching, market data parsing (FIX/FAST, ITCH), and direct exchange connectivity with zero-copy ring buffers and cache-line aligned data structures.
* **Java Backend Infrastructure:** Manages system orchestration, higher-level OMS/EMS state tracking, accounting, and API gateways.
* **Python Analytics Pipeline:** Runs offline/real-time quantitative alpha strategy modeling, backtesting verification, and operational telemetry.
* **Redis:** Provides high-throughput state persistence and inter-process event distribution.

---

## Directory Structure

```text
ApexQuantum-HFT/
├── cmake/                      # CMake modules & toolchain configurations
│   └── CompilerFlags.cmake     # Micro-architecture optimization (AVX2, LTO, etc.)
│
├── core-cpp/                   # Low-Latency C++20 Execution Engine
│   ├── include/                # Header files (.hpp)
│   │   ├── common/             # Types, lock-free queues, memory alignment utilities
│   │   ├── market_data/        # Order book engine & L2/L3 market feeds
│   │   ├── matching/           # High-speed matching algorithms
│   │   ├── network/            # Kernel bypass / Socket wrappers (UDP/TCP)
│   │   └── order_book/         # Zero-copy OrderBook data structure
│   ├── src/                    # Implementation files (.cpp)
│   ├── tests/                  # Catch2 / GoogleTest benchmark & unit testing suits
│   └── CMakeLists.txt          # C++ Core build definitions
│
├── backend-java/               # Java Institutional Backend & OMS
│   ├── src/
│   │   ├── main/java/com/apexquantum/
│   │   │   ├── config/         # Environment & component configurations
│   │   │   ├── ipc/            # JNI / Shared Memory bindings to C++ Core
│   │   │   ├── oms/            # Order Management System logic
│   │   │   └── risk/           # Pre-trade risk check checks
│   │   └── test/               # JUnit test suites
│   ├── pom.xml / build.gradle  # Build configuration
│   └── README.md
│
├── analytics-python/           # Python Strategy & Risk Telemetry
│   ├── strategies/             # Alpha generation strategies
│   ├── telemetry/              # Real-time latency tracking & dashboard metrics
│   ├── utils/                  # Redis consumers and serialization helpers
│   ├── requirements.txt        # Python dependency specifications
│   └── main.py                 # Analytics service entry point
│
├── docker/                     # Dockerization & container manifests
│   ├── Dockerfile.cpp          # Build environment for C++ Core
│   ├── Dockerfile.java         # Container build for Java OMS
│   └── docker-compose.yml      # Full-stack orchestrator setup
│
├── scripts/                    # Utility scripts (build, benchmark, deployment)
│   ├── build_cpp.ps1           # PowerShell build script for C++ engine
│   └── run_benchmarks.cmd      # Latency & throughput benchmarking runners
│
├── .gitignore
├── LICENSE
└── README.md

```

---

## System Requirements & Prerequisites

### Compiler & Tooling Requirements

* **C++ Compiler:** GCC 13+ (GCC 16.2+ recommended via `w64devkit` on Windows / Native GCC on Linux)
* **Build System:** CMake 3.22+ & Ninja / Make
* **Java SDK:** OpenJDK 17+ or OpenJDK 21 LTS
* **Python Engine:** Python 3.10+
* **In-Memory Cache:** Redis Server 7.0+

---

## Quick Start Guide

### 1. Environment Verification

Ensure `g++` points to modern GCC 13+:

```bash
g++ --version
# Output should reflect GCC 13+ or GCC 16+

```

### 2. Building the Low-Latency C++ Engine

```powershell
# Navigate to the core-cpp module
cd core-cpp

# Generate Ninja/Make build files
cmake -B build -G "Ninja" -DCMAKE_BUILD_TYPE=Release

# Compile binaries with SIMD optimizations enabled
cmake --build build --config Release

```

### 3. Running the Micro-Benchmarks

```powershell
./build/bin/latency_benchmarks

```

---

## Performance Targets

| Metric | Target |
| --- | --- |
| **Tick-to-Trade Latency** | `< 2.5 μs` |
| **SPSC Queue Throughput** | `> 50M ops/sec` |
| **Order Book Update Latency** | Sub-microsecond (`< 800 ns`) |

---

## License

This project is licensed under the MIT License - see the [LICENSE](https://www.google.com/search?q=LICENSE) file for details.

```

```
