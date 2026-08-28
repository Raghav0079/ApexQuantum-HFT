FROM ubuntu:24.04 AS build
RUN apt-get update && apt-get install -y --no-install-recommends cmake g++ ninja-build \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /src
COPY . .
RUN cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DAPEXQUANTUM_BUILD_TESTS=OFF \
    && cmake --build build

FROM ubuntu:24.04
COPY --from=build /src/build/core-cpp/apexquantum_engine /usr/local/bin/apexquantum_engine
ENTRYPOINT ["/usr/local/bin/apexquantum_engine"]
