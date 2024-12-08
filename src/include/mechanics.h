/*
 * Copyright (c) 2024, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef SRC_INCLUDE_MECHANICS_H_
#define SRC_INCLUDE_MECHANICS_H_

/* custom imports */
#include "curses_tui.h"
#include "reward.h"
#include "snake.h"

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
#define OVER_HEIGHT      6
#define OVER_WIDTH       GAME_WIDTH/2
#define MOE              4  /* margin of error */
#define REDUCE_DELAY     25
#define GAME_OVER        "GAMEOVER!"
#define GAME_PAUSE       "PAUSED!"
#define GAME_RETRY       " RETRY "
#define GAME_RESUME      " RESUME "
#define GAME_QUIT        " QUIT "

class Mechanics {
 public:
    Mechanics();
    ~Mechanics();

    enum class GameState {
        CONTINUE,
        RETRY,
        QUIT
    };

    void init();
    bool is_initialized();
    void init_screen();
    void init_playarea();
    void init_menu();
    void init_score();
    void init_snake();
    void init_reward();
    void render_menu();
    GameState game_over();
    GameState game_pause();
    int read() const;
    GameState update();
    void set_direction(Direction direction);
    bool is_out_of_bounds(int row, int col);

 private:
    CursesWindow m_game_win, m_menu_win, m_score_win;
    Snake m_snake;
    Reward m_reward;
    bool m_initialized = false;
    unsigned int m_delay = 1000, m_score = 0;
};

#endif  // SRC_INCLUDE_MECHANICS_H_
