/*
 * Copyright (c) 2024, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

/* standard imports */

/* custom imports */
#include "include/mechanics.h"
#include "include/curses_tui.h"
#include "include/snake.h"

Mechanics::Mechanics() {
    init();
}

Mechanics::~Mechanics() {}

void Mechanics::init() {
    init_screen();
    m_initialized = true;
}

bool Mechanics::is_initialized() {
    return m_initialized;
}

void Mechanics::init_screen() {
    init_playarea();
    init_menu();
    init_snake();
    init_reward();
}

void Mechanics::init_playarea() {
    /* game screen */
    m_game_win.create_window(LINES, COLS*2/3, 0, 0);
    m_game_win.cbreak();
    m_game_win.cursor_mode(CURSOR_INVISIBLE);
    m_game_win.set_echo(false);
    m_game_win.draw_box();
}

void Mechanics::init_menu() {
    /* menu screen */
    size_t menu_width = 38;
    size_t menu_height = 13;
    size_t menu_start_row = LINES/2 - menu_height/2;
    size_t menu_start_col = COLS*2/3 + COLS/6 - menu_width/2;
    m_menu_win.create_window(menu_height, menu_width,
        menu_start_row, menu_start_col);
    m_menu_win.cursor_mode(CURSOR_INVISIBLE);
    /* wait for input for atleast 1 second */
    m_menu_win.set_delay(1000);
    m_menu_win.set_border();
    render_menu();
}

void Mechanics::init_snake() {
    ::wattron(m_game_win.get_window(), A_BOLD | A_STANDOUT);
    size_t y_pos = m_game_win.get_height()/2;
    size_t x_pos = m_game_win.get_width()/2-1;
    SnakeUnit unit(y_pos, x_pos, '#');
    snake.add_unit(unit);
    m_game_win.print(y_pos, x_pos, unit.get_symbol());
    wstandend(m_game_win.get_window());
}

void Mechanics::init_reward() {
    int height = m_game_win.get_height();
    int width = m_game_win.get_width();
    reward.init(height, width);
    SnakeUnit head = snake.get_head();
    reward.mark_blocked(head.get_row(), head.get_col());
    reward.random_position();
    int y_pos = reward.get_row();
    int x_pos = reward.get_col();
    m_game_win.print(y_pos, x_pos, reward.get_symbol());
}

void Mechanics::render_menu() {
    ::wattron(m_menu_win.get_window(), A_BOLD | A_STANDOUT);
    std::string menu_str = "Snake Game Menu";
    int win_mid = m_menu_win.get_width()/2;
    int col_pos = win_mid - menu_str.size()/2;
    m_menu_win.print(2, col_pos, menu_str);
    wstandend(m_menu_win.get_window());

    std::string entry = "Up:    w, k, up-arrow";
    col_pos = win_mid - 12;
    m_menu_win.print(4, col_pos, entry);

    entry = "Down:  s, j, down-arrow";
    m_menu_win.print(5, col_pos, entry);

    entry = "Left:  a, h, left-arrow";
    m_menu_win.print(6, col_pos, entry);

    entry = "Right: d, l, right-arrow";
    m_menu_win.print(7, col_pos, entry);

    entry = "Pause: p";
    m_menu_win.print(9, col_pos, entry);

    entry = "Quit:  q";
    m_menu_win.print(10, col_pos, entry);
}

int Mechanics::read() const {
    return m_menu_win.read();
}

void Mechanics::update() {
    SnakeUnit next = snake.get_next_head();
    SnakeUnit tail = snake.remove_tail();
    snake.add_unit(next);
    ::wattron(m_game_win.get_window(), A_BOLD | A_STANDOUT);
    m_game_win.print(next.get_row(), next.get_col(), next.get_symbol());
    wstandend(m_game_win.get_window());
    m_game_win.print(tail.get_row(), tail.get_col(), ' ');
}

void Mechanics::set_direction(Direction direction) {
    snake.set_direction(direction);
}
