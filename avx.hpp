#pragma once
#include <algorithm>
#include <future>

namespace avx {
  inline auto difference8(float* row1, float* row2) -> __m256 {
    auto u = _mm256_load_ps(row1);
    auto v = _mm256_load_ps(row2);
    return _mm256_sub_ps(v, u);
  }

  inline auto difference(int m, float* row1, float* row2, float* ret) -> void {
    for (auto i = 0; i < m; i += 8) {
      *(__m256*)(ret + i) = difference8(row1 + i, row2 + i);
    }
  }

  inline auto length8(float* xs, float* ys, float* ds) -> __m256 {
    auto uX = _mm256_load_ps(xs);
    auto uY = _mm256_load_ps(ys);
    auto vX = _mm256_mul_ps(uX, uX);
    auto vY = _mm256_mul_ps(uY, uY);
    auto xpy = _mm256_add_ps(vX, vY);
    return _mm256_sqrt_ps(xpy);
  }

  inline auto accumulateLength(int m, float* xs, float* ys, float* ds) -> void {
    for (auto i = 0; i < m; i += 8) {
      auto u = _mm256_load_ps(ds);
      auto v = length8(xs + i, ys + i, ds);
      *(__m256*)(ds + i) = _mm256_add_ps(u, v);
    }
  }

  inline auto distance(const int n, const int m, float* xs, float* ys, float* tempX, float* tempY, float* ds) -> void {
    auto i = 0;
    auto j = 1;

    auto startXs = xs;
    auto startYs = ys;

    while (j < n) {
      difference(m, xs, xs + m, tempX);
      difference(m, ys, ys + m, tempY);
      accumulateLength(m, tempX, tempY, ds);
      ++i;
      ++j;
      xs += m;
      ys += m;
    }
    difference(m, xs, startXs, tempX);
    difference(m, ys, startYs, tempY);
    accumulateLength(m, tempX, tempY, ds);
  }

  inline auto min(const int m, float* arr) -> float {
    auto u = _mm256_load_ps(arr);
    auto i = 8;
    for (i; i < m; i += 8) {
      auto v = _mm256_load_ps(arr + i);
      u = _mm256_min_ps(u, v);
    }
    auto ptr = (float*)&u;
    auto ret = *ptr;
    for (auto j = 1; j < 8; ++j) {
      ret = std::min(ret, *(ptr + j));
    }
    return ret;
  }

  inline auto indexOfMin(int k, float* arr) -> int {
    auto best = min(k, arr);

    auto u = _mm256_set1_ps(best);

    auto offset = 0;

    auto idx = 0UL;

    for (auto i = 0; i < k; i += 8) {
      offset += i;
      auto v = _mm256_load_ps(arr + i);
      auto mask = _mm256_movemask_ps(_mm256_cmp_ps(u, v, _CMP_EQ_OS));
      auto bit = _BitScanForward(&idx, mask);

      if (bit) {
        break;
      }
    }
    return idx + offset;
  }
}
