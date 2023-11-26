#ifndef A1_MAZE_1_MODEL_LEE_ALGORITHM_H
#define A1_MAZE_1_MODEL_LEE_ALGORITHM_H

#include <fstream>
#include <iostream>
#include <list>
#include <sstream>

#include "maze_container.h"

namespace s21 {
class LeeAlgorithm {
 public:
  using matrix = std::vector<std::vector<int>>;
  using point = MazeContainer::Point;
  LeeAlgorithm() = default;

  std::list<std::pair<int, int>> Solve(std::string filepath,
                                       std::pair<int, int> start,
                                       std::pair<int, int> end) {
    this->SetMaze(filepath);
    return this->Solve(start, end);
  }
  std::list<std::pair<int, int>> Solve(int rows, int cols, matrix right_wall,
                                       matrix bottom_wall,
                                       std::pair<int, int> start,
                                       std::pair<int, int> end) {
    this->rows_ = rows;
    this->cols_ = cols;
    this->maze_ = MazeContainer(rows, cols, right_wall, bottom_wall);
    return this->Solve(start, end);
  }
  std::list<std::pair<int, int>> Solve(std::pair<int, int> start,
                                       std::pair<int, int> end) {
    this->SetStart(start);
    this->WaveMaze();
    return this->Solve(end);
  }
  std::list<std::pair<int, int>> Solve(std::pair<int, int> end) {
    this->SetEnd(end);
    this->ShortestPath();
    return this->path_;
  }

 private:
  point start_{};
  point end_{};
  MazeContainer maze_{};
  int cols_{};
  int rows_{};
  matrix waves_{};
  std::list<std::pair<int, int>> path_{};
  int GetWaveValue(point p) { return this->waves_[p.y][p.x]; }
  bool IsUnmarked(point p) { return this->waves_[p.y][p.x] == 0; }
  void MarkWavePoint(point p, int value) { this->waves_[p.y][p.x] = value; }
  void МarkNeighbors(point p) {
    int value = GetWaveValue(p) + 1;
    for (auto i : this->maze_.GetNeighbors(p)) {
      if (IsUnmarked(i)) {
        MarkWavePoint(i, value);
      }
    }
  };
  std::pair<point, bool> WayBack(point p) {
    this->path_.push_front({p.x, p.y});
    for (auto i : this->maze_.GetNeighbors(p)) {
      if (GetWaveValue(p) == GetWaveValue(i) + 1) {
        return {i, true};
      }
    }
    return {p, false};
  };

  void SetMaze(MazeContainer maze) {
    this->maze_ = maze;
    this->cols_ = maze.GetCols();
    this->rows_ = maze.GetRows();
  }

  void SetMaze(std::string filepath) {
    this->maze_ = MazeContainer(filepath);
    this->cols_ = this->maze_.GetCols();
    this->rows_ = this->maze_.GetRows();
  }

  void SetStart(std::pair<int, int> start) { this->start_ = start; }

  void SetEnd(std::pair<int, int> end) { this->end_ = end; }

  void WaveMaze() {
    this->waves_ = matrix(this->rows_, std::vector<int>(this->cols_, 0));
    MarkWavePoint(this->start_, 1);
    int value = 1;
    bool exit_flag = false;
    while (!exit_flag) {
      exit_flag = true;
      for (int i = 0; i < this->rows_; i++) {
        for (int j = 0; j < this->cols_; j++) {
          point p({j, i});
          if (IsUnmarked(p)) {
            exit_flag = false;
          }
          if (!IsUnmarked(p) && GetWaveValue(p) == value) {
            МarkNeighbors(p);
          }
        }
      }
      ++value;
    }
  };
  void ShortestPath() {
    this->path_.clear();
    point current = this->end_;
    bool exit_flag = true;
    do {
      auto step_back = WayBack(current);
      current = step_back.first;
      exit_flag = step_back.second;
    } while (exit_flag);
  }
};
}  // namespace s21

#endif  // A1_MAZE_1_MODEL_LEE_ALGORITHM_H
