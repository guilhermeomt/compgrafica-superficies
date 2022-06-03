#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <iostream>
#include <cstring>

template <typename T>
class Matrix {
private:
  int _rows;
  int _cols;
public:
  T** values;


  Matrix() {
    _rows = 1;
    _cols = 1;
    values = new T * [1];
  }

  ~Matrix() {
    for (int i = 0; i < _rows; i++) {
      delete[] values[i];
    }
    delete[] values;
  }

  Matrix(int rows, int cols) {
    _rows = rows;
    _cols = cols;
    values = new T * [rows];
    for (int i = 0; i < rows; i++) {
      values[i] = new T[cols];
    }
  }

  Matrix(T v[3]) {
    _rows = 3;
    _cols = 1;
    values = new T * [3];
    for (int i = 0; i < 3; i++) {
      values[i] = new T[1];
      values[i][0] = v[i];
    }
    values[2][0] = 1;
  }

  void set(int row, int col, T value) {
    //std::memcpy(&values[row][col], &value, sizeof(T));
    values[row][col] = value;
  }

  T* get(int row, int col) const {
    return &values[row][col];
  }

  inline int rows() const { return _rows; }
  inline int cols() const { return _cols; }

  Matrix<T> identity(int rows, int cols) {
    Matrix result(rows, cols);
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        result.set(i, j, (i == j) ? 1 : 0);
      }
    }
    return result;
  }

  static T* to_array(const Matrix<T>* m) {
    T* result = {};
    for (int i = 0; i < 4; i++) {
      result = m->get(i, 0);
      printf("%f \n", result[i]);
    }
    return result;
  }

  Matrix<T> operator*(const Matrix<T>& other) const {
    Matrix result(_rows, other.cols());
    for (int i = 0; i < _rows; i++) {
      for (int j = 0; j < other.cols(); j++) {
        T sum = 0;
        for (int k = 0; k < _cols; k++) {
          sum += values[i][k] * other.values[k][j];
        }
        result.set(i, j, sum);
      }
    }
    return result;
  }

  // Matrix<T>& Matrix<T>::operator*(T scalar) const {
  //   Matrix result(_rows, _cols);
  //   for (int i = 0; i < _rows; i++) {
  //     for (int j = 0; j < _cols; j++) {
  //       result.set(i, j, values[i][j] * scalar);
  //     }
  //   }
  //   return result;
  // }

  Matrix<T>& operator=(const Matrix& other) {
    if (_rows != other._rows || _cols != other._cols) {
      for (int i = 0; i < _rows; i++) {
        delete[] values[i];
      }
      delete[] values;
      _rows = other._rows;
      _cols = other._cols;
      values = new T * [_rows];
      for (int i = 0; i < _rows; i++) {
        values[i] = new T[_cols];
      }
    }
    for (int i = 0; i < _rows; i++) {
      for (int j = 0; j < _cols; j++) {
        values[i][j] = other.values[i][j];
      }
    }
    return *this;
  }

};

#endif // __MATRIX_H__