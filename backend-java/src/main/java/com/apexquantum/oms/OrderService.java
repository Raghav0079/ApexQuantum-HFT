package com.apexquantum.oms;

import com.apexquantum.config.ServiceConfig;

public final class OrderService {
    private final ServiceConfig config;

    public OrderService(ServiceConfig config) {
        this.config = config;
    }

    public ServiceConfig config() {
        return config;
    }
}
