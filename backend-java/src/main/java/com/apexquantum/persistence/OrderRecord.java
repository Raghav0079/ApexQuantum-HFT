package com.apexquantum.persistence;

import java.math.BigDecimal;
import java.time.Instant;

public record OrderRecord(
        String orderId,
        String side,
        String orderType,
        BigDecimal price,
        long quantity,
        String status,
        Instant createdAt) {
}
