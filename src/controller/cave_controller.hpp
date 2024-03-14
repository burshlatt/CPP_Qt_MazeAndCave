#ifndef MAZE_AND_CAVE_CONTROLLER_CAVE_CONTROLLER_HPP
#define MAZE_AND_CAVE_CONTROLLER_CAVE_CONTROLLER_HPP

#include <string_view>

#include "cave.hpp"

namespace s21 {
class CaveController {
public:
    using size_type = Cave::size_type;

public:
    CaveController() = default;
    ~CaveController() = default;

public:
    inline Settings GetSettings() const {
        return cave_.GetSettings();
    }

public:
    void Generate(const Settings &sett) {
        cave_.Generate(sett);
    }

    void GenerateStep() {
        cave_.GenerateStep();
    }

public:
    void SaveConfig(std::string_view path) const {
        cave_.SaveConfig(path);
    }

    void LoadConfig(std::string_view path) {
        cave_.LoadConfig(path);
    }

public:
    size_type& operator()(size_type row, size_type col) {
        return cave_(row, col);
    }

    size_type operator()(size_type row, size_type col) const {
        return cave_(row, col);
    }

private:
    Cave cave_;
};
} // namespace s21

#endif // MAZE_AND_CAVE_CONTROLLER_CAVE_CONTROLLER_HPP
