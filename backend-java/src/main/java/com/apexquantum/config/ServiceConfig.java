package com.apexquantum.config;

public record ServiceConfig(String environment, String redisUrl) {
    public static ServiceConfig fromEnvironment() {
        return new ServiceConfig(
                valueOrDefault("APEXQUANTUM_ENV", "development"),
                valueOrDefault("REDIS_URL", "redis://localhost:6379"));
    }

    private static String valueOrDefault(String name, String fallback) {
        String value = System.getenv(name);
        return value == null || value.isBlank() ? fallback : value;
    }
}
