#include <iostream>
#include <vector>
#include <fstream>
#include <stack>
#include <unordered_map>
#include <algorithm>

std::vector<std::string> get_input(const std::string& file_name) {
    std::ifstream fs {file_name};
    if (!fs.is_open()) {
        throw std::runtime_error {"Can't open the file"};
    }

    std::vector<std::string> input {};
    std::string line;
    while (fs >> line) {
        input.push_back(line);
    }

    return input;
}

int solve1(const std::vector<std::string>& input) {    
    const std::unordered_map<char, char> symbol_map {
        {'(', ')'},
        {'{', '}'},
        {'[', ']'},
        {'<', '>'}
    };
    const std::unordered_map<char, int> value_map {
        {')', 3},
        {']', 57},
        {'}', 1197},
        {'>', 25137}
    };

    int result = 0;
    for (const auto& line : input) {
        std::stack<char> expected {};
        for (char c : line) {
            if (c == ')' || c == '}' || c == '>' || c == ']') {
                char e = expected.top(); expected.pop();
                if (c != e) {
                    result += value_map.at(c);
                    break;
                }
            } else {
                expected.push(symbol_map.at(c));
            }
        }
    }

    return result;
}

unsigned long long solve2(const std::vector<std::string>& input) {
        const std::unordered_map<char, char> symbol_map {
        {'(', ')'},
        {'{', '}'},
        {'[', ']'},
        {'<', '>'}
    };
    const std::unordered_map<char, int> value_map {
        {')', 1},
        {']', 2},
        {'}', 3},
        {'>', 4}
    };

    std::vector<unsigned long long> scores {};
    for (const auto& line : input) {
        std::stack<char> expected {};
        bool is_corrupted = false;
        for (char c : line) {
            if (c == ')' || c == '}' || c == '>' || c == ']') {
                char e = expected.top(); expected.pop();
                if (c != e) {
                    is_corrupted = true;
                    break;
                }
            } else {
                expected.push(symbol_map.at(c));
            }
        }
        if (!is_corrupted) {
            unsigned long long score = 0;
            while (!expected.empty()) {
                unsigned long long current = expected.top(); expected.pop();
                score *= 5;
                score += value_map.at(current);
            }
            scores.push_back(score);
        }
    }

    std::sort(scores.begin(), scores.end());
    return scores[scores.size() / 2];
}

int main() {
    auto input = get_input("input.txt");
    std::cout << solve1(input) << '\n';
    std::cout << solve2(input) << '\n';

    return 0;
}