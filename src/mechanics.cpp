/*
 * Copyright (c) 2024, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

/* standard imports */
#include <cstring>

/* custom imports */
#include "include/mechanics.h"
#include "include/curses_tui.h"
#include "include/snake.h"

Mechanics::Mechanics() {
    init();
}

Mechanics::~Mechanics() {}

void Mechanics::init() {
    if (MENU_WIDTH + MIN_GAME_WIDTH + MOE > COLS) return;
    if (MIN_GAME_HEIGHT > LINES) return;
    /* minimum screen requirement passed */
    init_screen();
    m_initialized = true;
}

bool Mechanics::is_initialized() {
    return m_initialized;
}

void Mechanics::init_screen() {
    init_menu();
    init_score();
    init_playarea();
    init_snake();
    init_reward();
}

void Mechanics::init_menu() {
    /* menu screen */
    size_t start_row = LINES/2 - MENU_HEIGHT/2;
    size_t start_col = GAME_WIDTH + MOE/2;
    m_menu_win.create_window(MENU_HEIGHT, MENU_WIDTH, start_row, start_col);
    m_menu_win.cursor_mode(CURSOR_INVISIBLE);
    /* wait for input for atleast 1 second */
    m_menu_win.set_delay(m_delay);
    m_menu_win.set_border();
    render_menu();
}

void Mechanics::init_score() {
    /* score screen */
    m_score_win.create_window(SCORE_HEIGHT, SCORE_WIDTH, 0, 0);
    m_score_win.cursor_mode(CURSOR_INVISIBLE);
    m_score_win.print(" SCORE: 0");
}

void Mechanics::init_playarea() {
    /* game screen */
    m_game_win.create_window(GAME_HEIGHT, GAME_WIDTH, SCORE_HEIGHT, 0);
    m_game_win.cbreak();
    m_game_win.cursor_mode(CURSOR_INVISIBLE);
    m_game_win.set_echo(false);
    m_game_win.draw_box();
}

void Mechanics::init_snake() {
    ::wattron(m_game_win.get_window(), A_BOLD | A_STANDOUT);
    size_t y_pos = m_game_win.get_height()/2;
    size_t x_pos = m_game_win.get_width()/2-1;
    SnakeUnit unit(y_pos, x_pos, '#');
    m_snake.add_unit(unit);
    m_game_win.print(y_pos, x_pos, unit.get_symbol());
    wstandend(m_game_win.get_window());
}

void Mechanics::init_reward() {
    int height = m_game_win.get_height();
    int width = m_game_win.get_width();
    m_reward.init(height, width);
    SnakeUnit head = m_snake.get_head();
    m_reward.mark_blocked(head.get_row(), head.get_col());
    m_reward.random_position();
    DISPLAY_ARTIFACT(m_reward);
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

Mechanics::GameState Mechanics::game_over() {
    /* gameover banner window */
    CursesWindow over_win;
    size_t start_row = GAME_HEIGHT/2 - OVER_HEIGHT/2;
    size_t start_col = GAME_WIDTH/2 - OVER_WIDTH/2;
    over_win.create_window(OVER_HEIGHT, OVER_WIDTH, start_row, start_col);
    over_win.cbreak();
    over_win.cursor_mode(CURSOR_INVISIBLE);
    over_win.set_echo(false);
    init_pair(1, COLOR_RED, -1);
    ::wattron(over_win.get_window(), A_BOLD | COLOR_PAIR(1));
    over_win.draw_box();
    start_col = OVER_WIDTH/2 - strlen(GAME_OVER)/2;
    over_win.print(1, start_col, GAME_OVER);
    wstandend(over_win.get_window());

    std::string score = "SCORE: " + std::to_string(m_score);
    start_col = OVER_WIDTH/2 - score.size()/2;
    over_win.print(2, start_col, score);

    /* options to retry and quit */
    auto select_option = [&](GameState choice) {
        start_col = OVER_WIDTH/2 - (strlen(GAME_RETRY)+strlen(GAME_QUIT)+2)/2;
        if (choice == GameState::RETRY) {
            ::wattron(over_win.get_window(), A_BOLD | A_STANDOUT);
            over_win.print(4, start_col, GAME_RETRY);
            wstandend(over_win.get_window());
            over_win.print("  ");
            over_win.print(GAME_QUIT);
            return;
        }
        over_win.print(4, start_col, GAME_RETRY);
        over_win.print("  ");
        ::wattron(over_win.get_window(), A_BOLD | A_STANDOUT);
        over_win.print(GAME_QUIT);
        wstandend(over_win.get_window());
    };

    chtype key;
    GameState choice = GameState::RETRY;
    select_option(choice);
    bool chosen = false;
    while (!chosen) {
        key = over_win.read();
        switch (key) {
            case KEY_LEFT:
                choice = GameState::RETRY;
                select_option(choice);
                break;
            case KEY_RIGHT:
                choice = GameState::QUIT;
                select_option(choice);
                break;
            case KEY_ENTER:
                chosen = true;
                break;
        }
    }

    return choice;
}

Mechanics::GameState Mechanics::game_pause() {
    /* game-pause banner window */
    CursesWindow pause_win;
    size_t start_row = GAME_HEIGHT/2 - OVER_HEIGHT/2;
    size_t start_col = GAME_WIDTH/2 - OVER_WIDTH/2;
    pause_win.create_window(OVER_HEIGHT, OVER_WIDTH, start_row, start_col);
    pause_win.cbreak();
    pause_win.cursor_mode(CURSOR_INVISIBLE);
    pause_win.set_echo(false);
    ::wattron(pause_win.get_window(), A_BOLD);
    pause_win.draw_box();
    start_col = OVER_WIDTH/2 - strlen(GAME_PAUSE)/2;
    pause_win.print(1, start_col, GAME_PAUSE);
    wstandend(pause_win.get_window());

    std::string score = "SCORE: " + std::to_string(m_score);
    start_col = OVER_WIDTH/2 - score.size()/2;
    pause_win.print(2, start_col, score);

    /* options to retry and quit */
    auto select_option = [&](GameState choice) {
        start_col = OVER_WIDTH/2 - (strlen(GAME_RESUME)+strlen(GAME_QUIT)+2)/2;
        if (choice == GameState::CONTINUE) {
            ::wattron(pause_win.get_window(), A_BOLD | A_STANDOUT);
            pause_win.print(4, start_col, GAME_RESUME);
            wstandend(pause_win.get_window());
            pause_win.print("  ");
            pause_win.print(GAME_QUIT);
            return;
        }
        pause_win.print(4, start_col, GAME_RESUME);
        pause_win.print("  ");
        ::wattron(pause_win.get_window(), A_BOLD | A_STANDOUT);
        pause_win.print(GAME_QUIT);
        wstandend(pause_win.get_window());
    };

    chtype key;
    GameState choice = GameState::CONTINUE;
    select_option(choice);
    bool chosen = false;
    while (!chosen) {
        key = pause_win.read();
        switch (key) {
            case KEY_LEFT:
                choice = GameState::CONTINUE;
                select_option(choice);
                break;
            case KEY_RIGHT:
                choice = GameState::QUIT;
                select_option(choice);
                break;
            case KEY_ENTER:
                chosen = true;
                break;
        }
    }

    pause_win.clear();
    pause_win.refresh();

    return choice;
}

int Mechanics::read() const {
    return m_menu_win.read();
}

Mechanics::GameState Mechanics::update() {
    SnakeUnit next = m_snake.get_next_head();
    /* check if snake next position is out of bounds */
    if (is_out_of_bounds(next.get_row(), next.get_col())) return game_over();

    /* check if snake doesn't overlap with its body */
    if (!m_snake.is_valid_position(next)) return game_over();

    m_snake.add_unit(next);
    m_reward.mark_blocked(next.get_row(), next.get_col());
    ::wattron(m_game_win.get_window(), A_BOLD | A_STANDOUT);
    DISPLAY_ARTIFACT(next);
    wstandend(m_game_win.get_window());

    /* check if next snake position is a reward position */
    if (next.get_row() != m_reward.get_row() ||
            next.get_col() != m_reward.get_col()) {
        /* new head is not a reward position */
        SnakeUnit tail = m_snake.remove_tail();
        m_reward.mark_unblocked(tail.get_row(), tail.get_col());
        DISPLAY_EMPTY(tail);
        return GameState::CONTINUE;
    }

    /* this codeflow comes when snake eats the reward */
    m_reward.random_position();
    DISPLAY_ARTIFACT(m_reward);

    /* increase the score by 10 */
    m_score += 10;
    std::string score = std::to_string(m_score);
    m_score_win.print(0, 8, score);

    /* decrease the delay by 25ms until it reaches 100ms */
    m_delay = MAX(m_delay-REDUCE_DELAY, 100);
    m_menu_win.set_delay(m_delay);
    return GameState::CONTINUE;
}

void Mechanics::set_direction(Direction direction) {
    m_snake.set_direction(direction);
}

bool Mechanics::is_out_of_bounds(int row, int col) {
    if (row <=0 || row >= m_game_win.get_height()-1) return true;
    if (col <=0 || col >= m_game_win.get_width()-1) return true;
    return false;
}
