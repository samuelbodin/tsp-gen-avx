#include <vector>
#include "seq.hpp"
#include "random.hpp"
#include "jtime.hpp"

auto evolve(const float goal, const float mutationRate, std::vector<path::Path> pop) -> std::pair<float, int> {
  auto distances = std::vector<float>(pop.size());

  float bestOfGen;
  auto generation = 1;

  while (true) {
    for(auto i = 0u; i < pop.size(); ++i) {
      distances[i] = path::distance(pop[i]);
    }

    auto fittest = path::idxOfFittest(0, pop.size(), distances);

    bestOfGen = distances[fittest];

    printf("best of gen %4i: %8.1f, goal: %8.1f\n", generation, bestOfGen, goal);

    if (goal <= bestOfGen) {
      pop = nextGen(mutationRate, pop, distances);
      ++generation;
    } else {
      break;
    }
  }

  return {bestOfGen, generation};
}

auto findParent(const int range, std::vector<path::Path>& pop, std::vector<float>& ds) -> int {
  const auto start = random::randomInt(0, pop.size() - range);
  return path::idxOfFittest(start, range, ds);
}

auto nextGen(const float mutationRate, std::vector<path::Path>& pop, std::vector<float>& ds) -> std::vector<path::Path> {
  auto v = std::vector<path::Path>(pop.size());
  for (auto& p : v) {
    auto idx1 = findParent(8, pop, ds);
    auto idx2 = findParent(8, pop, ds);

    p = path::crossover(pop[idx1], pop[idx2]);

    path::mutate(mutationRate, p);
  }
  return v;
}

auto randomPopulation(const int n, std::vector<point::Point>& pts) -> std::vector<path::Path> {
  auto paths = std::vector<path::Path>(n);
  for (auto& p : paths) {
    point::shufflePoints(pts);
    p = pts;
  }
  return paths;
}

auto seq::travelingSalesperson(const int paths, const float goal, const float mutationRate, std::vector<point::Point>& ps) -> Data {
  auto path_vec = randomPopulation(paths, ps);

  auto t = jtime::now();
  auto result = evolve(goal, mutationRate, path_vec);
  return {t, jtime::now(), result.first, result.second};
}
