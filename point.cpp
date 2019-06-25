#include "point.hpp"
#include <random>
#include <string>
#include <algorithm>

auto point::scale(const point::Point& p, float s) -> point::Point {
  return {p.x * s, p.y * s};
}

auto point::add(const point::Point& p, const point::Point& q) -> point::Point {
  return {p.x + q.x, p.y + q.y};
}

auto point::sub(const point::Point& p, const point::Point& q) -> point::Point {
  return {p.x - q.x, p.y - q.y};
}

auto point::distance(const point::Point& p, const point::Point& q) -> float {
  auto x = q.x - p.x;
  auto y = q.y - p.y;
  return sqrt(x * x + y * y);
}

auto point::equals(const point::Point& p, const point::Point& q) -> bool {
  return p.x == q.x && p.y == q.y;
}

auto randomFloat() -> float {
  static std::random_device rd;
  static auto rng = std::mt19937(rd());
  static auto dis =
      std::uniform_real_distribution<>(0.f, 1.f);
  return static_cast<float>(dis(rng));
}

auto point::randomPoints(int n, const Point& mid, float r)
-> std::vector<Point> {
  auto pts = std::vector<Point>(n);
  for (auto& p : pts) {
    p = {mid.x + randomFloat() * 2 * r - r,
      mid.y + randomFloat() * 2 * r - r};
  }
  return pts;
}

auto point::mockPoints(int n, point::Point start) -> std::vector<point::Point> {
  //static std::random_device rd;
  //static auto rng = std::mt19937(rd());
  auto ps = std::vector<point::Point>(n);
  for (auto i = 0; i < n; ++i) {
    ps[i].x = start.x + i;
    ps[i].y = start.y + i;
  }
  //std::shuffle(next(begin(ps)), end(ps), rng);
  return ps;
}

auto point::shufflePoints(std::vector<Point>& ps) -> void {
  static std::random_device rd;
  static auto rng = std::mt19937(rd());
  std::shuffle(begin(ps), end(ps), rng);
}

auto point::toStr(const Point& p) -> std::string {
  return "Point{x: " + std::to_string(p.x)
      + ", y: " + std::to_string(p.y) + '}';
}
