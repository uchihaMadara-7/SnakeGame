/*
 * Copyright (c) 2024, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

/* custom imports */
#include "include/snake.h"

/* snake unit definitions */

int SnakeUnit::get_row() {
    return m_row;
}

int SnakeUnit::get_col() {
    return m_col;
}

SnakeUnit::chtype SnakeUnit::get_symbol() {
    return m_symbol;
}

void SnakeUnit::set_row(int row) {
    m_row = row;
}

void SnakeUnit::set_col(int col) {
    m_col = col;
}

void SnakeUnit::set_symbol(int symbol) {
    m_symbol = symbol;
}

/* snake class definitions */

SnakeUnit Snake::get_tail() {
    return m_snake_body.front();
}

SnakeUnit Snake::get_head() {
    return m_snake_body.back();
}

SnakeUnit Snake::get_next_head() {
    SnakeUnit head = get_head();
    int row = head.get_row();
    int col = head.get_col();

    switch (m_direction) {
        case UP:
            --row;
            break;
        case DOWN:
            ++row;
            break;
        case LEFT:
            --col;
            break;
        case RIGHT:
            ++col;
            break;
    }

    SnakeUnit next(row, col, '#');
    return next;
}

std::string Snake::get_key(SnakeUnit position) {
    int row = position.get_row();
    int col = position.get_col();
    std::string key = std::to_string(row) + "/" + std::to_string(col);
    return key;
}

bool Snake::is_valid_position(SnakeUnit position) {
    std::string key = get_key(position);
    std::string tail_key = get_key(get_tail());
    if (key == tail_key) return true;
    return !m_snake_position.count(key);
}

void Snake::add_unit(SnakeUnit unit) {
    m_snake_body.push(unit);
    std::string key = get_key(unit);
    m_snake_position.insert(key);
}

SnakeUnit Snake::remove_tail() {
    SnakeUnit tail = get_tail();
    m_snake_body.pop();
    std::string key = get_key(tail);
    m_snake_position.erase(key);
    return tail;
}

void Snake::set_direction(Direction direction) {
    if (0 == (m_direction + direction)) return;
    m_direction = direction;
}
