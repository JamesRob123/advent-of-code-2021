#include <iostream>
#include <array>
#include <fstream>
#include <ranges>
#include <queue>

std::array<std::array<uint8_t, 10>, 10> get_input(const std::string& file_name) {
    std::ifstream fs {file_name};
    if (!fs.is_open()) {
        throw std::runtime_error {"Can't open the file"};
    }

    std::array<std::array<uint8_t, 10>, 10> input {};
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            char c;
            fs >> c;
            input[i][j] = c - '0';
        }
    }

    return input;
}

template <typename Input>
int solve1(Input input) {
    // Requires c++20
    int count = 0;
    for (int i : std::views::iota(0, 100)) {
        // simulating step i
        // increase alla levels by 1
        std::array<std::array<bool, 10>, 10> done {};
        std::queue<std::pair<int, int>> q {};
        for (int row = 0; row < input.size(); ++row) {
            for (int col = 0; col < input[row].size(); ++col) {
                ++input[row][col];
                if (input[row][col] > 9) {
                    done[row][col] = true;
                    q.push(std::make_pair(row, col));
                    ++count;
                }
            }
        }

        // flash all squids with a energy level greater than 9
        while (!q.empty()) {
            auto current = q.front(); q.pop();
            int current_row = current.first, current_col = current.second;
            for (int row = std::max(0, current_row - 1); row <= std::min((int)(input.size() - 1), current_row + 1); ++row) {
                for (int col = std::max(0, current_col - 1); col <= std::min((int)(input[i].size() - 1), current_col + 1); ++col) {
                    if (done[row][col]) {
                        continue;
                    }
                    ++input[row][col];
                    if (input[row][col] > 9) {
                        done[row][col] = true;
                        q.push(std::make_pair(row, col));
                        ++count;
                    }
                }
            }
        }

        // set every squid, which flashed, to energy level 0
        for (int row = 0; row < input.size(); ++row) {
            for (int col = 0; col < input[i].size(); ++col) {
                if (done[row][col]) {
                    input[row][col] = 0;
                }
            }
        }
    }

    return count;
}

template <typename Input>
int solve2(Input input) {
    // Requires c++20
    for (int i = 1; ; ++i) {
        // simulating step i
        // increase alla levels by 1
        std::array<std::array<bool, 10>, 10> done {};
        std::queue<std::pair<int, int>> q {};
        for (int row = 0; row < input.size(); ++row) {
            for (int col = 0; col < input[row].size(); ++col) {
                ++input[row][col];
                if (input[row][col] > 9) {
                    done[row][col] = true;
                    q.push(std::make_pair(row, col));
                }
            }
        }

        // flash all squids with a energy level greater than 9
        while (!q.empty()) {
            auto current = q.front(); q.pop();
            int current_row = current.first, current_col = current.second;
            for (int row = std::max(0, current_row - 1); row <= std::min((int)(input.size() - 1), current_row + 1); ++row) {
                for (int col = std::max(0, current_col - 1); col <= std::min((int)(input[i].size() - 1), current_col + 1); ++col) {
                    if (done[row][col]) {
                        continue;
                    }
                    ++input[row][col];
                    if (input[row][col] > 9) {
                        done[row][col] = true;
                        q.push(std::make_pair(row, col));
                    }
                }
            }
        }

        // set every squid, which flashed, to energy level 0
        bool is_in_sync = true;
        for (int row = 0; row < input.size(); ++row) {
            for (int col = 0; col < input[i].size(); ++col) {
                if (done[row][col]) {
                    input[row][col] = 0;
                    continue;
                }
                is_in_sync = false;
            }
        }
        if (is_in_sync) {
            return i;
        }
    }

    return -1;
}

int main() {
    auto input = get_input("input.txt");
    std::cout << solve1(input) << '\n';
    std::cout << solve2(input) << '\n';

    return 0;
}