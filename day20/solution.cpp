#include <iostream>
#include <vector>
#include <bitset>
#include <fstream>

char outside{'.'};

std::pair<std::string, std::vector<std::string>> get_input(const std::string& file_name) {
    std::ifstream fs {file_name};
    if (!fs.is_open()) {
        throw std::runtime_error {"Can't open the file"};
    }
    std::string image_enhancement_algorithm;
    fs >> image_enhancement_algorithm;
    
    std::string current {};
    std::getline(fs, current);
    std::getline(fs, current);
    std::vector<std::string> input {};
    while (std::getline(fs, current)) {
        input.push_back(current);
    }

    return std::make_pair(image_enhancement_algorithm, input);
}

char at(int i, int j, const auto& input_image) {
  if (i < 0 || i >= input_image.size())
    return outside;
  if (j < 0 || j >= input_image[0].size())
    return outside;
  return input_image[i][j];
}

char update_cell(int row, int col, const auto& input_image, const auto& image_enhancement_algorithm) {
    std::bitset<9> current {};
    for (int di = -1; di <= 1; ++di) {
        for (int dj = -1; dj <= 1; ++dj) {
            int bit_index = 8 - ((di + 1) * 3 + (dj + 1));
            char val = at(di + row, dj + col, input_image);
            if (val == '#') {
                current.set(bit_index, true);
            } else {
                current.set(bit_index, false);
            }
        }
    }

    return image_enhancement_algorithm[current.to_ulong()];
};

template <typename Input>
int solve1(const Input& input) {
    const auto& image_enhancement_algorithm = input.first;
    auto input_image = input.second;

    for (int i = 0; i < 50; ++i) {
        char new_outside = update_cell(-10, -10, input_image, image_enhancement_algorithm);
        std::vector<std::string> output_image {};
        for (int row = -1; row <= (int)input_image.size(); ++row) {
            std::string row_str {};
            for (int col = -1; col <= (int)input_image[0].size(); ++col) {
                row_str += update_cell(row, col, input_image, image_enhancement_algorithm);
            }
            output_image.push_back(row_str);
        }
        input_image = std::move(output_image);
        outside = new_outside;
    }

    int count = 0;
    for (int i = 0; i < input_image.size(); ++i) {
        for (int j = 0; j < input_image[0].size(); ++j) {
            if (input_image[i][j] != '.') {
                ++count;
            }
        }
    }
    return count;
}

int main() {
    auto input = get_input("input.txt");
    std::cout << solve1(input) << '\n';

    return 0;
}