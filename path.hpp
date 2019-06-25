#pragma once
#include <vector>
#include "point.hpp"

namespace path {
  using Path = std::vector<point::Point>;

  auto distance(const point::Point& p0, const point::Point& p1) -> float;

  auto distance(Path& p) -> float;

  auto idxOfFittest(const int start, const int end, std::vector<float>& ds) -> int;

  auto toStr(Path& p) -> std::string;

  auto mutate(const float mutationRate, Path& p) -> void;

  auto crossover(const Path& a, const Path& b) -> Path;
} // namespace path
