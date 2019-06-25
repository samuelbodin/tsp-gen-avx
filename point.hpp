#pragma once
#include <vector>

namespace point {
  struct Point;

  auto scale(const Point& p, float s) -> Point;
  auto add(const Point& p, const Point& q) -> Point;
  auto sub(const Point& p, const Point& q) -> Point;
  auto distance(const Point& p, const Point& q) -> float;
  auto equals(const point::Point& p, const point::Point& q) -> bool;
  auto randomPoints(int n, const Point& mid, float r)
  -> std::vector<Point>;

  auto mockPoints(int n, point::Point start) -> std::vector<point::Point>;

  auto shufflePoints(std::vector<Point>& ps) -> void;

  auto toStr(const Point& p) -> std::string;

  template <typename T>
  auto toStringMany(const T& vec) -> std::string {
    std::string s = "Points{\n";

    for (const auto& p : vec) {
      s += "    " + point::toStr(p) + '\n';
    }

    return s + "}";
  }

  struct Point {
    float x;
    float y;

    auto operator+(const Point& other) const -> Point {
      return add(*this, other);
    }

    auto operator-(const Point& other) const -> Point {
      return sub(*this, other);
    }

    auto operator*(float s) const -> Point {
      return scale(*this, s);
    }

    auto operator/(float s) const -> Point {
      return scale(*this, 1.f / s);
    }

    auto operator+=(const Point& other) -> Point& {
      *this = add(*this, other);
      return *this;
    }

    auto operator==(const Point& other) const -> bool {
      return equals(*this, other);
    }
  };
}
