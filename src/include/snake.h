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
#include <set>
#include <string>

enum Direction {
    UP = -1,
    DOWN = 1,
    LEFT = -2,
    RIGHT = 2
};

class SnakeUnit {
 public:
    typedef unsigned int chtype;

    SnakeUnit(int row, int col) : m_row(row), m_col(col) {
        m_symbol = 'x';
    }

    SnakeUnit(int row, int col, int symbol) :
        m_row(row), m_col(col), m_symbol(symbol) {}

    /* getters */
    int get_row();
    int get_col();
    chtype get_symbol();

    /* setters */
    void set_row(int row);
    void set_col(int col);
    void set_symbol(int symbol);

 private:
    int m_row, m_col;
    chtype m_symbol;
};

class Snake {
 public:
    /* getters */
    SnakeUnit get_tail();
    SnakeUnit get_head();
    SnakeUnit get_next_head();
    bool is_valid_position(SnakeUnit position);

    /* setters */
    void add_unit(SnakeUnit unit);
    SnakeUnit remove_tail();
    void set_direction(Direction direction);

 private:
    std::queue<SnakeUnit> m_snake_body;
    Direction m_direction = RIGHT;
    std::set<std::string> m_snake_position;

    std::string get_key(SnakeUnit position);
};

#endif  // SRC_INCLUDE_SNAKE_H_
