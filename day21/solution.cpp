#include <iostream>
#include <fstream>
#include <cstring>

std::pair<int, int> get_input(const std::string& file_name) {
    std::ifstream fs {file_name};
    if (!fs.is_open()) {
        throw std::runtime_error {"Can't open the file"};
    }

    std::string current_line;
    int a;
    std::getline(fs, current_line);
    a = current_line.back() - '0';
    int b;
    std::getline(fs, current_line);
    b = current_line.back() - '0';
    return std::make_pair(a, b);
}

int solve1(const std::pair<int, int>& input) {
    int player1 = input.first;
    int player2 = input.second;
    int current_roll {1};
    int score1 = 0;
    int score2 = 0;
    int count_rolls = 0;

    auto move_player = [](int& player) {
        ++player;
        if (player > 10) {
            player = 1;
        }
    };

    while (true) {
        for (int i = 0; i < current_roll; ++i) {
            move_player(player1);
        }
        ++current_roll; if (current_roll > 100) current_roll = 1;
        for (int i = 0; i < current_roll; ++i) {
            move_player(player1);
        }
        ++current_roll; if (current_roll > 100) current_roll = 1;
        for (int i = 0; i < current_roll; ++i) {
            move_player(player1);
        }
        ++current_roll; if (current_roll > 100) current_roll = 1;

        count_rolls += 3;
        score1 += player1;
        if (score1 >= 1000) {
            return score2 * count_rolls;
        }

        for (int i = 0; i < current_roll; ++i) {
            move_player(player2);
        }
        ++current_roll; if (current_roll > 100) current_roll = 1;
        for (int i = 0; i < current_roll; ++i) {
            move_player(player2);
        }
        ++current_roll; if (current_roll > 100) current_roll = 1;
        for (int i = 0; i < current_roll; ++i) {
            move_player(player2);
        }
        ++current_roll; if (current_roll > 100) current_roll = 1;

        count_rolls += 3;
        score2 += player2;
        if (score2 >= 1000) {
            return score1 * count_rolls;
        }
    }
}

std::pair<long long, long long> operator+(std::pair<long long, long long> p1, std::pair<long long, long long> p2) {
    return std::make_pair(p1.first + p2.first, p1.second + p2.second);
}

void move_player(long long& player, long long steps) {
    for (long long i = 0; i < steps; ++i) {
        ++player;
        if (player > 10) {
            player = 1;
        }
    }
}

std::pair<long long, long long> dp[11][11][22][22][4][10][2] {};
std::pair<long long, long long> calc(long long player1, long long player2, long long score1, long long score2, bool player1_turn, long long rolls, long long current_steps) {
    if (rolls >= 3) {
        if (player1_turn) {
            move_player(player1, current_steps);
            score1 += player1;
        } else {
            move_player(player2, current_steps);
            score2 += player2;
        }
        player1_turn = !player1_turn;
        rolls = 0;
        current_steps = 0;
    }

    if (score1 >= 21) {
        return std::make_pair(1, 0);
    }
    if (score2 >= 21) {
        return std::make_pair(0, 1);
    }

    if (dp[player1][player2][score1][score2][rolls][current_steps][player1_turn] != std::make_pair(-1LL, -1LL)) {
        return dp[player1][player2][score1][score2][rolls][current_steps][player1_turn];
    }

    auto first = calc(player1, player2, score1, score2, player1_turn, rolls + 1, current_steps + 1);
    auto second = calc(player1, player2, score1, score2, player1_turn, rolls + 1, current_steps + 2);
    auto third = calc(player1, player2, score1, score2, player1_turn, rolls + 1, current_steps + 3);

    auto result = first + second + third;
    dp[player1][player2][score1][score2][rolls][current_steps][player1_turn] = result;
    return result;
}

long long solve2(const std::pair<long long, long long>& input) {
    for (long long i = 0; i < 11; ++i) {
        for (long long j = 0; j < 11; ++j) {
            for (long long k = 0; k < 22; ++k) {
                for (long long l = 0; l < 22; ++l) {
                    for (int m = 0; m < 4; ++m) {
                        for (int n = 0; n < 10; ++n) {
                            for (int o = 0; o < 2; ++o) {
                                dp[i][j][k][l][m][n][o] = std::make_pair(-1LL, -1LL);
                            }
                        }
                    }
                }
            }
        }
    }
    long long player1 = input.first, player2 = input.second;
    auto p = calc(player1, player2, 0, 0, true, 0, 0);
    return std::max(p.first, p.second);
}

int main() {
    auto input = get_input("input.txt");
    std::cout << solve1(input) << '\n';
    std::cout << solve2(input) << '\n';

    return 0;
}