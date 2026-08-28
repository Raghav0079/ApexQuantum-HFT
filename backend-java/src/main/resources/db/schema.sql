CREATE TABLE IF NOT EXISTS orders (
    order_id VARCHAR(64) PRIMARY KEY,
    side VARCHAR(8) NOT NULL CHECK (side IN ('BUY', 'SELL')),
    order_type VARCHAR(16) NOT NULL CHECK (order_type IN ('LIMIT', 'MARKET')),
    price NUMERIC(20, 8),
    quantity BIGINT NOT NULL CHECK (quantity > 0),
    status VARCHAR(32) NOT NULL,
    created_at TIMESTAMPTZ NOT NULL
);

CREATE INDEX IF NOT EXISTS orders_created_at_idx ON orders (created_at);
CREATE INDEX IF NOT EXISTS orders_status_idx ON orders (status);
