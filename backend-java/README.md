# ApexQuantum Backend

Java 17 backend boundary for order lifecycle management, pre-trade risk checks, IPC, system orchestration, and durable order storage.

## PostgreSQL

Set `POSTGRES_URL`, `POSTGRES_USER`, and `POSTGRES_PASSWORD` before using `PostgresOrderRepository`. Apply `src/main/resources/db/schema.sql` to create the order tables.

The application does not connect to PostgreSQL during startup. A database is required only when repository methods are called.

The service is intentionally dependency-light until the transport and persistence contracts are finalized.
