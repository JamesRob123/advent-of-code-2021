#include <iostream>
#include <fstream>
#include <bitset>
#include <unordered_map>
#include <ranges>
#include <functional>
#include <algorithm>

std::string get_input(const std::string& file_name) {
    std::ifstream fs {file_name};
    if (!fs.is_open()) {
        throw std::runtime_error {"Can't open the file"};
    }

    std::string input {};
    fs >> input;
    return input;
}

template <std::size_t N>
std::bitset<N> str_iterator_to_bitset(std::string::const_iterator& str) {
    std::bitset<N> bs {};
    for (auto i : std::views::iota(1ULL, N + 1)) {
        bs.set(N - i, *str - '0');
        ++str;
    }
    return bs;
}

uint64_t calc_sum_of_versions(std::string::const_iterator& message) {
    std::bitset<3> version = str_iterator_to_bitset<3>(message);
    std::bitset<3> id = str_iterator_to_bitset<3>(message);

    uint64_t sum_of_versions {version.to_ullong()};
    if (id.to_ulong() == 4UL) {     // Handle id 4
        std::bitset<5> group {};
        do {
            group = str_iterator_to_bitset<5>(message);
        } while (group[4]);
    } else {
        std::bitset<1> length_type_ID = str_iterator_to_bitset<1>(message);
        if (length_type_ID.to_ulong() == 0UL) {             // Handle packages with length type id 0
            long total_length = static_cast<long>(str_iterator_to_bitset<15>(message).to_ulong());
            auto message_begining = message;
            while (message - message_begining < total_length) {
                sum_of_versions += calc_sum_of_versions(message);
            }
        } else {                                            // Handle packages with length type id 1
            uint32_t num_of_sub_packages = str_iterator_to_bitset<11>(message).to_ulong();
            while (num_of_sub_packages--) {
                sum_of_versions += calc_sum_of_versions(message);
            }
        }
    }
    return sum_of_versions;
}

const std::unordered_map<char, std::string> hex_to_binary {
    {'0', "0000"},
    {'1', "0001"},
    {'2', "0010"},
    {'3', "0011"},
    {'4', "0100"},
    {'5', "0101"},
    {'6', "0110"},
    {'7', "0111"},
    {'8', "1000"},
    {'9', "1001"},
    {'A', "1010"},
    {'B', "1011"},
    {'C', "1100"},
    {'D', "1101"},
    {'E', "1110"},
    {'F', "1111"},
};

uint64_t solve1(const std::string& input) {
    // Convert the input to binary
    std::string input_as_binary {};
    for (char c : input) {
        input_as_binary += hex_to_binary.at(c);
    }

    auto begining = input_as_binary.cbegin();
    return calc_sum_of_versions(begining);
}

const std::unordered_map<uint8_t, std::function<uint64_t(uint64_t, uint64_t)>> operator_map {
    {0, [](uint64_t n1, uint64_t n2) -> uint64_t { return std::plus<uint64_t>{}(n1, n2); }},
    {1, [](uint64_t n1, uint64_t n2) -> uint64_t { return std::multiplies<uint64_t>{}(n1, n2); }},
    {2, [](uint64_t n1, uint64_t n2) -> uint64_t { return std::min<uint64_t>(n1, n2); }},
    {3, [](uint64_t n1, uint64_t n2) -> uint64_t { return std::max<uint64_t>(n1, n2); }},
    {5, [](uint64_t n1, uint64_t n2) -> uint64_t { return std::greater<uint64_t>{}(n1, n2); }},
    {6, [](uint64_t n1, uint64_t n2) -> uint64_t { return std::less<uint64_t>{}(n1, n2); }},
    {7, [](uint64_t n1, uint64_t n2) -> uint64_t { return std::equal_to<uint64_t>{}(n1, n2); }}
};

uint64_t evaluate_message(std::string::const_iterator& message) {
    str_iterator_to_bitset<3>(message);
    std::bitset<3> id = str_iterator_to_bitset<3>(message);

    uint64_t package_val {-1ULL};
    if (id.to_ulong() == 4UL) { // Handle id 4
        std::bitset<64> package_val_bs {};
        std::bitset<5> group {};
        do {
            group = str_iterator_to_bitset<5>(message);
            package_val_bs <<= 4;
            package_val_bs |= std::bitset<package_val_bs.size()>(group.to_ullong() & 0xF);
        } while (group[4]);
        return package_val_bs.to_ullong();
    } else {
        std::bitset<1> length_type_ID = str_iterator_to_bitset<1>(message);
        if (length_type_ID.to_ulong() == 0UL) { // Handle packages with length type id 0
            long total_length = static_cast<long>(str_iterator_to_bitset<15>(message).to_ulong());
            auto message_begining = message;
            while (message - message_begining < total_length) {
                package_val = (package_val == -1ULL ? evaluate_message(message) : operator_map.at(id.to_ulong())(package_val, evaluate_message(message)));
            }
        } else { // Handle packages with length type id 1
            uint32_t num_of_sub_packages = str_iterator_to_bitset<11>(message).to_ulong();
            while (num_of_sub_packages--) {
                package_val = (package_val == -1ULL ? evaluate_message(message) : operator_map.at(id.to_ulong())(package_val, evaluate_message(message)));
            }
        }
    }
    return package_val;
}

uint64_t solve2(const std::string& input) {
    // Convert the input to binary
    std::string input_as_binary {};
    for (char c : input) {
        input_as_binary += hex_to_binary.at(c);
    }

    auto begining = input_as_binary.cbegin();
    return evaluate_message(begining);
}

int main() {
    auto input = get_input("input.txt");
    std::cout << solve1(input) << '\n';
    std::cout << solve2(input) << '\n';

    return 0;
}