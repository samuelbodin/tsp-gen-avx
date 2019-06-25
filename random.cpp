#include "random.hpp"
#include <random>

auto random::randomInt(int lo, int hi) -> int {
  auto diff = hi - lo;
  static std::random_device rd;
  static auto rng = std::mt19937(rd());
  static auto dis = std::uniform_int_distribution<>(0, std::numeric_limits<int>::max());
  return static_cast<int>(dis(rng)) % diff + lo;
}

auto random::randomFloat() -> float {
  static std::random_device rd;
  static auto rng = std::mt19937(rd());
  static auto dis = std::uniform_real_distribution<>(0.f, 1.f);
  return static_cast<float>(dis(rng));
}
