from dataclasses import dataclass


@dataclass
class LatencyMetrics:
    samples: int = 0
    last_latency_ns: int | None = None

    def record(self, latency_ns: int) -> None:
        if latency_ns < 0:
            raise ValueError("latency must be non-negative")
        self.samples += 1
        self.last_latency_ns = latency_ns
