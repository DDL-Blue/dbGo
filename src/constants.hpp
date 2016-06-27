/*
 *    ________  ________  .____      __________.__
 *    \______ \ \______ \ |    |     \______   \  |  __ __   ____
 *     |    |  \ |    |  \|    |      |    |  _/  | |  |  \_/ __ \
 *     |    `   \|    `   \    |___   |    |   \  |_|  |  /\  ___/
 *    /_______  /_______  /_______ \  |______  /____/____/  \___  >
 *            \/        \/        \/         \/                 \/
 */

#ifndef GOPA2_CONSTANTS_HPP_B16B00B135A93A55555555M01189998819991197253
#define GOPA2_CONSTANTS_HPP_B16B00B135A93A55555555M01189998819991197253

#include <cstdint>
#include <array>
#include <ncurses.h>

#define Table std::array<std::array<StoneType, TABLE_H>, TABLE_W>

/**Players*/
enum Player { PLAYER_1, PLAYER_2, PLAYER_NONE };

/**CEvent types*/
enum EventType { EV_NONE, EV_PLAYER_MOVE, EV_PLAYER_PASS, EV_PLAYER_SURRENDER, EV_GAME_END };


/**Phases of the game*/
enum GamePhase { PHASE_START, PHASE_P1_MOVE, PHASE_P2_MOVE, PHASE_END };
/**Ingame GUI states*/
enum GameFocus { GAME_FOCUS_GAME, GAME_FOCUS_GUI };
enum GameGUISelected {
    GAME_GUI_SEL_PASS, GAME_GUI_SEL_SAVE, GAME_GUI_SEL_SURRENDER
};

/**Game controls*/
const uint32_t K_LEFT = KEY_LEFT;
const uint32_t K_RIGHT = KEY_RIGHT;
const uint32_t K_UP = KEY_UP;
const uint32_t K_DOWN = KEY_DOWN;
const uint32_t K_SELECT = '\n';
const uint32_t K_SWITCH = '\t';


/**Stone types*/
enum StoneType { STONE_1, STONE_2, STONE_NONE, STONE_MARKED};
/**Stones colors*/
const uint32_t STONE_CHAR[] = { COLOR_PAIR(2) , COLOR_PAIR(3)  };
/**Game redrawing speed - frames per second*/
const uint32_t FPS = 30; //decrease for slow computers. Even like 5-15 should be fine.
const uint32_t NS_PER_FRAME = 1000000000 / FPS; //DO NOT CHANGE

/**Game objects sizes*/
const uint32_t TABLE_W = 19;
const uint32_t TABLE_H = 19;
const uint32_t TABLE_GFX_W = 57;
const uint32_t TABLE_GFX_H = 19;

const uint32_t GAME_GFX_W = 59;
const uint32_t GAME_GFX_H = 26;

/**Connection status*/
enum ConnectionStatus {CONN_OK, CONN_ERR_ADDRINFO, CONN_ERR_SOCKET, CONN_ERR_CONN, CONN_UNINITIALIZED, CONN_ERR_LISTEN, CONN_ERR_BIND, CONN_ENDED};

/**AI phases*/
enum AIStatus { AI_IDLE, AI_COMPUTING, AI_DONE };

/**Maximal size of limited version of CEvent objects*/
const uint32_t EVENT_DATA_MAX_SIZE = 64;

/**Value of a point in AI heuristics*/
const int32_t AI_POINT_VALUE = 1000;
/**Minimal number of possible moves generated in each step of AI heuristics
 * Bigger means a bit smarter and way slower AI.
 * */
const uint32_t AI_POINT_MIN_NUMBER = 3;

/**Chars used for background drawing*/
const int32_t SHADING_CHARS[] = { ' ','.','~','*','/','{','%','#' };

/**blinking cursor animations*/
const short CURSOR_COLOR_ANIMATION_1[] = { COLOR_BLACK, COLOR_BLACK, COLOR_MAGENTA, COLOR_MAGENTA, COLOR_RED, COLOR_RED, COLOR_MAGENTA,
                                           COLOR_RED, COLOR_RED, COLOR_BLACK, COLOR_BLACK, COLOR_BLACK  };

const short CURSOR_COLOR_ANIMATION_2[] = { COLOR_BLACK, COLOR_BLACK, COLOR_BLUE, COLOR_BLUE, COLOR_CYAN, COLOR_CYAN, COLOR_CYAN,
                                           COLOR_BLUE, COLOR_BLUE, COLOR_BLACK, COLOR_BLACK, COLOR_BLACK  };

/**Menu phases*/
enum MenuPhase {
    MENU_HOME, MENU_INGAME, MENU_NEWGAME, MENU_AIPICK, MENU_NETPICK, MENU_END
};

/**MsgBox popus sizes*/
const uint32_t MSGBOX_W = 40;
const uint32_t MSGBOX_H = 15; //should be smaller than GAME_GFX_* sizes

/**Maximal length of MsgBox text input*/
const uint32_t MSGBOX_INPUT_MAX_LEN = 33;

/**Name of the folder with saved games*/
const std::string SAVE_FOLDER_NAME = "saves/";


#endif //GOPA2_CONSTANTS_HPP_B16B00B135A93A55555555M01189998819991197253
