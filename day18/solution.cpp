#include <iostream>
#include <vector>
#include <fstream>
#include <variant>
#include <memory>
#include <sstream>
#include <cmath>

std::vector<std::string> get_input(const std::string& file_name) {
    std::ifstream fs {file_name};
    if (!fs.is_open()) {
        throw std::runtime_error {"Can't open the file"};
    }

    std::vector<std::string> input {};
    std::string current {};
    while (std::getline(fs, current)) {
        input.push_back(current);
    }

    return input;
}

struct Node {
    std::variant<std::unique_ptr<Node>, int> l_child = 0;
    std::variant<std::unique_ptr<Node>, int> r_child = 0;
    Node* parent = nullptr;
};

std::unique_ptr<Node> build_tree(std::istringstream& str_stream, Node* parent);

std::variant<std::unique_ptr<Node>, int> read_single_unit_from_pair(std::istringstream& str_stream, Node* parent) {
    std::variant<std::unique_ptr<Node>, int> unit {};
    char next_char = str_stream.peek();
    if (next_char == '[') {
        str_stream.get();
        unit = build_tree(str_stream, parent);
    } else {
        int val;
        str_stream >> val;
        unit = val;
    }
    return unit;
}

std::unique_ptr<Node> build_tree(std::istringstream& str_stream, Node* parent) {
    std::unique_ptr<Node> root = std::make_unique<Node>();
    root->parent = parent;
    root->l_child = read_single_unit_from_pair(str_stream, root.get());
    str_stream.get();   // extract comma
    root->r_child = read_single_unit_from_pair(str_stream, root.get());
    str_stream.get();   // extract c_bracket
    return root;
}

// TODO: make this a part of the node class
int* get_successor(Node* current) {
    Node* parent = current->parent;
    try {
        while (parent != nullptr && std::get<std::unique_ptr<Node>>(parent->r_child).get() == current) {
            parent = parent->parent;
            current = current->parent;
        }
    } catch (...) {
        return &(std::get<int>(parent->r_child));
    }
    if (parent == nullptr) {
        return nullptr;
    }
    parent = std::get<std::unique_ptr<Node>>(parent->r_child).get();
    try {
        while (true) {
            parent = std::get<std::unique_ptr<Node>>(parent->l_child).get();
        }
    } catch (...) {
        return &(std::get<int>(parent->l_child));
    }
}

// TODO: make this a part of the node class
int* get_predecessor(Node* current) {
    Node* parent = current->parent;
    try {
        while (parent != nullptr && std::get<std::unique_ptr<Node>>(parent->l_child).get() == current) {
            parent = parent->parent;
            current = current->parent;
        }
    } catch (...) {
        return &(std::get<int>(parent->l_child));
    }
    if (parent == nullptr) {
        return nullptr;
    }
    parent = std::get<std::unique_ptr<Node>>(parent->l_child).get();
    try {
        while (true) {
            parent = std::get<std::unique_ptr<Node>>(parent->r_child).get();
        }
    } catch (...) {
        return &(std::get<int>(parent->r_child));
    }
}

// does one action of explode
bool explode(Node* snail_fish_number, int depth) {
    if (depth == 4) {
        int* successor = get_successor(snail_fish_number);
        if (successor != nullptr) {
            *successor += std::get<int>(snail_fish_number->r_child);
        }
        int* predecessor = get_predecessor(snail_fish_number);
        if (predecessor != nullptr) {
            *predecessor += std::get<int>(snail_fish_number->l_child);
        }
        try {
            if (std::get<std::unique_ptr<Node>>(snail_fish_number->parent->l_child).get() == snail_fish_number) {
                snail_fish_number->parent->l_child = 0;
                return true;
            }
        } catch (...) {}
        snail_fish_number->parent->r_child = 0;
        return true;
    }
    bool exploded = false;
    try {
        exploded |= explode(std::get<0>(snail_fish_number->l_child).get(), depth + 1);
    } catch (...) {}
    if (exploded) {
        return true;
    }
    try {
        exploded |= explode(std::get<0>(snail_fish_number->r_child).get(), depth + 1);
    } catch (...) {}
    return exploded;
}

// does one action of split
bool split(Node* snail_fish_number) {
    bool did_split = false;
    try {
        if (std::get<int>(snail_fish_number->l_child) >= 10) {
            int val = std::get<int>(snail_fish_number->l_child);
            Node temp = {(int)std::floor(val / 2.0), (int)std::ceil(val / 2.0), snail_fish_number};
            snail_fish_number->l_child = std::make_unique<Node>(std::move(temp));
            return true;
        }
    } catch(std::bad_variant_access&) {
        did_split |= split(std::get<std::unique_ptr<Node>>(snail_fish_number->l_child).get());
    }
    if (did_split) {
        return true;
    }
    try {
        if (std::get<int>(snail_fish_number->r_child) >= 10) {
            int val = std::get<int>(snail_fish_number->r_child);
            Node temp = {(int)std::floor(val / 2.0), (int)std::ceil(val / 2.0), snail_fish_number};
            snail_fish_number->r_child = std::make_unique<Node>(std::move(temp));
            return true;
        }
    } catch(std::bad_variant_access&) {
        did_split |= split(std::get<std::unique_ptr<Node>>(snail_fish_number->r_child).get());
    }

    return did_split;
}

void reduce(Node* snail_fish_number) {
    while (explode(snail_fish_number, 0) || split(snail_fish_number)) {};
}

std::unique_ptr<Node> add(std::unique_ptr<Node> snail_fish_number1, std::unique_ptr<Node> snail_fish_number2) {
    Node temp = {std::move(snail_fish_number1), std::move(snail_fish_number2), nullptr};
    std::unique_ptr<Node> root = std::make_unique<Node>(std::move(temp));
    std::get<std::unique_ptr<Node>>(root->l_child)->parent = root.get();
    std::get<std::unique_ptr<Node>>(root->r_child)->parent = root.get();
    return root;
}

int calc_magnitude(Node* snail_fish_number) {
    int sum = 0;
    try {
        if (std::get<std::unique_ptr<Node>>(snail_fish_number->l_child).get()) {
            sum += calc_magnitude(std::get<std::unique_ptr<Node>>(snail_fish_number->l_child).get()) * 3;
        }
    } catch (...) {
        sum += std::get<int>(snail_fish_number->l_child) * 3;
    }
    try {
        if (std::get<std::unique_ptr<Node>>(snail_fish_number->r_child).get()) {
            sum += calc_magnitude(std::get<std::unique_ptr<Node>>(snail_fish_number->r_child).get()) * 2;
        }
    } catch (...) {
        sum += std::get<int>(snail_fish_number->r_child) * 2;
    }
    return sum;
}

void print(Node* root) {
    std::cout << "["; std::flush(std::cout);
    try {
        print(std::get<std::unique_ptr<Node>>(root->l_child).get());
    } catch (...) {
        std::cout << std::get<int>(root->l_child);
    }
    std::cout << ","; std::flush(std::cout);;
    try {
        print(std::get<std::unique_ptr<Node>>(root->r_child).get());
    } catch (...) {
        std::cout << std::get<int>(root->r_child);
    }
    std::cout << "]"; std::flush(std::cout);;
}

int solve1(const std::vector<std::string>& input) {
    std::unique_ptr<Node> last_snail_fish_number = nullptr;
    for (const auto& snail_fish_number : input) {
        std::istringstream iss {snail_fish_number};   
        iss.get();
        std::unique_ptr<Node> snail_fish_number_root = build_tree(iss, nullptr);

        if (last_snail_fish_number != nullptr) {
            reduce(last_snail_fish_number.get());
            snail_fish_number_root = add(std::move(last_snail_fish_number), std::move(snail_fish_number_root));   
        }
        reduce(snail_fish_number_root.get());
        last_snail_fish_number = std::move(snail_fish_number_root);
    }
    return calc_magnitude(last_snail_fish_number.get());
}

int solve2(const std::vector<std::string>& input) {
    auto calc_mag_of_2 = [](const std::string& str1, const std::string& str2) {
        std::istringstream iss_first {str1}; iss_first.get();
        std::istringstream iss_second {str2}; iss_second.get();
        std::unique_ptr<Node> snail_fish_number_root_first = build_tree(iss_first, nullptr);
        std::unique_ptr<Node> snail_fish_number_root_second = build_tree(iss_second, nullptr);

        reduce(snail_fish_number_root_first.get());
        reduce(snail_fish_number_root_second.get());

        snail_fish_number_root_first = add(std::move(snail_fish_number_root_first), std::move(snail_fish_number_root_second));
        reduce(snail_fish_number_root_first.get());
        return calc_magnitude(snail_fish_number_root_first.get());
    };

    int max = 0;
    for (int i = 0; i < input.size(); ++i) {
        for (int j = i + 1; j < input.size(); ++j) {
            max = std::max(max, calc_mag_of_2(input[i], input[j]));
            max = std::max(max, calc_mag_of_2(input[j], input[i]));
        }
    }
    return max;
}

int main() {
    auto input = get_input("input.txt");
    std::cout << solve1(input) << '\n';
    std::cout << solve2(input) << '\n';

    return 0;
}