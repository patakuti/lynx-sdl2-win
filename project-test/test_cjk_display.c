/* Test CJK character display with PDCurses SDL2 */
#include <curses.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    setlocale(LC_ALL, "");

    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    clear();

    /* Test 1: ASCII characters */
    mvaddstr(0, 0, "ASCII: ABCDEFGHIJ");

    /* Test 2: Japanese characters using addstr (UTF-8) */
    mvaddstr(2, 0, "Japanese: ");
    addstr("日本語テスト");

    /* Test 3: Mixed ASCII and Japanese */
    mvaddstr(4, 0, "Mixed: ABC日本語DEF");

    /* Test 4: Check cursor position after Japanese */
    mvaddstr(6, 0, "After JP: ");
    addstr("あ");
    int y, x;
    getyx(stdscr, y, x);
    mvprintw(7, 0, "Cursor after 'あ': y=%d, x=%d (expected x=12)", y, x);

    /* Test 5: Multiple Japanese characters */
    mvaddstr(9, 0, "Multiple: ");
    addstr("漢字");
    getyx(stdscr, y, x);
    mvprintw(10, 0, "Cursor after '漢字': y=%d, x=%d (expected x=14)", y, x);

    mvaddstr(12, 0, "Press any key to exit...");

    refresh();
    getch();
    endwin();

    return 0;
}
