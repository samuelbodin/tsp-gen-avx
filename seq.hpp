#pragma once
#include <vector>
#include "path.hpp"
#include "point.hpp"
#include "jtime.hpp"
#include "data.hpp"

namespace seq {
  auto travelingSalesperson(const int paths, const float goal, const float mutationRate, std::vector<point::Point>&) -> Data;
}

auto evolve(const float , const float , std::vector<path::Path>) -> std::pair<float, int>;
auto nextGen(const float, std::vector<path::Path>&, std::vector<float>&) -> std::vector<path::Path>;
auto findParent(const int, std::vector<path::Path>&) -> path::Path;
auto cities(const int) -> std::vector<point::Point>&;
auto randomPopulation(const int, std::vector<point::Point>&) -> std::vector<path::Path>;
auto print_paths(std::vector<path::Path>&) -> void;
auto main() -> int;
