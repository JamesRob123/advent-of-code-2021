#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <algorithm>

std::pair<std::string, std::vector<std::vector<char>>> get_input(const std::string& file_name) {
    std::ifstream fs {file_name};
    if (!fs.is_open()) {
        throw std::runtime_error {"Can't open the file"};
    }

    std::string polymer_template {};
    fs >> polymer_template;
    std::string current_line {};
    std::getline(fs, current_line);
    std::getline(fs, current_line);

    std::vector<std::vector<char>> pair_insertions (26, std::vector<char>(27, 26));
    while (std::getline(fs, current_line)) {
        char p1 = current_line[0] - 'A';
        char p2 = current_line[1] - 'A';
        char a = current_line[6] - 'A';
        pair_insertions[p1][p2] = a;
    }

    return std::make_pair(std::move(polymer_template), std::move(pair_insertions));
}

template <typename Input>
int solve1(Input input) {
    auto& polymer_template = input.first;
    auto& pair_insertions = input.second;
    for (int i = 0; i < 10; ++i) {
        std::string next_polymer_template {polymer_template[0]};
        for (uint64_t j = 1; j < polymer_template.size(); ++j) {
            char p1 = polymer_template[j - 1] - 'A';
            char p2 = polymer_template[j] - 'A';
            if (pair_insertions[p1][p2] != 26) {
                char a = pair_insertions[p1][p2] + 'A';
                next_polymer_template += a;
            } 
            next_polymer_template += p2 + 'A';
        }
        polymer_template = next_polymer_template;
    }

    uint64_t mem[26] {};
    for (uint64_t i = 0; i < polymer_template.size(); ++i) {
        ++mem[polymer_template[i] - 'A'];
    }
    uint64_t max = *std::max_element(std::begin(mem), std::end(mem));
    std::transform(std::cbegin(mem), std::cend(mem), std::begin(mem), [](uint64_t n) {
        return (n == 0 ? (uint64_t)-1 : n);
    });
    uint64_t min = *std::min_element(std::begin(mem), std::end(mem));
    return max - min;
}

template <typename Input>
uint64_t solve2(Input input) {
    auto& polymer_template = input.first;
    auto& pair_insertions = input.second;
    std::vector<std::vector<uint64_t>> histogram (26, std::vector<uint64_t>(26));
    for (int i = 1; i < polymer_template.size(); ++i) {
        int p1 = polymer_template[i - 1] - 'A';
        int p2 = polymer_template[i] - 'A';
        ++histogram[p1][p2];
    }
    for (int i = 0; i < 40; ++i) {
        std::vector<std::vector<uint64_t>> next_histogram (26, std::vector<uint64_t>(26));
        for (int p1 = 0; p1 < histogram.size(); ++p1) {
            for (int p2 = 0; p2 < histogram[p1].size(); ++p2) {
                uint64_t count = histogram[p1][p2];
                if (pair_insertions[p1][p2] != 26) {
                    int c = pair_insertions[p1][p2];
                    next_histogram[p1][c] += count;
                    next_histogram[c][p2] += count;
                } else {
                    next_histogram[p1][p2] += count;
                }
            }
        }
        histogram = next_histogram;
    }
    uint64_t count[26] {};
    for (int p1 = 0; p1 < histogram.size(); ++p1) {
        for (int p2 = 0; p2 < histogram[p1].size(); ++p2) {
            count[p1] += histogram[p1][p2];
        }
    }
    ++count[polymer_template.back() - 'A'];
    uint64_t max = *std::max_element(std::begin(count), std::end(count));
    std::transform(std::cbegin(count), std::cend(count), std::begin(count), [](uint64_t n) {
        return (n == 0 ? (uint64_t)-1 : n);
    });
    uint64_t min = *std::min_element(std::begin(count), std::end(count));
    return max - min;
}

int main() {
    auto input = get_input("input.txt");
    std::cout << solve1(input) << '\n';
    std::cout << solve2(input) << '\n';

    return 0;
}