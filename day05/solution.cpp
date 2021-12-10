#include <iostream>
#include <vector>
#include <string>
#include <fstream>

std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> get_input(const std::string& file_name) {
    std::ifstream fs {file_name};
    if (!fs.is_open()) {
        throw std::runtime_error {"Can't open file"};
    }

    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> input {};
    while (true) {
        int x1, y1, x2, y2 {};
        if (!(fs >> x1)) {
            break;
        }
        fs.get();
        fs >> y1;

        for (int i = 0; i < 4; ++i) {
            fs.get();
        }

        fs >> x2;
        fs.get();
        fs >> y2;

        input.push_back(std::make_pair(std::make_pair(x1, y1), std::make_pair(x2, y2)));
    }

    return input;
}

template <typename Input>
int solve1(Input input) {
    int matrix[1000][1000] {};

    for (auto& [pos1, pos2] : input) {
        // x is the same
        if (pos1.first == pos2.first) {
            if (pos1.second > pos2.second) {  // y of pos1 will always be less or equal to y of pos2
                std::swap(pos1, pos2);
            }
            for (int i = pos1.second; i <= pos2.second; ++i) {
                ++matrix[i][pos1.first];
            }
        } else if (pos1.second == pos2.second) {    // y is the same
            if (pos1.first > pos2.first) {  // x of pos1 will always be less or equal to x of pos2
                std::swap(pos1, pos2);
            }
            for (int i = pos1.first; i <= pos2.first; ++i) {
                ++matrix[pos1.second][i];
            }
        }
    }

    int count = 0;
    for (int i = 0; i < 1000; ++i) {
        for (int j = 0; j < 1000; ++j) {
            if (matrix[i][j] > 1) {
                ++count;
            }
        }
    }

    return count;
}

template <typename Input>
int solve2(Input input) {
    int matrix[1000][1000] {};

    for (auto& [pos1, pos2] : input) {
        // x is the same
       if (pos1.first == pos2.first) {
            if (pos1.second > pos2.second) {  // y of pos1 will always be less or equal to y of pos2
                std::swap(pos1, pos2);
            }
            for (int i = pos1.second; i <= pos2.second; ++i) {
                ++matrix[i][pos1.first];
            }
        } else if (pos1.second == pos2.second) {    // y is the same
            if (pos1.first > pos2.first) {  // x of pos1 will always be less or equal to x of pos2
                std::swap(pos1, pos2);
            }
            for (int i = pos1.first; i <= pos2.first; ++i) {
                ++matrix[pos1.second][i];
            }
        } else {
            if (pos1.second > pos2.second) {  // y of pos1 will always be less or equal to y of pos2
                std::swap(pos1, pos2);
            }
            if (pos1.first < pos2.first) {
                while (pos1.second <= pos2.second) {
                    ++matrix[pos1.second][pos1.first];
                    ++pos1.first;
                    ++pos1.second;
                }
            } else {
                while (pos1.second <= pos2.second) {
                    ++matrix[pos1.second][pos1.first];
                    --pos1.first;
                    ++pos1.second;
                }
            }
        }
    }

    int count = 0;
    for (int i = 0; i < 1000; ++i) {
        for (int j = 0; j < 1000; ++j) {
            if (matrix[i][j] > 1) {
                ++count;
            }
        }
    }

    return count;
}

int main() {
    auto input = get_input("input.txt");
    std::cout << solve1(input) << '\n';
    std::cout << solve2(input) << '\n';

    return 0;
}