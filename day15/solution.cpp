#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <tuple>

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

unsigned long long solve1(const std::vector<std::vector<uint8_t>>& input) {
    std::vector<std::vector<unsigned long long>> dp (input.size(), std::vector<unsigned long long>(input[0].size(), (unsigned long long)-1));
    std::queue<std::tuple<int, int, unsigned long long>> q {};
    q.push(std::make_tuple(0, 0, 0));
    while (!q.empty()) {
        auto current = q.front(); q.pop();
        auto current_row = std::get<0>(current);
        auto current_col = std::get<1>(current);
        auto current_length = std::get<2>(current);
        if (current_row < 0 || current_row >= input.size() || current_col < 0 || current_col >= input[0].size()) {
            continue;
        }
        current_length += input[current_row][current_col];
        if (current_length >= dp[current_row][current_col]) {
            continue;
        }
        dp[current_row][current_col] = current_length;
        q.push(std::make_tuple(current_row + 1, current_col, current_length));
        q.push(std::make_tuple(current_row - 1, current_col, current_length));
        q.push(std::make_tuple(current_row, current_col + 1, current_length));
        q.push(std::make_tuple(current_row, current_col - 1, current_length));
    }
    return dp[input.size() - 1][input[0].size() - 1] - input[0][0];
}

unsigned long long solve2(std::vector<std::vector<uint8_t>> input) {
    std::vector<std::vector<uint8_t>> larger_input (input.size() * 5, std::vector<uint8_t>(input[0].size() * 5));
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            for (int ii = 0; ii < input.size(); ++ii) {
                for (int jj = 0; jj < input.size(); ++jj) {
                    auto next = input[ii][jj] + i + j;
                    if (next >= 10) {
                        next = next % 10 + 1;
                        if (next == 10) {
                            next == 1;
                        }
                    }
                    larger_input[i * input.size() + ii][j * input[0].size() + jj] = next;
                }
            }
        }
    }
    input = larger_input;

    // dijkstra
    std::priority_queue<std::tuple<uint64_t, int, int>, std::vector<std::tuple<uint64_t, int, int>>, std::greater<std::tuple<uint64_t, int, int>>> q {};
    std::vector<std::vector<uint64_t>> dists (input.size(), std::vector<uint64_t>(input[0].size(), (uint64_t)-1));
    std::vector<std::vector<bool>> seen (input.size(), std::vector<bool>(input[0].size()));
    q.push(std::make_tuple(0, 0, 0));
    while (!q.empty()) {
        auto current = q.top(); q.pop();
        auto dist = std::get<0>(current);
        auto row = std::get<1>(current);
        auto col = std::get<2>(current);
        
        if (seen[row][col]) {
            continue;
        }
        seen[row][col] = true;

        if (row + 1 < input.size() && dists[row + 1][col] > dist + input[row + 1][col]) {
            q.emplace(dist + input[row + 1][col], row + 1, col); 
            dists[row + 1][col] = dist + input[row + 1][col];
        }
        if (row != 0 && dists[row - 1][col] > dist + input[row - 1][col]) {
            q.emplace(dist + input[row - 1][col], row - 1, col); 
            dists[row - 1][col] = dist + input[row - 1][col];
        }
        if (col + 1 < input[0].size() && dists[row][col + 1] > dist + input[row][col + 1]) {
            q.emplace(dist + input[row][col + 1], row, col + 1); 
            dists[row][col + 1] = dist + input[row][col + 1];
        }
        if (col != 0 && dists[row][col - 1] > dist + input[row][col - 1]) {
            q.emplace(dist + input[row][col - 1], row, col - 1); 
            dists[row][col - 1] = dist + input[row][col - 1];
        }
    }
    return dists[input.size() - 1][input[0].size() - 1];
}

int main() {
    auto input = get_input("input.txt");
    std::cout << solve1(input) << '\n';
    std::cout << solve2(input) << '\n';

    return 0;
}