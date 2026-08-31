"""Analytics service entry point."""

import os
import time

import redis

from telemetry.metrics import LatencyMetrics


def main() -> None:
    metrics = LatencyMetrics()
    redis_url = os.getenv("REDIS_URL", "redis://localhost:6379")
    client = redis.Redis.from_url(redis_url, decode_responses=True)
    print(f"Analytics service ready; redis={redis_url}; samples={metrics.samples}")

    while True:
        message = client.blpop("apexquantum:orders", timeout=5)
        if message:
            _, payload = message
            metrics.record(len(payload))
            print(f"Order received by analytics: {payload}")
        time.sleep(0.1)


if __name__ == "__main__":
    main()
