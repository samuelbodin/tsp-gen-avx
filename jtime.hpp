#pragma once
#include <chrono>
#include <string>

namespace jtime {
  using time_type = std::chrono::time_point<std::chrono::high_resolution_clock>;
  using duration = std::chrono::duration<float>;
  using clock = std::chrono::high_resolution_clock;
  using ms = std::chrono::milliseconds;
  using us = std::chrono::microseconds;

  auto diff_ms(time_type t0, time_type t1) -> int64_t;
  auto diff_us(time_type t0, time_type t1) -> int64_t;

  inline auto now() -> time_type {
    return clock::now();
  }
}
