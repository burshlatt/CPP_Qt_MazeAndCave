#ifndef A1_MAZE_1_MODEL_MAZE_CONTAINER_H
#define A1_MAZE_1_MODEL_MAZE_CONTAINER_H

#include <fstream>
#include <limits>
#include <list>
#include <sstream>
#include <vector>

namespace s21 {
struct MazeContainer {
 public:
  using matrix = std::vector<std::vector<int>>;
  struct Point {
    Point() = default;
    Point(std::pair<int, int> p) {
      this->x = p.first;
      this->y = p.second;
    };
    bool operator==(Point other) {
      return other.x == this->x && other.y == this->y;
    }
    bool operator!=(Point other) { return !(other == *this); }
    int x{};
    int y{};
  };

  struct district {
    district(int x, int y) {
      this->top = Point(std::pair<int, int>({x, y - 1}));
      this->bottom = Point(std::pair<int, int>({x, y + 1}));
      this->left = Point(std::pair<int, int>({x - 1, y}));
      this->right = Point(std::pair<int, int>({x + 1, y}));
    };
    district(Point p) {
      this->top = Point(std::pair<int, int>({p.x, p.y - 1}));
      this->bottom = Point(std::pair<int, int>({p.x, p.y + 1}));
      this->left = Point(std::pair<int, int>({p.x - 1, p.y}));
      this->right = Point(std::pair<int, int>({p.x + 1, p.y}));
    };
    Point top{};
    Point bottom{};
    Point right{};
    Point left{};
  };

  MazeContainer() = default;

  MazeContainer(std::string filepath) {
    std::ifstream myfile(filepath);

    myfile >> this->rows >> this->cols;

    this->right_wall = ParseMatrix(myfile, this->rows, this->cols);

    myfile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    this->bottom_wall = ParseMatrix(myfile, this->rows, this->cols);
  };

  MazeContainer(int rows, int cols, matrix right_wall, matrix bottom_wall) {
    this->rows = rows;
    this->cols = cols;
    this->right_wall = right_wall;
    this->bottom_wall = bottom_wall;
  };

  ~MazeContainer() = default;

  // Getters
  int GetCols() const { return cols; }

  int GetRows() const { return rows; }

  const matrix& GetRightWall() const { return right_wall; }

  const matrix& GetBottomWall() const { return bottom_wall; }

  bool IsPointWithBottomWall(Point p) {
    return this->bottom_wall[p.y][p.x] == 1;
  }

  int IsPointWithRightWall(Point p) { return this->right_wall[p.y][p.x] == 1; }
  bool IsInMaze(Point p) {
    return (((p.x - 0) | (this->cols - p.x - 1)) >= 0) &&
           (((p.y - 0) | (this->rows - p.y - 1)) >= 0);
  }
  std::list<Point> GetNeighbors(Point p) {
    std::list<Point> neighbors{};
    district d(p);

    if (IsInMaze(d.top) && !IsPointWithBottomWall(d.top)) {
      neighbors.push_back(d.top);
    }

    if (IsInMaze(d.bottom) && !IsPointWithBottomWall(p)) {
      neighbors.push_back(d.bottom);
    }

    if (IsInMaze(d.left) && !IsPointWithRightWall(d.left)) {
      neighbors.push_back(d.left);
    }

    if (IsInMaze(d.right) && !IsPointWithRightWall(p)) {
      neighbors.push_back(d.right);
    }

    return neighbors;
  }

 private:
  int cols{};
  int rows{};
  matrix right_wall{};
  matrix bottom_wall{};
  matrix ParseMatrix(std::ifstream& input, int rows, int cols) {
    matrix matrix(rows, std::vector<int>(cols, 0));

    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
        input >> matrix[i][j];
      }
    }
    return matrix;
  }
};
}  // namespace s21

#endif  //  A1_MAZE_1_MODEL_MAZE_CONTAINER_H
