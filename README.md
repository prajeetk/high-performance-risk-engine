# High-Performance Risk Engine

A modular, low-latency risk management engine written in **Modern C++20**, inspired by the pre-trade risk systems used in quantitative trading firms and electronic exchanges.

The engine sits between a trading strategy and an exchange, validating every incoming order against configurable risk rules before it can be executed. It is designed with an emphasis on **performance, extensibility, and clean software architecture**, making it both a learning project and a realistic simulation of production trading infrastructure.

## Features

* **Pre-Trade Risk Validation**
  * Position limit checks
  * Maximum notional exposure checks
  * Fat-finger price protection
  * Order approval/rejection with detailed reasons

* **Position & PnL Tracking**
  * Real-time position updates
  * Average cost basis calculation
  * Realized and unrealized PnL

* **Modular Risk Engine**
  * Independent, pluggable risk checks
  * Chain-of-responsibility inspired validation pipeline
  * Easily extensible for new risk rules

* **Performance-Oriented Design**
  * Modern C++20
  * Cache-friendly data structures
  * Minimal dynamic allocations in hot paths
  * Benchmarking support for latency and throughput analysis

* **Testing**
  * Comprehensive unit tests for risk checks and position management
  * Edge-case validation
  * Deterministic behavior across scenarios

## Tech Stack

* C++20
* CMake
* Catch2
* fmt
* spdlog

## Project Structure

```text
src/
├── core/
│   ├── Order
│   ├── Trade
│   └── Position
├── risk/
│   ├── RiskEngine
│   ├── PositionLimit
│   ├── ExposureLimit
│   ├── FatFingerCheck
│   └── PnL
├── utils/
└── tests/
```

## Build & Run

Requirements: CMake 3.20+, a C++20 compiler (MSVC, GCC, or Clang).

```bash
cmake -S . -B build
cmake --build build
./build/risk_engine
ctest --test-dir build
```

On Windows with Visual Studio:

```powershell
cmake -S . -B build
cmake --build build --config Release
.\build\Release\risk_engine.exe
ctest --test-dir build -C Release
```

## Motivation

Modern trading firms cannot rely solely on profitable strategies—they must also ensure every order adheres to strict risk constraints before reaching the market. This project recreates the core responsibilities of a pre-trade risk engine in a simplified but realistic form, focusing on software engineering principles commonly found in high-performance trading systems.

## Future Improvements

* Multi-threaded order validation
* Lock-free order ingestion
* Per-account and per-symbol risk limits
* Dynamic risk configuration (JSON/YAML)
* Historical order replay
* Real-time monitoring dashboard
* Latency profiling and optimization
* Market data integration

---

This project is part of a larger quantitative trading systems portfolio, with the goal of exploring the software architecture, performance considerations, and engineering practices behind modern electronic trading systems.
