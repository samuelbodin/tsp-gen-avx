#pragma once
#include <vector>
#include "jio.hpp"
#include <iomanip>

namespace matrix {
  struct Matrix {
    int rows;
    int cols;
    mutable float* data;

    Matrix(const int rows, const int cols) {
      this->rows = rows;
      this->cols = cols;
      data = static_cast<float*>(_mm_malloc(sizeof(float) * rows * cols, 32));
      std::fill(data, data + rows * cols, 0.f);
    }

    ~Matrix() {
      _mm_free(data);
    }

    auto at(const int i, const int j) const -> float& {
      return data[i * cols + j];
    }
  };

  inline auto toStr(Matrix& a) -> std::string {
    auto ss = std::stringstream();
    auto s = std::string();
    s.resize(10);
    ss << "Matrix {\n";
    for (auto i = 0; i < a.rows; ++i) {
      ss << "   ";
      for (auto j = 0; j < a.cols; ++j) {
        std::fill(begin(s), end(s), '0');
        s = std::to_string(a.at(i, j));
        ss << s.substr(0, 10);
        if (j < a.cols - 1) {
          ss << ", ";
        }
      }
      ss << '\n';
    }
    ss << '}';
    return ss.str();
  }

  inline auto transpose(Matrix& a, Matrix& b) -> void {
    b.rows = a.cols;
    b.cols = a.rows;
    for (auto i = 0; i < a.rows; ++i) {
      for (auto j = 0; j < a.cols; ++j) {
        b.at(j, i) = a.at(i, j);
      }
    }
  }

  inline auto transpose(const int rows, const int cols, std::vector<float> a, const Matrix& b) -> void {
    for (auto i = 0; i < rows; ++i) {
      for (auto j = 0; j < cols; ++j) {
        b.at(j, i) = a[i * cols + j];
      }
    }
  }
}
