#include "jtime.hpp"
#include "par.hpp"
#include "seq.hpp"
#include "data.hpp"
#include <iostream>

auto main() -> int {
  const auto cities = 16;
  const auto paths = 16;
  const auto mutationRate = .015f;

  auto ps = point::randomPoints(cities, {0.f, 0.f}, 100.f);

  auto goal = 0.75f * par::referenceDistance(paths, ps);

  auto result1 = seq::travelingSalesperson(paths, goal, mutationRate, ps);
  printf("time: %8i ms, distance: %8.1f, generations: %4i, goal: %8.1f\n", static_cast<int>(jtime::diff_ms(result1.start, result1.end)), result1.distance, result1.generations, goal);
  std::cin.get();

  auto result2 = par::travelingSalesperson(paths, 4, goal, mutationRate, ps);
  printf("time: %8i us, distance: %6.1f, generations: %3i, goal: %6.1f\n", static_cast<int>(jtime::diff_us(result2.start, result2.end)), result2.distance, result2.generations, goal);

  std::cin.get();
  return 0;
}
