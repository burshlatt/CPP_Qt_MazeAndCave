#ifndef MAZE_MODEL_CAVE_GENERATOR_HPP
#define MAZE_MODEL_CAVE_GENERATOR_HPP

#include <random>
#include <algorithm>

#include "cave.hpp"

namespace s21 {
struct Settings {
    std::size_t rows{};
    std::size_t cols{};
    std::size_t live_chance{};
    std::size_t generation_count{};
    std::pair<std::size_t, std::size_t> live_limit;
    std::pair<std::size_t, std::size_t> born_limit;
};

class Generator {
private:
    using size_type = std::size_t;

public:
    Generator() = default;
    ~Generator() = default;

public:
    static Cave Generate(const Settings &sett) {
        Cave cave{InitializeRandom(sett.rows, sett.cols, sett.live_chance)};
        size_type generation = 0;

        while (++generation < sett.generation_count)
            Generate(sett, cave);

        return cave;
    }

    static Cave& Generate(const Settings &sett, Cave &cave) {
        Cave tmp = cave;

        for (size_type row = 0; row != sett.rows; ++row) {
            for (size_type col = 0; col != sett.cols; ++col) {
                size_type count = CountLiveNeighbours(row, col, cave);

                if (cave(row, col) == 1 && (count < sett.live_limit.first || count > sett.live_limit.second))
                    tmp(row, col) = 0;
                else if (cave(row, col) == 0 && (count >= sett.born_limit.first && count <= sett.born_limit.second))
                    tmp(row, col) = 1;
            }
        }

        cave = std::move(tmp);

        return cave;
    }

private:
    static Cave InitializeRandom(size_type rows, size_type cols, size_type live_chance) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<size_type> distribution(0, 100);

        Cave::data_type generation(rows * cols);

        std::generate(generation.begin(), generation.end(), [&distribution, &gen, live_chance] {
            size_type chance = distribution(gen);

            return chance <= live_chance ? 1 : 0;
        });

        return Cave(generation, rows, cols);
    }

    static size_type CountLiveNeighbours(size_type row, size_type col, const Cave &cave) {
        size_type count = 0;
        for (auto item : {
            cave(row, col - 1), cave(row, col + 1), cave(row - 1, col),
            cave(row + 1, col), cave(row - 1, col - 1), cave(row - 1, col + 1),
            cave(row + 1, col - 1), cave(row + 1, col + 1)
        }) {
            if (item != 0) count++;
        }

        return count;
    }
};
}  // namespace s21

#endif  // MAZE_MODEL_CAVE_GENERATOR_HPP
