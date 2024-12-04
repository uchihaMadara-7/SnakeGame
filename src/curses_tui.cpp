/*
 * Copyright (c) 2024, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

/* custom imports */
#include "include/curses_tui.h"

/* Initialize the static member */
bool CursesWindow::m_initialized = false;

CursesWindow::CursesWindow() {
    init();
    // m_win = stdscr;
    // m_win = newwin(rows, cols, starty, startx);
    // box(m_win, '|', '-');
    // wrefresh(m_win);
    // get_dimension(m_height, m_width);
}

CursesWindow::~CursesWindow() {
    ::delwin(m_win);
    /* closes the window and restore the terminal settings */
    ::endwin();
}

void CursesWindow::init() {
    if (!m_initialized) {
        /* Initialises the screen */
        ::initscr();
        /* Allow handling of control characters like arrow keys, enter, ... */
        ::keypad(stdscr, TRUE);
        ::start_color();
        ::use_default_colors();
        /* Default color pair 0 cannot be modified */
        // init_pair(0, -1, -1);
        // ::wattron(m_win, COLOR_PAIR(0));
        ::refresh();
        m_initialized = true;
    }
}

void CursesWindow::create_window(int rows, int cols, int row_start, int col_start) {
    m_win = newwin(rows, cols, row_start, col_start);
    ::keypad(m_win, TRUE);
    // box(m_win, '|', '-');
    // wrefresh(m_win);
    get_dimension(m_height, m_width);
}

void CursesWindow::cbreak() {
    /*
     * One-character-a-time. To disable the buffering of typed characters by
     * the TTY driver and get a character-at-a-time input without cbreak(),
     * getch() will read input after newline (till then input will be queued)
     */
    // ::raw();
    ::cbreak();
}

void CursesWindow::set_echo(bool flag) {
    /* noecho(): To suppress the automatic echoing of typed characters */
    flag? ::echo() : ::noecho();
}

void CursesWindow::cursor_mode(int mode) {
    ::curs_set(mode);
}

void CursesWindow::nodelay() {
    ::nodelay(m_win, TRUE);
}

/* delay is in milli-seconds */
void CursesWindow::set_delay(int delay) {
    ::wtimeout(m_win, delay);
}

void CursesWindow::clear() {
    ::wclear(m_win);
}

void CursesWindow::move(int y, int x) {
    ::wmove(m_win, y, x);
}

void CursesWindow::movex(int x) {
    ::wmove(m_win, ::getcury(m_win), x);
}

void CursesWindow::movey(int y) {
    ::wmove(m_win, y, ::getcurx(m_win));
}

void CursesWindow::get_dimension(int &height, int &width) {
    height = ::getmaxy(m_win);
    width = ::getmaxx(m_win);
}

int CursesWindow::get_height() {
    return m_height;
}

int CursesWindow::get_width() {
    return m_width;
}

int CursesWindow::get_cursor_x() {
    return ::getcurx(m_win);
}

int CursesWindow::get_cursor_y() {
    return ::getcury(m_win);
}

WINDOW* CursesWindow::get_window() {
    return m_win;
}

void CursesWindow::draw_box() {
    ::box(m_win, ACS_VLINE, ACS_HLINE);
}

/* void set_border(char ls = BORDER_LS, char rs = BORDER_RS,
    char ts = BORDER_TS, char bs = BORDER_BS, char tl = BORDER_TL,
    char tr = BORDER_TR, char bl = BORDER_BL, char br = BORDER_BR); */
void CursesWindow::set_border(char ls, char rs, char ts, char bs,
    char tl, char tr, char bl, char br) {
    ::wborder(m_win, ls, rs, ts, bs, tl, tr, bl, br);
    ::wrefresh(m_win);
}

void CursesWindow::refresh() {
    ::wrefresh(m_win);
}

int CursesWindow::read() const {
    return ::wgetch(m_win);
}

void CursesWindow::print(int c) {
    ::waddch(m_win, c);
    ::wrefresh(m_win);
}

void CursesWindow::print(int y, int x, int c) {
    mvwaddch(m_win, y, x, c);
    ::wrefresh(m_win);
}

void CursesWindow::print(std::string msg) {
    ::waddstr(m_win, msg.c_str());
    ::wrefresh(m_win);
}

void CursesWindow::print(int y, int x, std::string msg) {
    mvwaddstr(m_win, y, x, msg.c_str());
    ::wrefresh(m_win);
}
