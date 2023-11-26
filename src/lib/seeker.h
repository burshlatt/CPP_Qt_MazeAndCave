#ifndef A1_MAZE_1_MODEL_LEE_ALGORITHM_H
#define A1_MAZE_1_MODEL_LEE_ALGORITHM_H

#include <list>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

#include "container.h"

namespace s21 {
class Seeker {
    public:
        using Coord = std::pair<int, int>;
        using Way = std::list<std::pair<int, int>>;
        using Matrix = std::vector<std::vector<int>>;
        using ContPoint = MazeContainer::Point;

        Seeker() = default;
        ~Seeker() = default;

        Way Solve(int rows, int cols, Matrix v_walls, Matrix h_walls, Coord start, Coord end) {
            rows_ = rows;
            cols_ = cols;
            maze_ = MazeContainer(rows, cols, v_walls, h_walls);
            return Solve(start, end);
        }

        Way Solve(Coord start, Coord end) {
            SetStart(start);
            WaveMaze();
            return Solve(end);
        }

        Way Solve(Coord end) {
            SetEnd(end);
            ShortestPath();
            return way_;
        }

    private:
        int cols_{};
        int rows_{};

        Way way_{};
        Matrix waves_{};
        ContPoint end_{};
        ContPoint start_{};
        MazeContainer maze_{};

        inline int GetWaveValue(ContPoint p) const noexcept {
            return waves_[p.y][p.x];
        }

        inline bool IsUnmarked(ContPoint p) const noexcept {
            return waves_[p.y][p.x] == 0;
        }

        inline void MarkWavePoint(ContPoint p, int value) noexcept {
            waves_[p.y][p.x] = value;
        }

        void МarkNeighbors(ContPoint p) noexcept {
            int value = GetWaveValue(p) + 1;
            for (auto i : maze_.GetNeighbors(p))
                if (IsUnmarked(i))
                    MarkWavePoint(i, value);
        }

        std::pair<ContPoint, bool> WayBack(ContPoint p) {
            way_.emplace_front(p.x, p.y);
            for (auto i : maze_.GetNeighbors(p))
                if (GetWaveValue(p) == GetWaveValue(i) + 1)
                    return {i, true};
            return {p, false};
        }

        inline void SetStart(const Coord& start) noexcept {
            start_ = start;
        }

        inline void SetEnd(const Coord& end) noexcept {
            end_ = end;
        }

        void WaveMaze() {
            waves_ = Matrix(rows_, std::vector<int>(cols_, 0));
            MarkWavePoint(start_, 1);
            int value = 1;
            bool exit_flag = false;
            while (!exit_flag) {
                exit_flag = true;
                for (int i = 0; i < rows_; i++) {
                    for (int j = 0; j < cols_; j++) {
                        ContPoint p({j, i});
                        if (IsUnmarked(p))
                            exit_flag = false;
                        if (!IsUnmarked(p) && GetWaveValue(p) == value)
                            МarkNeighbors(p);
                    }
                }
                ++value;
            }
        }

        void ShortestPath() {
            way_.clear();
            ContPoint current = end_;
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
