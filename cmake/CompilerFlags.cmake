# cmake/CompilerFlags.cmake

function(apply_hft_compiler_flags TARGET_NAME)
    target_compile_features(${TARGET_NAME} PRIVATE cxx_std_20)

    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${TARGET_NAME} PRIVATE
            -O3                         # Maximum optimization level
            -march=native               # Enable all AVX2/AVX-512 instructions on host hardware
            -funroll-loops              # Unroll loops for branch optimization
            -ffast-math                 # High-speed floating point math (trade safety for speed)
            -Wall
            -Wextra
            -Wpedantic
            -Wno-padded                 # Silence alignment padding warnings
        )
        
        # Enable Link-Time Optimization (LTO) for Release builds
        target_link_options(${TARGET_NAME} PRIVATE
            $<$<CONFIG:Release>:-flto>
            $<$<CONFIG:Release>:-s>      # Strip symbol tables in release builds
        )
    elseif (MSVC)
        target_compile_options(${TARGET_NAME} PRIVATE
            /O2 /arch:AVX2 /std:c++20
        )
    endif()
endfunction()