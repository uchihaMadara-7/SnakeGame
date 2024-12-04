/*
 * Copyright (c) 2024, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef SRC_INCLUDE_SNAKE_H_
#define SRC_INCLUDE_SNAKE_H_

/* standard imports */
#include <queue>

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class SnakeUnit {
 public:
    typedef unsigned int chtype;

    SnakeUnit(int row, int col) : row(row), col(col) {
        symbol = 'x';
    }

    SnakeUnit(int row, int col, int symbol) :
        row(row), col(col), symbol(symbol) {}

    /* getters */
    int get_row();
    int get_col();
    chtype get_symbol();

    /* setters */
    void set_row(int row);
    void set_col(int col);
    void set_symbol(int symbol);

 private:
    int row, col;
    chtype symbol;
};

class Snake {
 public:
    /* getters */
    SnakeUnit get_tail();
    SnakeUnit get_head();
    SnakeUnit get_next_head();

    /* setters */
    void add_unit(SnakeUnit unit);
    SnakeUnit remove_tail();

 private:
    std::queue<SnakeUnit> body;
    Direction direction = RIGHT;
};

#endif  // SRC_INCLUDE_SNAKE_H_
