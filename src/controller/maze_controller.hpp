#ifndef MAZE_AND_CAVE_CONTROLLER_MAZE_CONTROLLER_HPP
#define MAZE_AND_CAVE_CONTROLLER_MAZE_CONTROLLER_HPP

#include <string_view>

#include "seeker.hpp"
#include "builder.hpp"

namespace s21 {
class MazeController {
public:
    using Way = Seeker::Way;
    using Coord = Seeker::Coord;
    using Matrix = Builder::Matrix;

public:
    MazeController() = default;
    ~MazeController() = default;

public:
    inline int GetRows() const {
        return builder_.GetRows();
    }

    inline int GetCols() const {
        return builder_.GetCols();
    }

    inline const Matrix& GetVerticalWalls() const noexcept {
        return builder_.GetVerticalWalls();
    }

    inline const Matrix& GetHorizontalWalls() const noexcept {
        return builder_.GetHorizontalWalls();
    }

public:
    void BuildMaze(int rows, int cols) {
        builder_.BuildMaze(rows, cols);
    }

    Way FindWay(int rows, int cols, Matrix v_walls, Matrix h_walls, Coord start, Coord end) {
        return seeker_.Solve(rows, cols, v_walls, h_walls, start, end);
    }

    void SaveConfig(std::string_view path) const {
        builder_.SaveConfig(path);
    }

    void LoadConfig(std::string_view path) {
        builder_.LoadConfig(path);
    }

private:
    Seeker seeker_;
    Builder builder_;
};
} // namespace s21

#endif // MAZE_AND_CAVE_CONTROLLER_MAZE_CONTROLLER_HPP
