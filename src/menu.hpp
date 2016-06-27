/*
 *    ________  ________  .____      __________.__
 *    \______ \ \______ \ |    |     \______   \  |  __ __   ____
 *     |    |  \ |    |  \|    |      |    |  _/  | |  |  \_/ __ \
 *     |    `   \|    `   \    |___   |    |   \  |_|  |  /\  ___/
 *    /_______  /_______  /_______ \  |______  /____/____/  \___  >
 *            \/        \/        \/         \/                 \/
 */

#ifndef GOPA2_MENU_HPP_B16B00B135A93A55555555M01189998819991197253
#define GOPA2_MENU_HPP_B16B00B135A93A55555555M01189998819991197253

#include <string>
#include "constants.hpp"
#include "game.hpp"

/**
 * This class represents the game menu. It creates and runs games within it.
 * */
class CMenu {
public:
    CMenu();
    ~CMenu();

    /**One tick of the game. This should be called frequently*/
    bool Tick();

    /** Drawes the game. CALL DrawRebuild(...) AT LEAS ONCE BEFORE USING THIS METHOD!
     *  @return Returns true if game is running, false if game reached it's end.
     * */
    void Draw();

    /** Sets/changes the game drawing position. Should be called at start before Draw() and each time the terminal size is changed.
     * @param[in] x The X-coordinate of top left corner
     * @param[in] y The Y-coordinate of top left corner
     * */
    void DrawRebuild( uint32_t x, uint32_t y);

private:
    /**Current phase/screen of the menu*/
    MenuPhase m_phase;
    /**selected ithem within the screen*/
    uint32_t m_select;

    /**Draws box - the menu gfx*/
    void DrawBox();

    /**
     * Draws a button at the coordinates x,y (relatively from left top corner of the popup
     * @param [in] x X-coordinate of the button. (left top corner)
     * @param [in] y Y-coordinate of the button. (left top corner)
     * @param [in] text Button text. I recommend to add a space at the end and the beginning of the text.
     * @param [in] active Changes the colors of the button - whether it is selected or not.
     * */
    void DrawButton(int x, int y, const std::string &text, bool active);

    /**Draws the game name*/
    void DrawGameName();

    /**Game that is currently run.*/
    CGame * m_game;
    /**Ncurses window*/
    WINDOW *m_window;

    /**Ncurses window coordinates saved for new games.*/
    uint32_t m_winX, m_winY;
    /**Outputs from dialog boxes*/
    std::string m_address;
    std::string m_loadName;
};


#endif //GOPA2_MENU_HPP_B16B00B135A93A55555555M01189998819991197253
