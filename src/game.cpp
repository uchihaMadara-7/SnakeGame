/*
 * Copyright (c) 2024, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

/* standard imports */
#include <chrono>

/* custom imports */
#include "include/logger.h"
#include "include/mechanics.h"
#include "include/snake.h"

#define LOG_FILE "game.log"

int main() {
    /* Setting up logger */
    #ifdef DEBUG_FLAG
    Logger &logger = Logger::getInstance(LOG_FILE);
    if (!logger.is_initialized()) {
        /* Early return due to init failure */
        printf("Log initialisation failed!");
        return 1;
    }
    logger.set_level(LOG_DEBUG);
    #endif

    Mechanics &game = Mechanics::getInstance();
    if (!game.is_initialized()) {
        printf("Game initialisation failed!");
        return 1;
    }

    int ch;
    bool quit = false;

    while (!quit) {
        ch = game.read();
        // INFO_TRACE("read game character %d", ch);
        switch (ch) {
            case 'q':
                quit = true;
                break;
            case KEY_DOWN:
                game.set_direction(DOWN);
                break;
            case KEY_UP:
                game.set_direction(UP);
                break;
            case KEY_LEFT:
                game.set_direction(LEFT);
                break;
            case KEY_RIGHT:
                game.set_direction(RIGHT);
                break;
        }

        bool gameover = !game.update();
        quit |= gameover;
    }

    return 0;
}
