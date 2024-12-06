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

#define DISPLAY_ARTIFACT(artifact)                            \
    {                                                         \
      int y_pos = artifact.get_row();                         \
      int x_pos = artifact.get_col();                         \
      m_game_win.print(y_pos, x_pos, artifact.get_symbol());  \
    }

#define DISPLAY_EMPTY(artifact)                               \
    {                                                         \
      int y_pos = artifact.get_row();                         \
      int x_pos = artifact.get_col();                         \
      m_game_win.print(y_pos, x_pos, ' ');                    \
    }

#define MAX(first, second) ((first) > (second))?(first):(second)

#define MENU_WIDTH       38
#define MENU_HEIGHT      13
#define SCORE_WIDTH      COLS
#define SCORE_HEIGHT     1
#define MIN_GAME_WIDTH   40
#define MIN_GAME_HEIGHT  (20 + SCORE_HEIGHT)
#define GAME_WIDTH       (COLS - MENU_WIDTH - MOE)
#define GAME_HEIGHT      (LINES - SCORE_HEIGHT)
#define MOE              4  /* margin of error */
#define REDUCE_DELAY     25

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

int Mechanics::read() const {
    return m_menu_win.read();
}

bool Mechanics::update() {
    SnakeUnit next = m_snake.get_next_head();
    /* check if snake next position is out of bounds */
    if (is_out_of_bounds(next.get_row(), next.get_col())) return false;

    /* check if snake doesn't overlap with its body */
    if (!m_snake.is_valid_position(next)) return false;

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
        return true;
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
    return true;
}

void Mechanics::set_direction(Direction direction) {
    m_snake.set_direction(direction);
}

bool Mechanics::is_out_of_bounds(int row, int col) {
    if (row <=0 || row >= m_game_win.get_height()-1) return true;
    if (col <=0 || col >= m_game_win.get_width()-1) return true;
    return false;
}
