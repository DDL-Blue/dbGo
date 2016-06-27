/*
 *    ________  ________  .____      __________.__
 *    \______ \ \______ \ |    |     \______   \  |  __ __   ____
 *     |    |  \ |    |  \|    |      |    |  _/  | |  |  \_/ __ \
 *     |    `   \|    `   \    |___   |    |   \  |_|  |  /\  ___/
 *    /_______  /_______  /_______ \  |______  /____/____/  \___  >
 *            \/        \/        \/         \/                 \/
 */

#include <iostream>
#include <ncurses.h>
#include <cmath>
#include "constants.hpp"
#include "structures.hpp"

#include "game.hpp"
#include "draw.hpp"
#include "menu.hpp"
#include "msgbox.hpp"

using namespace std;

/**
 * @mainpage Go
 * This is a school project - Japanese board game called Go.
 * Has three modes. PvP on one computer, PvP trough the network and PvE.
 * Libs used: ncurses, pthread, lrt
 * Other libs were not allowed - next (not school) version will feature sounds and some chiptune music! :)
 *
 * DDL Blue 2016
 * */

void Bg();
/*----------------------------------------------------------------------*/
int main() {

    DrawInit();

    int preLines = 0, preCols = 0;

try {
    timespec timer, tmpTimer;
    timer.tv_nsec = tmpTimer.tv_nsec = timer.tv_sec = tmpTimer.tv_sec = 0;


    CMenu *menu = new CMenu();


    while (menu->Tick()) { //game loop

        clock_gettime(CLOCK_REALTIME, &tmpTimer); //redraw game
        if (((uint32_t) abs(tmpTimer.tv_nsec - timer.tv_nsec) > NS_PER_FRAME) ||
            (tmpTimer.tv_sec > timer.tv_sec &&
             (uint32_t) (tmpTimer.tv_nsec + 1000000000 - timer.tv_nsec) > NS_PER_FRAME)
                ) {

            timer.tv_nsec = tmpTimer.tv_nsec;
            timer.tv_sec = tmpTimer.tv_sec;


            if (LINES!=preLines || COLS!=preCols) {
                if ((unsigned) LINES <= GAME_GFX_H || (unsigned) COLS <= GAME_GFX_W) {
                    clear();
                    move(0, 0);
                    printw("Terminal too small, \nenlarge the window");
                    refresh();
                    continue;
                } else {
                    preLines = LINES;
                    preCols = COLS;
                    clear();
                    menu->DrawRebuild(((unsigned) COLS - GAME_GFX_W) / 2, ((unsigned) LINES - GAME_GFX_H) / 2);

                    Bg();
                    refresh();
                }
            }
            menu->Draw();
        }

    }

    delete menu;

    DrawExit();
}
    catch(const char* text){
        DrawExit();
        cerr << "EXCEPTION: " << text << endl;
    }


    return 0;
}
/*----------------------------------------------------------------------*/
/**
*Draws colo point as an ASCII art
*@param [in] x X-coordinate of the point
*@param [in] y Y-coordinate of the point
*@param [in] hue Hue of the color.
*@param [in] val Value of the color
*/
void DrawPoint(uint32_t x, uint32_t y, uint8_t hue, uint8_t val){
    int32_t h, v;

    v = ((val/32)+( (( 2*x+7*y*x+21*y )%8)>(val%8) ))%8;
    h = ((hue/42)+( ((3*x*x+7*y*x+17*y)%6)>(hue%6) ))%6;

    attron (COLOR_PAIR(12+h));
    mvaddch (y,x,SHADING_CHARS[v]);
    attroff (COLOR_PAIR(12+h));

}
/*----------------------------------------------------------------------*/
/**Draws background around the game*/
void Bg() {
    for (uint32_t i = 0; i < (unsigned) LINES; ++i) {
        for (uint32_t j = 0; j < (unsigned) COLS; ++j) {
            if (j < ((unsigned) COLS - GAME_GFX_W) / 2 || j > ((unsigned) COLS - GAME_GFX_W) / 2 + GAME_GFX_W ||
                i < ((unsigned) LINES - GAME_GFX_H) / 2 || i > ((unsigned) LINES - GAME_GFX_H) / 2 + GAME_GFX_H) {
                DrawPoint (j, i,
                           i*j,
                           400*sin( (float)i/LINES*5 )*cos( (float)j/COLS*3 ));
            }
        }
    }
}