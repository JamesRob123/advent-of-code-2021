#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <set>

std::pair<std::set<std::pair<int, int>>, std::vector<std::pair<bool, int>>> get_input(const std::string& file_name) {
    std::ifstream fs {file_name};
    if (!fs.is_open()) {
        throw std::runtime_error {"Can't open the file"};
    }

    std::string current_line;
    std::set<std::pair<int, int>> points {};
    while (std::getline(fs, current_line)) {
        if (current_line.empty()) {
            break;
        }
        std::istringstream iss {current_line};
        int n1, n2;
        iss >> n1;
        iss.get();
        iss >> n2;
        points.insert(std::make_pair(n1, n2));
    }

    std::vector<std::pair<bool, int>> foldes {};
    while (std::getline(fs, current_line)) {
        bool is_x_axis = (current_line[11] == 'x');
        int fold_val = std::stoi(current_line.c_str() + 13);
        foldes.emplace_back(is_x_axis, fold_val);
    }

    return std::make_pair(std::move(points), std::move(foldes));
}

template <typename Input>
int solve1(Input input) {
    auto fold = [&input](int axis, bool is_x_axis) -> void {
        if (axis < 0) {
            return;
        }
        auto current_point = input.first.begin();
        while (current_point != input.first.end()) {
            std::pair<int, int> p = [is_x_axis, &current_point, axis] {
                if (is_x_axis) {
                    return std::make_pair(axis - (current_point->first - axis), current_point->second);
                } else {
                    return std::make_pair(current_point->first, axis - (current_point->second - axis));
                }
            }();
            input.first.insert(p);
            ++current_point;
        }
        std::erase_if(input.first, [axis, is_x_axis](const auto& p) {
            return (is_x_axis ? p.first : p.second) >= axis;
        });
    };

    auto is_x_axis {input.second[0].first};
    int fold_val {input.second[0].second};
    fold(fold_val, is_x_axis);

    return input.first.size();
}

template <typename Input>
void solve2(Input input) {
    auto print = [&input]() {
        auto highest_x = input.first.end();
        --highest_x;
        int highest_y = (*std::max_element(input.first.begin(), input.first.end(), [](const auto& p1, const auto& p2) {
            return p1.second < p2.second;
        })).second;
        for (int i = 0; i <= highest_y; ++i) {
            for (int j = 0; j <= highest_x->first; ++j) {
                if (input.first.find(std::make_pair(j, i)) == input.first.end()) {
                    std::cout << '.';
                } else {
                    std::cout << "█";
                }
            }
            std::cout << '\n';
        }
    };

    auto fold = [&input](int axis, bool is_x_axis) -> void {
        if (axis < 0) {
            return;
        }
        auto current_point = input.first.begin();
        while (current_point != input.first.end()) {
            std::pair<int, int> p = [is_x_axis, &current_point, axis] {
                if (is_x_axis) {
                    return std::make_pair(axis - (current_point->first - axis), current_point->second);
                } else {
                    return std::make_pair(current_point->first, axis - (current_point->second - axis));
                }
            }();
            input.first.insert(p);
            ++current_point;
        }
        std::erase_if(input.first, [axis, is_x_axis](const auto& p) {
            return (is_x_axis ? p.first : p.second) >= axis;
        });
    };

   for (int i = 0; i < input.second.size(); ++i) {
        auto is_x_axis {input.second[i].first};
        int fold_val {input.second[i].second};
        fold(fold_val, is_x_axis);
    }

    print();
}

int main() {
    auto input = get_input("input.txt");
    std::cout << solve1(input) << '\n';
    solve2(input);

    return 0;
}