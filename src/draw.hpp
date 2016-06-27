/*
 *    ________  ________  .____      __________.__
 *    \______ \ \______ \ |    |     \______   \  |  __ __   ____
 *     |    |  \ |    |  \|    |      |    |  _/  | |  |  \_/ __ \
 *     |    `   \|    `   \    |___   |    |   \  |_|  |  /\  ___/
 *    /_______  /_______  /_______ \  |______  /____/____/  \___  >
 *            \/        \/        \/         \/                 \/
 */


#ifndef GOPA2_DRAW_HPP_B16B00B135A93A55555555M01189998819991197253
#define GOPA2_DRAW_HPP_B16B00B135A93A55555555M01189998819991197253

#include <ncurses.h>

/**
 * Initializes ncurses, colors and all ncurses related stuff.
 * @return Returns true on succes, otherwise returns false.
 * */
bool DrawInit();

/**
 * End of the ncurses mode, return to the classical terminal mode.
 * */
void DrawExit();

#endif //GOPA2_DRAW_HPP_B16B00B135A93A55555555M01189998819991197253
