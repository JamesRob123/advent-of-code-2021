#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <numeric>

std::pair<std::vector<int>, std::vector<std::vector<std::vector<int>>>> get_input(const std::string& file_name) {
    std::ifstream fs {file_name};
    if (!fs.is_open()) {
        throw std::runtime_error {"Can't open file"};
    }
    std::pair<std::vector<int>, std::vector<std::vector<std::vector<int>>>> input {};
    input.first = {};

    std::string sequence {};
    std::getline(fs, sequence);
    std::istringstream is {sequence};
    int val = 0;
    while (is >> val) {
        input.first.push_back(val);
        char c;
        is.get(c);
    }

    int board = 0;
    int row = 0;
    input.second = {};
    input.second.push_back({});
    std::getline(fs, sequence);
    while (std::getline(fs, sequence)) {
        if (sequence.empty()) {
            ++board;
            input.second.push_back({});
            row = 0;
            continue;
        }
        std::istringstream is {sequence};
        input.second[board].push_back({});
        input.second[board][row] = {};
        while (is >> val) {
            input.second[board][row].push_back(val);
        }
        ++row;
    }

    return input;
}

template <typename Board>
bool is_bingo(const Board& board) {
    // Check each row
    for (int row = 0; row < board.size(); ++row) {
        if (std::accumulate(board[row].begin(), board[row].end(), 0) == -1 * board[row].size()) {
            return true;
        }
    }

    // Check each col
    for (int col = 0; col < board[0].size(); ++col) {
        int sum = 0;
        for (int row = 0; row < board.size(); ++row) {
            sum += board[row][col];
        }
        if (sum == -1 * board.size()) {
            return true;
        }
    }

    return false;
}

template <typename Input>
int solve1(Input& input) {
    const auto copy = input.second;
    for (int i = 0; i < input.first.size(); ++i) {
        int current_num = input.first[i];
        for (int j = 0; j < input.second.size(); ++j) {
            for (int row = 0; row < input.second[j].size(); ++row) {
                for (int col = 0; col < input.second[j][row].size(); ++col) {
                    if (input.second[j][row][col] == current_num) {
                        input.second[j][row][col] = -1;
                    }
                }
            }
            auto bingo = is_bingo(input.second[j]);
            if (bingo) {
                int sum = 0;
                for (int row = 0; row < input.second[j].size(); ++row) {
                    for (int col = 0; col < input.second[j][row].size(); ++col) {
                        sum += std::max(0, input.second[j][row][col]);
                    }
                }
                return sum * current_num;
            }
        }
    }

    return -1;
}

template <typename Input>
int solve2(Input& input) {
    const auto copy = input.second;
    int remaining = input.second.size();
    std::vector<bool> had_bingo (input.second.size());
    for (int i = 0; i < input.first.size(); ++i) {
        int current_num = input.first[i];
        for (int j = 0; j < input.second.size(); ++j) {
            for (int row = 0; row < input.second[j].size(); ++row) {
                for (int col = 0; col < input.second[j][row].size(); ++col) {
                    if (input.second[j][row][col] == current_num) {
                        input.second[j][row][col] = -1;
                    }
                }
            }
            auto bingo = is_bingo(input.second[j]);
            if (bingo && !had_bingo[j]) {
                had_bingo[j] = true;
                if (--remaining) {
                    continue;
                }
                int sum = 0;
                for (int row = 0; row < input.second[j].size(); ++row) {
                    for (int col = 0; col < input.second[j][row].size(); ++col) {
                        sum += std::max(0, input.second[j][row][col]);
                    }
                }
                return sum * current_num;
            }
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