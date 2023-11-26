#ifndef A1_MAZE_1_MODEL_MAZE_BUILDER_H
#define A1_MAZE_1_MODEL_MAZE_BUILDER_H

#include <time.h>

#include <fstream>
#include <random>
#include <vector>

namespace s21 {
class MazeBuilder {
 public:
  using Matrix = std::vector<std::vector<int>>;

  MazeBuilder() { InitEmptyMatrices(); };
  ~MazeBuilder() = default;

  Matrix& GetVerticalWalls() { return vertical_walls_; }
  Matrix& GetHorizontalWalls() { return horizontal_walls_; }

  void BuildMaze(int rows, int cols) {
    if (rows < 1 or cols < 1)
      throw std::runtime_error("Incorrect rows or cols value");
    rows_ = rows;
    cols_ = cols;
    InitEmptyMatrices();
    AddNewValuesToLine(0);
    for (int row = 0; row < rows_; ++row) {
      SetRightWallsForLine(row);
      SetBottomWallsForLine(row);
      NormalizeMatrixLine(row);
    }
  }

 private:
  int rows_ = 5;
  int cols_ = 5;
  Matrix maze_matrix_{};

  Matrix vertical_walls_{};
  Matrix horizontal_walls_{};

  void AddNewValuesToLine(int row) {
    static int current_num = 1;
    for (int num = 0; num < cols_; ++num, ++current_num) {
      if (maze_matrix_[row][num] == 0) maze_matrix_[row][num] = current_num;
    }
  }

  void SetRightWallsForLine(int row) {
    std::vector<int> random_line = GetRandomBinLine();
    for (int i = 0; i < cols_; i++) {
      if (random_line[i] == 1) {
        vertical_walls_[row][i] = 1;
      } else {
        if (i != cols_ - 1) {
          if (maze_matrix_[row][i] == maze_matrix_[row][i + 1]) {
            vertical_walls_[row][i] = 1;
          } else {
            ReplaceRightNums(row, i);
          }
        }
      }
    }
  }

  void ReplaceRightNums(int row, int current_possition) {
    int right_num = maze_matrix_[row][current_possition + 1];
    for (auto it = maze_matrix_[row].begin(); it < maze_matrix_[row].end();
         ++it) {
      if (*it == right_num) {
        *it = maze_matrix_[row][current_possition];
      }
    }
  }

  void SetBottomWallsForLine(int row) {
    std::vector<int> random_line = GetRandomBinLine();
    for (int i = 0; i < cols_; i++) {
      if (random_line[i] == 0) {
        horizontal_walls_[row][i] = 0;
      } else {
        int current_num = maze_matrix_[row][i];
        int count_of_not_bottom_wall = 0;
        for (int j = 0; j < cols_ && count_of_not_bottom_wall < 2; ++j) {
          if (maze_matrix_[row][j] == current_num &&
              horizontal_walls_[row][j] != 1) {
            ++count_of_not_bottom_wall;
          }
        }
        if (count_of_not_bottom_wall > 1) horizontal_walls_[row][i] = 1;
      }
    }
  }

  void InitEmptyMatrices() {
    Matrix empty_matrix(rows_, std::vector<int>(cols_, 0));
    maze_matrix_ = empty_matrix;
    vertical_walls_ = empty_matrix;
    horizontal_walls_ = empty_matrix;
  }

  std::vector<int> GetRandomBinLine() {
    static bool srand_called = false;
    if (!srand_called) {
      srand(time(0));
      srand_called = true;
    }
    std::vector<int> res(cols_);
    for (auto it = res.begin(); it != res.end(); ++it) {
      *it = (rand() % 2);
    }
    return res;
  }

  void NormalizeMatrixLine(int row) {
    if (row != rows_ - 1) {
      maze_matrix_[row + 1] = maze_matrix_[row];
      for (int i = 0; i < cols_; ++i) {
        if (horizontal_walls_[row][i] == 1) {
          maze_matrix_[row + 1][i] = 0;
        }
      }
      AddNewValuesToLine(row + 1);
    } else {
      for (int i = 0; i < cols_; ++i) {
        horizontal_walls_[row][i] = 1;
        if (maze_matrix_[row][i] != maze_matrix_[row][i + 1]) {
          vertical_walls_[row][i] = 0;
          ReplaceRightNums(row, i);
        }
      }
    }
  }

  std::string MakeContent() {
    std::string content =
        std::to_string(rows_) + " " + std::to_string(cols_) + '\n';
    content += MatrixToString(vertical_walls_);
    content += '\n';
    content += MatrixToString(horizontal_walls_);
    return content;
  }

  std::string MatrixToString(Matrix matrix) {
    std::string result = {};
    for (auto row_it = matrix.begin(); row_it < matrix.end(); ++row_it) {
      for (auto col_it = (*row_it).begin(); col_it < (*row_it).end();
           ++col_it) {
        result += std::to_string(*col_it);
        if (col_it != --(*row_it).end()) result += " ";
      }
      result += '\n';
    }
    return result;
  }
};
}  // namespace s21

#endif  // A1_MAZE_1_MODEL_MAZE_BUILDER_H
