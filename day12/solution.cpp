#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <functional>

std::unordered_map<std::string, std::vector<std::string>> get_input(const std::string& file_name) {
    std::ifstream fs {file_name};
    if (!fs.is_open()) {
        throw std::runtime_error {"Can't open the file"};
    }

    std::unordered_map<std::string, std::vector<std::string>> input {};
    std::string node1_str, node2_str;
    while (std::getline(fs, node1_str, '-')) {
        std::getline(fs, node2_str);
        input[node1_str].push_back(node2_str);
        input[node2_str].push_back(node1_str);
    }

    return input;
}

template <typename Input>
int solve1(const Input& input) {   
    std::function<int(const std::string&, std::unordered_set<std::string>&)> dfs;
    dfs = [&dfs, &input](const std::string& node, std::unordered_set<std::string>& seen) -> int {
        if (node == "end") {
            return 1;
        }
        if (std::islower(node[0])) {
            seen.insert(node);
        }
        int count = 0;
        for (const auto& child : input.at(node)) {
            if (seen.find(child) != seen.end()) {
                continue;
            }
            count += dfs(child, seen);
        }
        seen.erase(node);
        return count;
    };

    std::unordered_set<std::string> seen {};
    return dfs("start", seen);
};

template <typename Input>
int solve2(const Input& input) {   
    std::function<int(const std::string&, std::unordered_set<std::string>&, bool, bool, std::unordered_map<std::string, int>*)> dfs;
    dfs = [&dfs, &input](const std::string& node, std::unordered_set<std::string>& seen, bool done_second, bool you, std::unordered_map<std::string, int>* dp) -> int {
        if (node == "end") {
            return 1;
        }
        if (std::islower(node[0])) {
            seen.insert(node);
        }
        int count = 0;
        for (const auto& child : input.at(node)) {
            if (child == "start") {
                continue;
            }
            if (seen.find(child) != seen.end()) {
                if (!done_second) {
                    count += dfs(child, seen, true, true, dp);
                }
                continue;
            }
            count += dfs(child, seen, done_second, false, dp);
        }
        if (!done_second) {
            seen.erase(node);
        }
        return count;
    };

    std::unordered_set<std::string> seen {};
    std::unordered_map<std::string, int> dp[2];
    return dfs("start", seen, false, false, dp);
};

int main() {
    auto input = get_input("input.txt");
    std::cout << solve1(input) << '\n';
    std::cout << solve2(input) << '\n';

    return 0;
}