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
    return row;
}

int SnakeUnit::get_col() {
    return col;
}

SnakeUnit::chtype SnakeUnit::get_symbol() {
    return symbol;
}

void SnakeUnit::set_row(int row) {
    this->row = row;
}

void SnakeUnit::set_col(int col) {
    this->col = col;
}

void SnakeUnit::set_symbol(int symbol) {
    this->symbol = symbol;
}

/* snake class definitions */

SnakeUnit Snake::get_tail() {
    return body.front();
}

SnakeUnit Snake::get_head() {
    return body.back();
}

SnakeUnit Snake::get_next_head() {
    SnakeUnit head = get_head();
    int row = head.get_row();
    int col = head.get_col();

    switch (direction) {
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

void Snake::add_unit(SnakeUnit unit) {
    body.push(unit);
}

SnakeUnit Snake::remove_tail() {
    SnakeUnit tail = get_tail();
    body.pop();
    return tail;
}

void Snake::set_direction(Direction direction) {
    if (0 == (this->direction + direction)) return;
    this->direction = direction;
}
