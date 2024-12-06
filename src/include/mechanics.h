/*
 * Copyright (c) 2024, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

/* custom imports */
#include "curses_tui.h"
#include "reward.h"
#include "snake.h"

class Mechanics {
 public:
    Mechanics();
    ~Mechanics();

    /* Delete the copy constructor and copy operator */
    Mechanics(const Mechanics&) = delete;
    Mechanics& operator=(const Mechanics&) = delete;

    /* Make Editor class as Singleton */
    static Mechanics& getInstance() {
        static Mechanics instance;
        return instance;
    }

    void init();
    bool is_initialized();
    void init_screen();
    void init_playarea();
    void init_menu();
    void init_snake();
    void init_reward();
    void render_menu();
    int read() const;
    bool update();
    void set_direction(Direction direction);
    bool is_out_of_bounds(int row, int col);

 private:
    CursesWindow m_game_win;
    CursesWindow m_menu_win;
    Snake snake;
    Reward reward;
    bool m_initialized = false;
};
