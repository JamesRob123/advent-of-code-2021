#include <iostream>
#include <vector>
#include <fstream>

std::vector<std::pair<std::string, int>> get_input(const std::string& file_name) {
    std::fstream fs {file_name};
    if (!fs.is_open()) {
        throw std::runtime_error {"Can't open file"};
    }

    std::vector<std::pair<std::string, int>> input {};
    std::string command {};
    int val {};
    while (fs >> command, fs >> val) {
        input.push_back(std::make_pair(command, val));
    }
    return input;
};

int solve1(const std::vector<std::pair<std::string, int>>& commands) {
    long long hor {};
    long long depth {};
    for (const auto& command : commands) {
        if (command.first == "forward") {
            hor += command.second;
        } else if (command.first == "up") {
            depth -= command.second;
        } else if (command.first == "down") {
            depth += command.second;
        }
    }
    return hor * depth;
}

int solve2(const std::vector<std::pair<std::string, int>>& commands) {
    long long hor {};
    long long depth {};
    long long aim {};
    for (const auto& command : commands) {
        if (command.first == "forward") {
            hor += command.second;
            depth += command.second * aim;
        } else if (command.first == "up") {
            aim -= command.second;
        } else if (command.first == "down") {
            aim += command.second;
        }
    }
    return hor * depth;
}

int main() {
    auto commands = get_input("input.txt");
    std::cout << solve1(commands) << '\n';
    std::cout << solve2(commands) << '\n';

    return 0;
}