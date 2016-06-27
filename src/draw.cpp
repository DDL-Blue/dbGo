/*
 *    ________  ________  .____      __________.__
 *    \______ \ \______ \ |    |     \______   \  |  __ __   ____
 *     |    |  \ |    |  \|    |      |    |  _/  | |  |  \_/ __ \
 *     |    `   \|    `   \    |___   |    |   \  |_|  |  /\  ___/
 *    /_______  /_______  /_______ \  |______  /____/____/  \___  >
 *            \/        \/        \/         \/                 \/
 */

#include <iostream>
#include "draw.hpp"


bool DrawInit(){
    initscr();

    if(has_colors() == false) {
        endwin();
        std::cout << "Your terminal does not support color\n" << std::endl;
        return false;
    }
    start_color();

    //define colors

    init_pair (1, COLOR_BLACK, COLOR_WHITE); //board

    init_pair (2, COLOR_RED, COLOR_WHITE); //stone 1
    init_pair (3, COLOR_BLUE, COLOR_WHITE); //stone 2

    init_pair (4, COLOR_BLACK, COLOR_WHITE); //cursor

    //GUI
    init_pair (5, COLOR_WHITE, COLOR_BLACK); //border, GUI static text
    init_pair (6, COLOR_RED, COLOR_BLACK); //red score
    init_pair (7, COLOR_BLUE, COLOR_BLACK); //blue score

    init_pair (8, COLOR_WHITE, COLOR_BLACK); //button
    init_pair (9, COLOR_BLACK, COLOR_RED); //button selected

    init_pair(10, COLOR_WHITE, COLOR_BLUE); //MsgBox
    init_pair(11, COLOR_WHITE, COLOR_BLACK); //MsgBox text input

    init_pair(12, COLOR_RED, COLOR_BLACK); //BG
    init_pair(13, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(14, COLOR_BLUE, COLOR_BLACK);
    init_pair(15, COLOR_CYAN, COLOR_BLACK);
    init_pair(16, COLOR_GREEN, COLOR_BLACK);
    init_pair(17, COLOR_YELLOW, COLOR_BLACK);

    timeout (17);
    curs_set (0);
    noecho();
    keypad(stdscr, TRUE);

    return true;
}

/*----------------------------------------------------------------------*/
void DrawExit(){

    endwin();

}
