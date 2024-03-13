#ifndef MAZE_MODEL_MAZE_CONTAINER_HPP
#define MAZE_MODEL_MAZE_CONTAINER_HPP

#include <list>
#include <vector>
#include <limits>
#include <fstream>
#include <sstream>

namespace s21 {
class MazeContainer {
    public:
        using Matrix = std::vector<std::vector<int>>;

        struct Point {
            Point() = default;

            Point(std::pair<int, int> p) {
                this->x = p.first;
                this->y = p.second;
            }

            bool operator==(Point other) {
                return other.x == this->x && other.y == this->y;
            }

            bool operator!=(Point other) {
                return !(other == *this);
            }

            int x{};
            int y{};
        };

        struct District {
            District(int x, int y) {
                this->top = Point(std::pair<int, int>({x, y - 1}));
                this->bottom = Point(std::pair<int, int>({x, y + 1}));
                this->left = Point(std::pair<int, int>({x - 1, y}));
                this->right = Point(std::pair<int, int>({x + 1, y}));
            }

            District(Point p) {
                this->top = Point(std::pair<int, int>({p.x, p.y - 1}));
                this->bottom = Point(std::pair<int, int>({p.x, p.y + 1}));
                this->left = Point(std::pair<int, int>({p.x - 1, p.y}));
                this->right = Point(std::pair<int, int>({p.x + 1, p.y}));
            }

            Point top{};
            Point bottom{};
            Point right{};
            Point left{};
        };

        MazeContainer() = default;

        MazeContainer(int rows, int cols, const Matrix& v_walls_, const Matrix& h_walls_) {
            this->rows_ = rows;
            this->cols_ = cols;
            this->v_walls_ = v_walls_;
            this->h_walls_ = h_walls_;
        }

        ~MazeContainer() = default;

        inline bool IsPointWithBottomWall(Point p) const noexcept {
            return this->h_walls_[p.y][p.x] == 1;
        }

        inline int IsPointWithRightWall(Point p) const noexcept {
            return this->v_walls_[p.y][p.x] == 1;
        }

        inline bool IsInMaze(Point p) const noexcept {
            return (((p.x - 0) | (this->cols_ - p.x - 1)) >= 0) && (((p.y - 0) | (this->rows_ - p.y - 1)) >= 0);
        }

        std::list<Point> GetNeighbors(Point p) const {
            std::list<Point> neighbors{};
            District d(p);
            
            if (IsInMaze(d.top) && !IsPointWithBottomWall(d.top))
                neighbors.push_back(d.top);
            if (IsInMaze(d.bottom) && !IsPointWithBottomWall(p))
                neighbors.push_back(d.bottom);
            if (IsInMaze(d.left) && !IsPointWithRightWall(d.left))
                neighbors.push_back(d.left);
            if (IsInMaze(d.right) && !IsPointWithRightWall(p))
                neighbors.push_back(d.right);

            return neighbors;
        }

    private:
        int cols_{};
        int rows_{};
        Matrix v_walls_{};
        Matrix h_walls_{};

        Matrix ParseMatrix(std::ifstream& input, int rows, int cols) const {
            Matrix matrix(rows, std::vector<int>(cols, 0));

            for (int i = 0; i < rows; ++i)
                for (int j = 0; j < cols; ++j)
                    input >> matrix[i][j];

            return matrix;
        }
};
}  // namespace s21

#endif  //  MAZE_MODEL_MAZE_CONTAINER_HPP
