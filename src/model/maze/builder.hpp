#ifndef MAZE_AND_CAVE_MODEL_MAZE_BUILDER_HPP
#define MAZE_AND_CAVE_MODEL_MAZE_BUILDER_HPP

#include <filesystem>
#include <fstream>
#include <random>
#include <vector>

namespace fs = std::filesystem;

namespace s21 {
class Builder {
public:
    using Matrix = std::vector<std::vector<int>>;

public:
    Builder() { InitEmptyMatrices(); };
    ~Builder() = default;

public:
    inline int GetRows() const {
        return rows_;
    }

    inline int GetCols() const {
        return cols_;
    }

    inline const Matrix& GetVerticalWalls() const noexcept {
        return vertical_walls_;
    }

    inline const Matrix& GetHorizontalWalls() const noexcept {
        return horizontal_walls_;
    }

public:
    void BuildMaze(int rows, int cols) {
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

    void SaveConfig(std::string_view path) const {
        std::ofstream file(fs::path(path), std::ios::out);

        if (!file.is_open())
            throw std::ios_base::failure("Cannot create file");

        file << rows_ << ' ' << cols_ << '\n';

        for (const auto& row : vertical_walls_) {
            for (auto val : row)
                file << val << ' ';

            file << '\n';
        }

        file << '\n';

        for (const auto& col : horizontal_walls_) {
            for (auto val : col)
                file << val << ' ';

            file << '\n';
        }
    }

    void LoadConfig(std::string_view path) {
        std::ifstream file(fs::path(path), std::ios::in);

        if (!file.is_open())
            throw std::ios_base::failure("Cannot open file");

        file >> rows_ >> cols_;

        int value = 0;
        
        for (int i = 0; i < 2; ++i) {
            Matrix matrix(rows_, std::vector<int>(cols_));

            for (int row = 0; row < rows_; ++row)
                for (int col = 0; col < cols_; ++col)
                    file >> matrix[row][col];

            if (i == 0)
                vertical_walls_ = std::move(matrix);
            else
                horizontal_walls_ = std::move(matrix);
        }
    }

private:
    void AddNewValuesToLine(int row) noexcept {
        static int current_num{1};
        for (int num = 0; num < cols_; ++num, ++current_num)
            if (maze_matrix_[row][num] == 0)
                maze_matrix_[row][num] = current_num;
    }

    void SetRightWallsForLine(int row) {
        std::vector<int> random_line{GetRandomBinLine()};
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

    void ReplaceRightNums(int row, int current_possition) noexcept {
        int right_num{maze_matrix_[row][current_possition + 1]};

        for (auto& it : maze_matrix_[row])
            if (it == right_num)
                it = maze_matrix_[row][current_possition];
    }

    void SetBottomWallsForLine(int row) {
        std::vector<int> random_line{GetRandomBinLine()};
        for (int i = 0; i < cols_; i++) {
            if (random_line[i] == 0) {
                horizontal_walls_[row][i] = 0;
            } else {
                int current_num = maze_matrix_[row][i];
                int count_of_not_bottom_wall = 0;
                for (int j = 0; j < cols_ && count_of_not_bottom_wall < 2; ++j) {
                    if (maze_matrix_[row][j] == current_num && horizontal_walls_[row][j] != 1) {
                        ++count_of_not_bottom_wall;
                    }
                }
                if (count_of_not_bottom_wall > 1)
                    horizontal_walls_[row][i] = 1;
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
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution distribution(0, 1);
        std::vector<int> res(cols_);

        for (int i{}; i < cols_; ++i)
            res[i] = distribution(gen);

        return res;
    }

    void NormalizeMatrixLine(int row) noexcept {
        if (row != rows_ - 1) {
            maze_matrix_[row + 1] = maze_matrix_[row];

            for (int i = 0; i < cols_; ++i)
                if (horizontal_walls_[row][i] == 1)
                    maze_matrix_[row + 1][i] = 0;

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

private:
    int rows_ = 5;
    int cols_ = 5;

    Matrix maze_matrix_{};
    Matrix vertical_walls_{};
    Matrix horizontal_walls_{};
};
}  // namespace s21

#endif  // MAZE_MODEL_MAZE_BUILDER_HPP
