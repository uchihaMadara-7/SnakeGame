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

    Mechanics *game = new Mechanics;
    if (!game->is_initialized()) {
        printf("Game initialisation failed! ");
        printf("(Try increasing the window size) ");
        return 1;
    }

    int ch;
    bool quit = false;

    while (!quit) {
        Mechanics::GameState state;

        /* decision lambda function */
        auto decision = [&]() {
            switch (state) {
                case Mechanics::GameState::QUIT:
                    delete game;
                    quit = true;
                    break;
                case Mechanics::GameState::RETRY:
                    delete game;
                    game = new Mechanics;
                    break;
                case Mechanics::GameState::CONTINUE:
                    break;
            }
        };

        ch = game->read();
        switch (ch) {
            case 'q':
                state = Mechanics::GameState::QUIT;
                decision();
                break;
            case 'p':
                state = game->game_pause();
                decision();
                break;
            case KEY_DOWN:
                game->set_direction(DOWN);
                break;
            case KEY_UP:
                game->set_direction(UP);
                break;
            case KEY_LEFT:
                game->set_direction(LEFT);
                break;
            case KEY_RIGHT:
                game->set_direction(RIGHT);
                break;
        }

        if (quit) break;
        // auto start = std::chrono::high_resolution_clock::now();
        state = game->update();
        decision();
        // auto finish = std::chrono::high_resolution_clock::now();
        // auto duration = std::chrono::duration_cast<std::chrono::microseconds>(finish-start);
        // DEBUG_TRACE("update took: %lld us", duration.count());
    }

    return 0;
}
