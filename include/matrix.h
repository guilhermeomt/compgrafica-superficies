#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <iostream>

class Matrix {
private:
  int _rows;
  int _cols;
public:
  float** values;

  Matrix();
  ~Matrix();
  Matrix(int rows, int cols);
  Matrix(float v[3]);

  inline int rows() const { return _rows; }
  inline int cols() const { return _cols; }

  static Matrix identity(int rows, int cols);
  static float* to_array(const Matrix& m);

  void set(int row, int col, float value) {
    values[row][col] = value;
  }

  float get(int row, int col) const {
    return values[row][col];
  }

  Matrix operator*(const Matrix& other) const;
  Matrix operator*(float scalar) const;
  Matrix& operator=(const Matrix& other);

  void print() const {
    for (int i = 0; i < _rows; i++) {
      for (int j = 0; j < _cols; j++) {
        std::cout << values[i][j] << " ";
      }
      std::cout << std::endl;
    }
  }
};

#endif // __MATRIX_H__