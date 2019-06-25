#include <string>
#include <sstream>
#include "population.hpp"
#include "avx.hpp"
#include "jtime.hpp"
#include "random.hpp"
#include "par.hpp"

inline auto toStr(const int n, float* vec, int width) -> std::string {
  auto ss = std::stringstream();
  auto s = std::string();
  s.resize(width);

  for (auto i = 0; i < n; ++i) {
    std::fill(begin(s), end(s), '0');
    s = std::to_string(vec[i]);
    ss << s.substr(0, width);
    if (i < n - 1) {
      ss << ", ";
    }
  }
  return ss.str();
}

auto crossover(const int n, float* p1X, float* p1Y, float* p2X, float* p2Y, float* childX, float* childY) -> void {
  auto start = 0, end = 0;
  std::tie(start, end) = std::minmax(random::randomInt(0, n), random::randomInt(0, n));
  auto j = 0;
  for (auto i = 0; i < n; ++i) {
    if (start <= i && i < end) {
      childX[i] = p1X[i];
      childY[i] = p1Y[i];
    } else {
      while (true) {
        auto idx = end;

        auto x1 = p2X[j];
        auto y1 = p2Y[j];

        for (auto k = start; k < end; ++k) {
          auto x2 = p1X[k];
          auto y2 = p1Y[k];

          if (x1 == x2 && y1 == y2) {
            idx = k;
            break;
          }
        }
        if (idx < end) {
          ++j;
        } else {
          childX[i] = x1;
          childY[i] = y1;
          ++j;
          break;
        }
      }
    }
  }
}

auto mutate(const int n, float* xs, float* ys, const float mutationRate) -> void {
  for (auto i = 0; i < n; ++i) {
    if (random::randomFloat() < mutationRate) {
      const auto j = random::randomInt(0, n);
      std::swap(xs[i], xs[j]);
      std::swap(ys[i], ys[j]);
    }
  }
}

auto mate(population::Population& pop1, population::Population& pop2, float* childX, float* childY, float* ds, const float mutationRate) -> void {
  const auto n = pop1.cities;
  const auto m = pop1.paths;

  const auto range = 8;
  const auto maxStart = m - range;

  auto rand1 = random::randomInt(0, maxStart);
  auto rand2 = random::randomInt(0, maxStart);

  auto idx1 = rand1 + avx::indexOfMin(range, ds + rand1);
  auto idx2 = rand2 + avx::indexOfMin(range, ds + rand2);

  auto parent1X = &pop2.xs.at(idx1, 0);
  auto parent1Y = &pop2.ys.at(idx1, 0);

  auto parent2X = &pop2.xs.at(idx2, 0);
  auto parent2Y = &pop2.ys.at(idx2, 0);

  crossover(n, parent1X, parent1Y, parent2X, parent2Y, childX, childY);

  mutate(n, childX, childY, mutationRate);
}

auto evolve(jtime::time_type& t, bool& isDone, float& result, int& generations, const int id, const float goal, const float mutationRate, population::Population& pop1, population::Population& pop2) -> void {
  const auto n = pop1.cities;
  const auto m = pop1.paths;
  const auto limit = 1200;

  auto nextGenX = std::vector<float>(n * m);
  auto nextGenY = std::vector<float>(n * m);

  auto tempX = static_cast<float*>(_mm_malloc(sizeof(float*) * m, 32));
  auto tempY = static_cast<float*>(_mm_malloc(sizeof(float*) * m, 32));
  auto ds = static_cast<float*>(_mm_malloc(sizeof(float*) * m, 32));

  auto xs = pop1.xs.data;
  auto ys = pop1.xs.data;

  auto gen = 1;

  auto bestOfGen = 0.f;

  while (true) {

    std::fill(ds, ds + m, 0.f);

    avx::distance(n, m, xs, ys, tempX, tempY, ds);

    bestOfGen = avx::min(m, ds);

    printf("best of gen %4i: %8.1f, goal: %8.1f, thread: %1i\n", gen, bestOfGen, goal, id);

    matrix::transpose(pop1.xs, pop2.xs);
    matrix::transpose(pop1.ys, pop2.ys);

    auto j = 0;

    for (auto i = 0; i < m; ++i) {
      auto childX = &nextGenX[j];
      auto childY = &nextGenY[j];

      mate(pop1, pop2, childX, childY, ds, mutationRate);

      j += n;
    }

    matrix::transpose(m, n, nextGenX, pop1.xs);
    matrix::transpose(m, n, nextGenY, pop1.ys);
     
    if(bestOfGen < goal || isDone || limit < gen) {
      break;
    }

    ++gen;
  }

  if (!isDone) {
    isDone = true;
    t = jtime::now();
    result = limit <= gen ? -1.f : bestOfGen;
    generations = gen;
  }
}

auto par::referenceDistance(const int paths, std::vector<point::Point>& ps) -> float {
  auto pop = population::Population(ps.size(), paths);
  population::randomPopulation(paths, ps, pop);

  const auto n = pop.cities;
  const auto m = pop.paths;

  auto tempX = static_cast<float*>(_mm_malloc(sizeof(float*) * m, 32));
  auto tempY = static_cast<float*>(_mm_malloc(sizeof(float*) * m, 32));
  auto ds = static_cast<float*>(_mm_malloc(sizeof(float*) * m, 32));
  std::fill(ds, ds + m, 0.f);

  auto xs = pop.xs.data;
  auto ys = pop.xs.data;

  avx::distance(n, m, xs, ys, tempX, tempY, ds);

  auto best = avx::min(m, ds);

  _mm_free(tempX);
  _mm_free(tempY);
  _mm_free(ds);

  return best;
}

struct PopulationPair {
  population::Population* first;
  population::Population* second;
};

auto par::travelingSalesperson(const int paths, const int threadCount, const float goal, const float mutationRate, std::vector<point::Point>& ps) -> Data {
  const auto cities = ps.size();

  auto popPairs = std::vector<PopulationPair>(threadCount);
  auto threads = std::vector<std::thread>(threadCount);

  for (auto& p : popPairs) {
    p.first = new population::Population(cities, paths);
    p.second = new population::Population(cities, paths);
    population::randomPopulation(paths, ps, *p.first);
  }

  auto t1 = jtime::now();
  auto t2 = jtime::time_type{};

  auto isDone = false;
  auto result = 0.f;
  auto generations = 0;

  for (auto i = 0; i < threadCount; ++i) {
    auto firstPtr = popPairs[i].first;
    auto secondPtr = popPairs[i].second;
    threads[i] = std::thread(evolve, std::ref(t2), std::ref(isDone), std::ref(result), std::ref(generations), i, goal, mutationRate, std::ref(*firstPtr), std::ref(*secondPtr));
  }

  for (auto& thread : threads) {
    thread.join();
  }

  for (auto& p : popPairs) {
    delete p.first;
    delete p.second;
  }

  return {t1, t2, result, generations};
}
