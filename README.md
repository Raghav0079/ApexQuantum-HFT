
# ApexQuantum-HFT

[![C++ CI](https://img.shields.io/badge/C%2B%2B-Release%20Build-success?logo=c%2B%2B&logoColor=white)](https://github.com/)
[![Java](https://img.shields.io/badge/Java-17%2B-blue?logo=openjdk&logoColor=white)](https://openjdk.java.net/)
[![Python](https://img.shields.io/badge/Python-3.11%2B-3776AB?logo=python&logoColor=white)](https://www.python.org/)
[![Redis](https://img.shields.io/badge/Redis-7.x-DC382D?logo=redis&logoColor=white)](https://redis.io/)

> ApexQuantum-HFT is a modular, ultra-low latency HFT engine for market-data ingestion, order matching, and operational telemetry. The codebase is organized around a cache-friendly C++ execution core, a Java OMS layer, and a Python analytics/monitoring path backed by Redis.

---

## Architecture Overview

```text
+-------------------------------------------------------------------------+
|                         ApexQuantum-HFT Core                            |
+-------------------------------------------------------------------------+
|                                                                         |
|  +--------------------+   Shared Memory /   +------------------------+  |
|  | C++20 Exec Engine  | <=================> | Java Backend Service   |  |
|  | - Market Connectivity|   Lock-Free SPSC    | - OMS & Routing       |  |
|  | - Matching Engine   |    Ring Buffers     | - System Control      |  |
|  +---------+----------+                     +-----------+------------+  |
|            |                                            |               |
|            +------------------+     +-------------------+               |
|                               v     v                                   |
|                     +-------------------+                               |
|                     |    Redis Cache    |                               |
|                     | - Event Bus       |                               |
|                     | - Order Snapshots |                               |
|                     +---------+---------+                               |
|                               |                                         |
|                               v                                         |
|                     +-------------------+                               |
|                     | Python Analytics  |                               |
|                     | - Telemetry       |                               |
|                     | - Monitoring      |                               |
|                     +-------------------+                               |
|                                                                         |
+-------------------------------------------------------------------------+
```

* C++20 execution engine: low-latency order handling, parser path, and matching core
* Java backend: orchestration, OMS state, API layer, and integration glue
* Python analytics: Redis consumers, telemetry aggregation, and operational observability
* Redis: inter-service event distribution and order snapshots

---

## Repository Metadata

Use the following values for the GitHub repository description and tags:

- Description: `C++20 low-latency HFT engine with Redis, Java OMS, and Python analytics`
- Topics: `cpp`, `hft`, `trading`, `market-data`, `low-latency`, `redis`, `cmake`, `benchmarking`, `algorithmic-trading`

---

## Measured Performance

The benchmark evidence below was captured from the repo’s generated benchmark binary on the current host using Google Benchmark with `--benchmark_min_time=2s` and 10 repetitions. The output file is retained in `benchmark_output.json`.

| Benchmark | Host / config | Measured result | Notes |
| --- | --- | --- | --- |
| SPSC queue round-trip | 12-core Windows laptop, Release build, GCC 16.2 via w64devkit | Mean: 226.5 ns; median: 224.0 ns; stddev: 6.4 ns | Approx. 4.4M queue round-trips/sec |

> This project intentionally avoids publishing speculative targets as factual results. The numbers above are the real measured values from the repo’s benchmark artifact, and they are the basis for the operational documentation in this repository.

---

## System Requirements & Prerequisites

### Compiler & Tooling Requirements

* C++ Compiler: GCC 13+ (GCC 16.2+ recommended via `w64devkit` on Windows / native GCC on Linux)
* Build System: CMake 3.22+ and Ninja / Make
* Java SDK: OpenJDK 17+
* Python Engine: Python 3.10+
* In-Memory Cache: Redis Server 7.0+

---

## Quick Start Guide

### 1. Build the C++ engine

```powershell
cmake -S . -B build -G "Ninja" -DCMAKE_BUILD_TYPE=Release -DAPEXQ_BUILD_TESTS=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

### 2. Build the benchmark target

```powershell
cmake -S . -B build-bench -G "Ninja" -DCMAKE_BUILD_TYPE=Release -DAPEXQ_ENABLE_BENCHMARKS=ON -Dbenchmark_DIR=C:/path/to/benchmark/build
cmake --build build-bench
```

### 3. Run the benchmark

```powershell
./build-bench/core-cpp/apexquantum_latency_bench.exe --benchmark_min_time=2s --benchmark_repetitions=10 --benchmark_format=json > benchmark_output.json
```

### 4. Run the Docker demo stack

```bash
docker compose -f docker/docker-compose.yml up -d --build
./scripts/demo_order_pipeline.sh
```

---

## Directory Structure

```text
ApexQuantum-HFT/
├── cmake/
│   └── CompilerFlags.cmake
├── core-cpp/
│   ├── include/
│   ├── src/
│   ├── tests/
│   ├── bench/
│   └── CMakeLists.txt
├── backend-java/
│   ├── src/
│   └── pom.xml
├── analytics-python/
│   ├── telemetry/
│   ├── utils/
│   ├── requirements.txt
│   └── main.py
├── docker/
│   ├── Dockerfile.cpp
│   ├── Dockerfile.java
│   ├── Dockerfile.analytics
│   └── docker-compose.yml
├── scripts/
│   └── demo_order_pipeline.sh
├── .github/workflows/ci.yml
├── .gitignore
├── LICENSE
├── CMakeLists.txt
├── README.md
└── benchmark_output.json
```

---

## Demo / Proof of Working System

The project includes a lightweight end-to-end demo path that publishes a single order into Redis and confirms analytics observes it. The intended workflow is:

1. Start Redis and the service stack with Docker Compose.
2. Publish a test order to the `apexquantum:orders` Redis list.
3. Confirm analytics logs the `Order received by analytics` message.
4. Use the saved benchmark output as the measured latency evidence for the queue path.

This keeps the repo grounded in observable behavior rather than static claims.

---

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

