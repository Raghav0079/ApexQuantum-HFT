#pragma once

#include <functional>
#include <thread>
#include <type_traits>
#include <utility>

#if defined(_WIN32)
#include <windows.h>
#elif defined(__linux__)
#include <pthread.h>
#include <sched.h>
#endif

namespace apexq::utils {

inline bool pin_thread_to_core(int core_id) {
#if defined(_WIN32)
    if (core_id < 0) {
        return false;
    }

    DWORD_PTR mask = static_cast<DWORD_PTR>(1ULL << core_id);
    return SetThreadAffinityMask(GetCurrentThread(), mask) != 0;
#elif defined(__linux__)
    if (core_id < 0) {
        return false;
    }

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);

    const int rc = pthread_setaffinity_np(pthread_self(), sizeof(cpuset), &cpuset);
    return rc == 0;
#else
    (void)core_id;
    return false;
#endif
}

template <typename F, typename... Args>
inline std::thread create_pinned_thread(int core_id, F&& f, Args&&... args) {
    return std::thread([core_id, fn = std::forward<F>(f), ...captured_args = std::forward<Args>(args)]() mutable {
        pin_thread_to_core(core_id);
        std::invoke(fn, std::forward<decltype(captured_args)>(captured_args)...);
    });
}

}  // namespace apexq::utils
