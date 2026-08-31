# cmake/CompilerFlags.cmake

function(apply_hft_compiler_flags TARGET_NAME)
    target_compile_features(${TARGET_NAME} PRIVATE cxx_std_20)

    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${TARGET_NAME} PRIVATE
            -O3
            -march=native
            -funroll-loops
            -ffast-math
            -Wall
            -Wextra
            -Wpedantic
            -Wno-padded
            -fno-omit-frame-pointer
        )

        target_link_options(${TARGET_NAME} PRIVATE
            $<$<CONFIG:Release>:-s>
        )
    elseif (MSVC)
        target_compile_options(${TARGET_NAME} PRIVATE
            /O2 /arch:AVX2 /std:c++20 /permissive-
        )
    endif()
endfunction()