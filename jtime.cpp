#include "jtime.hpp"
#include <chrono>
#include <thread>

auto jtime::diff_ms(time_type t0, time_type t1) -> int64_t {
  auto diff = t1 - t0;
  auto d = std::chrono::duration_cast<ms>(diff);
  return d.count();
}

auto jtime::diff_us(time_type t0, time_type t1) -> int64_t {
  auto diff = t1 - t0;
  auto d = std::chrono::duration_cast<us>(diff);
  return d.count();
}
