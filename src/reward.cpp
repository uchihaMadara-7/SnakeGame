/*
 * Copyright (c) 2024, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

/* custom imports */
#include "include/reward.h"
#include <random>
#include <string>
#include <utility>

int Reward::get_row() {
    return m_row;
}

int Reward::get_col() {
    return m_col;
}

Reward::chtype Reward::get_symbol() {
    return m_symbol;
}

void Reward::set_position(int row, int col) {
    m_row = row;
    m_col = col;
}

void Reward::init(int height, int width) {
    for (int y=1; y < height-1; ++y) {
        for (int x=1; x < width-1; ++x) {
            mark_unblocked(y, x);
        }
    }
}

void Reward::random_position() {
    int last_index = m_free_cells.size()-1;
    std::random_device rd;  // Non-deterministic random number generator
    std::mt19937 generator(rd());
    int index = std::uniform_int_distribution<int>(0, last_index)(generator);
    int y_pos = m_free_cells[index].first;
    int x_pos = m_free_cells[index].second;
    set_position(y_pos, x_pos);
}

std::string Reward::get_key(int row, int col) {
    std::string key = std::to_string(row) + "/" + std::to_string(col);
    return key;
}

void Reward::mark_blocked(int row, int col) {
    std::string key = get_key(row, col);
    if (!m_position_to_index.count(key)) return;
    int index = m_position_to_index[key];
    int last_index = m_free_cells.size()-1;
    swap(m_free_cells[last_index], m_free_cells[index]);
    m_free_cells.pop_back();
    m_position_to_index.erase(key);
    std::pair<int, int> swapped_pos = m_free_cells[index];
    std::string swapped_key = get_key(swapped_pos.first, swapped_pos.second);
    m_position_to_index[swapped_key] = index;
}

void Reward::mark_unblocked(int row, int col) {
    std::string key = get_key(row, col);
    m_free_cells.push_back(std::make_pair(row, col));
    m_position_to_index[key] = m_free_cells.size()-1;
}
