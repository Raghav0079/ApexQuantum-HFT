FROM eclipse-temurin:17-jdk AS build
WORKDIR /app
COPY backend-java/pom.xml .
COPY backend-java/src src
RUN apt-get update && apt-get install -y --no-install-recommends maven \
    && mvn -q package -DskipTests \
    && rm -rf /var/lib/apt/lists/*

FROM eclipse-temurin:17-jre
WORKDIR /app
COPY --from=build /app/target/apexquantum-backend-0.1.0-SNAPSHOT.jar app.jar
ENTRYPOINT ["java", "-jar", "app.jar"]
