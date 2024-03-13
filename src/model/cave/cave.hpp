#ifndef MAZE_MODEL_CAVE_CAVE_HPP
#define MAZE_MODEL_CAVE_CAVE_HPP

#include <vector>
#include <algorithm>

namespace s21 {
class Cave {
public:
    using size_type = std::size_t;
    using data_type = std::vector<size_type>;

public:
    Cave(data_type gen, size_type rows, size_type cols) :
        data_(gen),
        rows_(rows),
        cols_(cols)
    {}

public:
    bool IsGood() const {
        if (rows_ == 0 || rows_ > 150 || cols_ == 0 || cols_ > 150)
            return false;

        if (rows_ * cols_ != data_.size())
            return false;

        if (!std::all_of(data_.begin(), data_.end(), [](size_type i) { return i == 1 || i == 0; }))
            return false;

        return true;
    }

    void Clear() noexcept {
        data_.clear();
        rows_ = cols_ = 0;
    }

public:
    size_type &operator()(size_type row, size_type col) {
        return data_[row * cols_ + col];
    }

    size_type operator()(size_type row, size_type col) const {
        if (row >= rows_ || col >= cols_)
            return 1;

        return data_[row * cols_ + col];
    }

private:
    data_type data_;
    size_type rows_{};
    size_type cols_{};
};
}  // namespace s21

#endif  // MAZE_MODEL_CAVE_CAVE_HPP
