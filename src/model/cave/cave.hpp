#ifndef MAZE_AND_CAVE_MODEL_CAVE_CAVE_HPP
#define MAZE_AND_CAVE_MODEL_CAVE_CAVE_HPP

#include <vector>
#include <random>
#include <fstream>
#include <algorithm>
#include <filesystem>
#include <string_view>

namespace fs = std::filesystem;

namespace s21 {
struct Settings {
    Settings() = default;
    Settings(const Settings&) = default;
    ~Settings() = default;

    std::size_t rows{};
    std::size_t cols{};
    std::size_t live_chance{};
    std::size_t generation_count{};
    std::pair<std::size_t, std::size_t> live_limit;
    std::pair<std::size_t, std::size_t> born_limit;
};

class Cave {
public:
    using size_type = std::size_t;
    using data_type = std::vector<size_type>;

public:
    Cave() = default;
    ~Cave() = default;

public:
    inline Settings GetSettings() const { return settings_; }

public:
    void Generate(const Settings &sett) {
        settings_ = sett;
        RandomInitialize();

        size_type generation = 0;

        while (++generation < sett.generation_count)
            GenerateStep();
    }

    void GenerateStep() {
        Cave tmp{*this};
        Cave cave{*this};

        for (size_type row = 0; row != settings_.rows; ++row) {
            for (size_type col = 0; col != settings_.cols; ++col) {
                size_type count = CountLiveNeighbours(row, col, cave);

                if (cave(row, col) == 1 && (count < settings_.live_limit.first || count > settings_.live_limit.second))
                    tmp(row, col) = 0;
                else if (cave(row, col) == 0 && (count >= settings_.born_limit.first && count <= settings_.born_limit.second))
                    tmp(row, col) = 1;
            }
        }

        *this = std::move(tmp);
    }

public:
    void SaveConfig(std::string_view path) const {
        std::ofstream file(fs::path(path), std::ios::out);

        if (!file.is_open())
            throw std::ios_base::failure("Cannot create file");

        file << settings_.rows << ' ' << settings_.cols << '\n';
        file << settings_.live_chance << ' ' << settings_.generation_count << '\n';
        file << settings_.live_limit.first << ' ' << settings_.live_limit.second << '\n';
        file << settings_.born_limit.first << ' ' << settings_.born_limit.second << '\n';

        for (auto val : data_)
            file << val << ' ';
    }

    void LoadConfig(std::string_view path) {
        std::ifstream file(fs::path(path), std::ios::in);

        if (!file.is_open())
            throw std::ios_base::failure("Cannot open file");

        file >> settings_.rows >> settings_.cols;
        file >> settings_.live_chance >> settings_.generation_count;
        file >> settings_.live_limit.first >> settings_.live_limit.second;
        file >> settings_.born_limit.first >> settings_.born_limit.second;

        size_type size{settings_.rows * settings_.cols};

        data_type tmp_data(size);

        for (size_type i{}; i < size; ++i)
            file >> tmp_data[i];

        data_ = std::move(tmp_data);
    }

public:
    size_type& operator()(size_type row, size_type col) {
        return data_[row * settings_.cols + col];
    }

    size_type operator()(size_type row, size_type col) const {
        if (row >= settings_.rows || col >= settings_.cols)
            return 1;

        return data_[row * settings_.cols + col];
    }

private:
    void RandomInitialize() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<size_type> distribution(0, 100);

        data_type tmp_data(settings_.rows * settings_.cols);

        size_type live_chance{settings_.live_chance};

        std::generate(tmp_data.begin(), tmp_data.end(), [&distribution, &gen, live_chance] {
            size_type chance = distribution(gen);

            return chance <= live_chance ? 1 : 0;
        });

        data_ = std::move(tmp_data);
    }

    size_type CountLiveNeighbours(size_type row, size_type col, const Cave &cave) {
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

private:
    data_type data_;
    Settings settings_;
};
}  // namespace s21

#endif  // MAZE_MODEL_CAVE_CAVE_HPP
