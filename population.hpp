#pragma once
#include <vector>
#include "point.hpp"
#include <algorithm>
#include "matrix.hpp"

namespace population {
  struct Population {
    int paths;
    int cities;
    matrix::Matrix xs;
    matrix::Matrix ys;

    Population(int cities, int paths) : paths{paths},
                                        cities{cities},
                                        xs(cities, paths),
                                        ys(cities, paths) { }
  };

  inline auto randomPopulation(const int n, std::vector<point::Point>& ps, Population& pop) -> void {
    const int m = ps.size();
    pop.xs.rows = m;
    pop.xs.cols = n;
    pop.ys.rows = m;
    pop.ys.cols = n;

    auto xs = std::vector<float>(n * m);
    auto ys = std::vector<float>(n * m);

    auto i = std::begin(xs);
    auto j = std::begin(ys);

    for (auto k = 0; k < n; ++k) {
      point::shufflePoints(ps);
      std::transform(begin(ps), end(ps), i, [](const auto& p) { return p.x; });
      std::transform(begin(ps), end(ps), j, [](const auto& p) { return p.y; });
      i += m;
      j += m;
    }

    matrix::transpose(n, m, xs, pop.xs);
    matrix::transpose(n, m, ys, pop.ys);
  }
}
