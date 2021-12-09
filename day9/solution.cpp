#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <numeric>
#include <algorithm>

std::vector<std::vector<uint8_t>> get_input(const std::string& file_name) {
    std::ifstream fs {file_name};
    if (!fs.is_open()) {
        throw std::runtime_error {"Can't open the file"};
    }

    std::vector<std::vector<uint8_t>> input {};
    std::string current_line;
    while (std::getline(fs, current_line)) {
        input.push_back({});
        for (char c : current_line) {
            input.back().push_back(c - '0');
        }
    }

    return input;
}

template <typename Input>
int solve1(Input& input) {
    auto check = [&input](int row, int col) {
        int upper = (row == 0 ? 10 : input[row - 1][col]);
        int lower = (row == input.size() - 1 ? 10 : input[row + 1][col]);
        int righter = (col == input[0].size() - 1 ? 10 : input[row][col + 1]);
        int lefter = (col == 0 ? 10 : input[row][col - 1]);
        int current = input[row][col];

        if (current < upper && current < lower && current < righter && current < lefter) {
            return true;
        }
        return false;
    };

    int count = 0;
    for (int i = 0; i < input.size(); ++i) {
        for (int j = 0; j < input[0].size(); ++j) {
            if (check(i, j)) {
                count += input[i][j] + 1;
            }
        }
    }

    return count;
}

template <typename Input>
int solve2(Input& input) {
    auto dfs = [&input](int row, int col) {
        std::queue<std::pair<int, int>> q {};
        q.push({row, col});
        int count = 0;
        while (!q.empty()) {
            auto current = q.front(); q.pop();
            int row_ = current.first;
            int col_ = current.second;
            if (row_ < 0 || row_ >= input.size() || col_ < 0 || col_ >= input[0].size() || input[row_][col_] == 9) {
                continue;
            }
            input[row_][col_] = 9;
            ++count;
            q.push({row_ + 1, col_});
            q.push({row_ - 1, col_});
            q.push({row_, col_ + 1});
            q.push({row_, col_ - 1});
        }
        return count;
    };

    int highest_vals[3] {};
    for (int row = 0; row < input.size(); ++row) {
        for (int col = 0; col < input[0].size(); ++col) {
            if (input[row][col] == 9) {
                continue;
            }
            int current_count = dfs(row, col);
            // TODO: use a for loop
            if (current_count > highest_vals[0]) {
                std::rotate(std::rbegin(highest_vals), std::rbegin(highest_vals) + 1, std::rend(highest_vals));
                highest_vals[0] = current_count;
            } else if (current_count > highest_vals[1]) {
                highest_vals[2] = highest_vals[1];
                highest_vals[1] = current_count;
            } else if (current_count > highest_vals[2]) {
                highest_vals[2] = current_count;
            }
        }
    }

    return std::accumulate(std::begin(highest_vals), std::end(highest_vals), 1, std::multiplies<int>());
}

int main() {
    auto input = get_input("input.txt");
    std::cout << solve1(input) << '\n';
    std::cout << solve2(input) << '\n';

    return 0;
}