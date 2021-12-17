#include <iostream>
#include <fstream>
#include <cctype>
#include <array>

std::pair<std::pair<int, int>, std::pair<int, int>> get_input(const std::string& file_name) {
    std::ifstream fs {file_name};
    if (!fs.is_open()) {
        throw std::runtime_error {"Can't open the file"};
    }

    std::string current_line {};
    std::getline(fs, current_line);
    std::array<int, 4> poses {};

    int count = 0;
    for (int i = 0; i < current_line.size(); ++i) {
        if(current_line[i] == '-' || std::isdigit(current_line[i])) {
            std::size_t pos {};
            poses[count++] = std::stoi(current_line.c_str() + i, &pos);
            i += pos - 1;
        }
    }

    return std::make_pair(std::make_pair(poses[0], poses[2]), std::make_pair(poses[1], poses[3]));
}

template <typename Input>
int solve1(const Input& input) {
    int box_x1 = input.first.first;
    int box_y1 = input.first.second;
    int box_x2 = input.second.first;
    int box_y2 = input.second.second;
    auto is_in_box = [box_x1, box_y1, box_x2, box_y2](const std::pair<int, int>& p) {
        int x = p.first, y = p.second;
        if (x <= std::max(box_x2, box_x1) && x >= std::min(box_x1, box_x2) && y <= std::max(box_y2, box_y1) && y >= std::min(box_y1, box_y2)) {
            return true;
        }
        return false;
    };
    auto can_reach_box = [box_x1, box_y1, box_x2, box_y2](const std::pair<int, int>& p) {
        int x = p.first, y = p.second;
        if (x > std::max(box_x2, box_x1) || y < std::min(box_y1, box_y2)) {
            return false;
        }
        return true;
    };

    int max_y = -1;
    for (int start_x_velocity = 1; start_x_velocity < 25; ++start_x_velocity) {
        for (int start_y_velocity = 1; start_y_velocity < 1000; ++start_y_velocity) {
            int current_x_velocity = start_x_velocity;
            int current_y_velocity = start_y_velocity;
            int current_x_pos = 0, current_y_pos = 0;
            bool is_possible = false;
            int current_max_y = -1;
            while (can_reach_box(std::make_pair(current_x_pos, current_y_pos))) {
                if (is_in_box({current_x_pos, current_y_pos})) {
                    is_possible = true;
                    break;
                }
                current_max_y = std::max(current_max_y, current_y_pos);
                // update pos
                current_y_pos += current_y_velocity;
                --current_y_velocity;
                current_x_pos += current_x_velocity;
                current_x_velocity += (current_x_velocity > 0 ? -1 : (current_x_velocity < 0 ? 1 : 0));
            }
            if (is_possible) {
                max_y = std::max(max_y, current_max_y);
            }
        }
    }

    return max_y;
}

template <typename Input>
int solve2(const Input& input) {
    int box_x1 = input.first.first;
    int box_y1 = input.first.second;
    int box_x2 = input.second.first;
    int box_y2 = input.second.second;
    auto is_in_box = [box_x1, box_y1, box_x2, box_y2](const std::pair<int, int>& p) {
        int x = p.first, y = p.second;
        if (x <= std::max(box_x2, box_x1) && x >= std::min(box_x1, box_x2) && y <= std::max(box_y2, box_y1) && y >= std::min(box_y1, box_y2)) {
            return true;
        }
        return false;
    };
    auto can_reach_box = [box_x1, box_y1, box_x2, box_y2](const std::pair<int, int>& p) {
        int x = p.first, y = p.second;
        if (x > std::max(box_x2, box_x1) || y < std::min(box_y1, box_y2)) {
            return false;
        }
        return true;
    };

    int count = 0;
    for (int start_x_velocity = 1; start_x_velocity < 1000; ++start_x_velocity) {
        for (int start_y_velocity = -1000; start_y_velocity < 1000; ++start_y_velocity) {
            int current_x_velocity = start_x_velocity;
            int current_y_velocity = start_y_velocity;
            int current_x_pos = 0, current_y_pos = 0;
            bool is_possible = false;
            while (can_reach_box(std::make_pair(current_x_pos, current_y_pos))) {
                if (is_in_box({current_x_pos, current_y_pos})) {
                    is_possible = true;
                    break;
                }
                // update pos
                current_y_pos += current_y_velocity;
                --current_y_velocity;
                current_x_pos += current_x_velocity;
                current_x_velocity += (current_x_velocity > 0 ? -1 : (current_x_velocity < 0 ? 1 : 0));
            }
            if (is_possible) {
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