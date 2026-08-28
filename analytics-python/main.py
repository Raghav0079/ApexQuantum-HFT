"""Analytics service entry point."""

from telemetry.metrics import LatencyMetrics


def main() -> None:
    metrics = LatencyMetrics()
    print(f"Analytics service ready; samples={metrics.samples}")


if __name__ == "__main__":
    main()
