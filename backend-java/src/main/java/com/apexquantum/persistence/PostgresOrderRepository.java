package com.apexquantum.persistence;

import com.apexquantum.config.ServiceConfig;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.sql.Timestamp;

public final class PostgresOrderRepository {
    private static final String INSERT_ORDER = """
            INSERT INTO orders (order_id, side, order_type, price, quantity, status, created_at)
            VALUES (?, ?, ?, ?, ?, ?, ?)
            ON CONFLICT (order_id) DO UPDATE SET
                status = EXCLUDED.status
            """;

    private final ServiceConfig config;

    public PostgresOrderRepository(ServiceConfig config) {
        this.config = config;
    }

    public void save(OrderRecord order) throws SQLException {
        try (Connection connection = DriverManager.getConnection(
                config.postgresUrl(), config.postgresUsername(), config.postgresPassword());
             PreparedStatement statement = connection.prepareStatement(INSERT_ORDER)) {
            statement.setString(1, order.orderId());
            statement.setString(2, order.side());
            statement.setString(3, order.orderType());
            statement.setBigDecimal(4, order.price());
            statement.setLong(5, order.quantity());
            statement.setString(6, order.status());
            statement.setTimestamp(7, Timestamp.from(order.createdAt()));
            statement.executeUpdate();
        }
    }
}
