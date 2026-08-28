param(
    [ValidateSet("Debug", "Release")]
    [string]$Configuration = "Release"
)

cmake -S . -B build -G Ninja `
    -DCMAKE_BUILD_TYPE=$Configuration `
    -DAPEXQUANTUM_BUILD_TESTS=ON
cmake --build build --config $Configuration
