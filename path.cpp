#include "path.hpp"
#include <array>
#include <string>
#include "random.hpp"

auto path::distance(Path& p) -> float {
  auto i = 0u;
  auto j = 1u;

  auto tot = 0.f;

  while(j < p.size()) {
    tot += point::distance(p[i++], p[j++]);
  }

  return tot + point::distance(p.front(), p.back());
}

auto path::idxOfFittest(const int start, const int end, std::vector<float>& ds) -> int {
  auto idx = start;
  auto min = ds[start];

  for(auto i = start + 1; i < end; ++i) {
    if (ds[i] < min) {
      min = ds[i];
      idx = i;
    }
  }
  return idx;
}

auto path::toStr(Path& p) -> std::string {
  auto s = std::string("");
  for (auto it = p.begin(); it != p.end(); ++it) {
    s += '(' + std::to_string((*it).x) + ", " + std::to_string((*it).y) + ")\n";
  }
  return s;
}

auto path::crossover(const Path& a, const Path& b) -> Path {
  auto s = 0u, e = 0u;
  std::tie(s, e) = std::minmax(random::randomInt(0, a.size()), random::randomInt(0, a.size()));
  auto c = std::vector<point::Point>(a.size());
  auto j = 0u;
  for (auto i = 0u; i < c.size(); ++i) {
    if (s <= i && i < e) {
      c[i] = a[i];
    } else {
      while (true) {
        const auto it = std::find(begin(a) + s, begin(a) + e, b[j]);

        if (begin(a) + e <= it) {
          break;
        }
        j++;
      }
      c[i] = b[j++];
    }
  }
  return c;
}

auto path::mutate(const float mutationRate, Path& p) -> void {
  for (auto i = 0u; i < p.size(); ++i) {
    if (random::randomFloat() < mutationRate) {
      const auto j = random::randomInt(0, p.size());
      std::swap(p[i], p[j]);
    }
  }
}
