/*
 * Copyright (c) 2024, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef SRC_INCLUDE_REWARD_H_
#define SRC_INCLUDE_REWARD_H_

/* standard imports */
#include <map>
#include <random>
#include <string>
#include <utility>
#include <vector>

class Reward {
 public:
    typedef unsigned int chtype;

    Reward() {
        symbol = 'x';
    }

    /* getters */
    int get_row();
    int get_col();
    chtype get_symbol();

    /* setters */
    void init(int height, int width);
    void set_position(int row, int col);
    void random_position();
    void mark_blocked(int row, int col);
    void mark_unblocked(int row, int col);

 private:
    int row, col;
    chtype symbol;
    std::map<std::string, int> position_to_index;
    std::vector<std::pair<int, int>> free_cells;

    std::string get_key(int row, int col);
};

#endif  // SRC_INCLUDE_REWARD_H_
