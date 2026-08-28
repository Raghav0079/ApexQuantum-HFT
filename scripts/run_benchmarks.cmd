@echo off
setlocal
if not exist build\bin\latency_benchmarks (
    echo Build the benchmark target before running this script.
    exit /b 1
)
build\bin\latency_benchmarks
