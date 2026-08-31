#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

compose_file="$ROOT_DIR/docker/docker-compose.yml"
if [[ ! -f "$compose_file" ]]; then
  echo "Compose file not found: $compose_file" >&2
  exit 1
fi

if ! docker compose -f "$compose_file" config >/dev/null 2>&1; then
  echo "docker compose not available or configuration invalid" >&2
  exit 1
fi

echo "Starting Redis + Java OMS + C++ core + analytics pipeline..."
docker compose -f "$compose_file" up -d --build

sleep 5

REDIS_KEY="apexquantum:orders"

echo "Publishing one test order to Redis..."
docker run --rm --network host redis:7-alpine redis-cli -u redis://localhost:6379 LPUSH "$REDIS_KEY" '{"id":1,"side":"BUY","price":100.0,"quantity":10}'

echo "Waiting for analytics to consume the message..."
for i in $(seq 1 20); do
  if docker logs "$(docker compose -f "$compose_file" ps -q analytics 2>/dev/null || true)" 2>/dev/null | grep -q "Order received by analytics"; then
    echo "Observed analytics consumption of the test order."
    break
  fi
  sleep 1
done

echo "Pipeline demo complete."
echo "Use: docker compose -f $compose_file logs -f"
