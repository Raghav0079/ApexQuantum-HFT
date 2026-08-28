package com.apexquantum.config;

public record ServiceConfig(
    String environment,
    String redisUrl,
    String postgresUrl,
    String postgresUsername,
    String postgresPassword) {
    public static ServiceConfig fromEnvironment() {
        return new ServiceConfig(
                valueOrDefault("APEXQUANTUM_ENV", "development"),
        valueOrDefault("REDIS_URL", "redis://localhost:6379"),
        valueOrDefault("POSTGRES_URL", "jdbc:postgresql://localhost:5432/apexquantum"),
        valueOrDefault("POSTGRES_USER", "apexquantum"),
        valueOrDefault("POSTGRES_PASSWORD", "apexquantum"));
    }

    private static String valueOrDefault(String name, String fallback) {
        String value = System.getenv(name);
        return value == null || value.isBlank() ? fallback : value;
    }
}
