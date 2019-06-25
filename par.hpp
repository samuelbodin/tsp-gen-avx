#pragma once
#include <vector>
#include "point.hpp"
#include "data.hpp"

namespace par {
  auto travelingSalesperson(const int paths, const int threadCount, float reference, const float mutationRate, std::vector<point::Point>& ps) -> Data;
  auto referenceDistance(const int paths, std::vector<point::Point>& ps) -> float;
}
