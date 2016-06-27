/*
 *    ________  ________  .____      __________.__
 *    \______ \ \______ \ |    |     \______   \  |  __ __   ____
 *     |    |  \ |    |  \|    |      |    |  _/  | |  |  \_/ __ \
 *     |    `   \|    `   \    |___   |    |   \  |_|  |  /\  ___/
 *    /_______  /_______  /_______ \  |______  /____/____/  \___  >
 *            \/        \/        \/         \/                 \/
 */

#ifndef GOPA2_GAME_HPP_B16B00B135A93A55555555M01189998819991197253
#define GOPA2_GAME_HPP_B16B00B135A93A55555555M01189998819991197253

#include <cstdint>
#include <string>
#include <vector>
class CGame;
struct CMove;
struct CEvent;
#include "constants.hpp"
#include "structures.hpp"
#include "table.hpp"
#include "player.hpp"

/*############################################################################################################################*/
/**
 * This structure is used to transfer game events through the network.
 * Can serve as an universal data container. Has ability to disassemble
 * static structures and basic types to a char stream - or reverse
 * Is also used for creating save files.
 * Read and write data in the same order!
 * */
struct CEvent{
    /**
     * Constructor with possibility to set unlimited size
     * @param[in] unlimited Is container unlimited? Otherwise max size set to EVENT_DATA_MAX_SIZE (constants.hpp)
     * */
    CEvent(bool unlimited = false);

    /***
     * Reset reading cursor to begin.
     * */
    void ResetCursor();
    virtual ~CEvent(){};

    /**Content data container. It is public, so can be directly modified or asked for size etc. */
    std::vector<uint8_t>m_data;
    /** Current read position*/
    uint32_t m_readCursor;

    /**
     * Creates a new CEvent with identical content (m_data).
     * @return Returns a pointer to a new CEvent object.
     * */
    CEvent * Clone();

    /**
     * Write new data of any size to the object. This is a template.
     * Accepts only data types, that can be used in union structure - default constructor required.
     * DO NOT use for objects with dynamic allocation (most std containers)
     * because this can only store static structures.
     *
     * @param[in] data Data you want to store.
     * @return Returns true on success, false when container too small (does not apply to unlimited version viz constructor )
     * */
    template <typename T>
    bool WriteData(T data) {
        if (!m_unlimited && m_data.size() + sizeof(T) > EVENT_DATA_MAX_SIZE) { return false; }
        union Uni{
            T orig;
            uint8_t tab[sizeof(T)];
        };

        Uni uni;
        uni.orig = data;
        for (uint32_t i = 0; i < sizeof(T); ++i) {
            m_data.push_back (uni.tab[i]);
        }
        return true;
    }

    /**
     * Reads data at read cursor position and moves the read cursor.
     * @param[out] data Reference to variable you want to store read data to.
     *@return Returns true on success or false if there is not enough data to read.
     * */
    template <typename T>
    bool ReadData (T &data){
        if ((m_data.size()-m_readCursor)<sizeof(T)) {return false;}
        union Uni{
            T orig;
            uint8_t tab[sizeof(T)];
        };
        Uni uni;
        for (uint32_t i = 0; i < sizeof(T); ++i) {
            uni.tab[i] = m_data[m_readCursor];
            ++m_readCursor;
        }
        data = uni.orig;
        return true;
    }

private:
    bool m_unlimited;

};

/*############################################################################################################################*/

/**
 * This class takes care of one whole game.
 * It is counting score, controlling the moves and connecting the whole game together.
 */
class CGame{
public:
    /**
     * The constructor. Accepts two pointers to players that will play the game.
     * @param[in] player1 Pointer to a red player. CGame will free this pointer when deleted
     * @param[in] player2 Pointer to a blue player. CGame will free this pointer when deleted.
     * */
    CGame ( CPlayer * player1, CPlayer * player2 );
    ~CGame();

    /** Perform one game tick. This should be called frequently.
     * @return Returns true if game is running, false if game reached it's end.
     * */
    bool Tick();

    /** Drawes the game. CALL DrawRebuild(...) AT LEAS ONCE BEFORE USING THIS METHOD! */
    void Draw();

    /** Sets/changes the game drawing position. Should be called at start before Draw() and each time the terminal size is changed.
     * @param[in] x The X-coordinate of top left corner
     * @param[in] y The Y-coordinate of top left corner
     * */
    void DrawRebuild( uint32_t x, uint32_t y);

    /**
     * Performs a move.
     * @param[in] player Which player does this move.
     * @param[in] move Structure that contains coordinates you want to put a stone to.
     * @return Returns true on success, otherwise returns false.
     * */
    bool Move ( Player player,const CMove &move);

    /**
     * Performs a pass.
     * @param[in] player Which player wants to pass.
     * */
    void Pass (Player player);

    /**
     * Ends the game. Calculate results. Shows them. Next tick() will return false and game will end.
     * */
    void GameEnd();

    /**
    * Player surrender.
    * @param[in] player Which player wants to surrender.
    * */
    void Surrender ( Player player);

    /**
     * Get constant reference to a table the game is played on. Useful for AIs.
     * @return Returns a constant reference of CTable object game uses.
     * */
    const CTable& getTable() const;

    /**
     * Send an event to this game. Useful on network games - a way to get an information about other player's actions.
     * @param[in] event Pointer to event to send. Method will free this pointer.
     * */
    void SendEvent (CEvent * event);

    /**
     * Saves game to a file.
     * @param[in] name Name of the save
     * @return Returns true on success otherwise false (with error popup message).
     * */
    bool GameSave(const std::string &name);

    /**
     * Loads game from a file.
     * @param[in] name Name of the save
     * @return Returns true on success otherwise false (with error popup message).
     * */
    bool GameLoad(const std::string &name);

    //cursor
    /**
     * Set cursor position on the board.
     * @param[in] x The X-coordinate
     * @param[in] y The Y-coordinate
     * */
    void SetCursor (uint32_t x, uint32_t y);

    /**
     * Show the cursor.
     * */
    void ShowCursor ();

    /**
     * Hide the cursor.
     * */
    void HideCursor ();

    /**Determines if controlling table cursor or GUI menu*/
    GameFocus m_focus;
private:
    /** The other player is now on move. */
    void TogglePlayer();

    /** Did the previous player pass?*/
    bool m_passed;

    /** Resend this event to the first player, clone it for the second one
     * Takes responsibility for the memory management - the pointer will be freed.
     * */
    void SendEventToPlayers (CEvent * event);

    /**Table object the game is played on.*/
    CTable * m_table;
    /**Which GUI button is selected*/
    GameGUISelected m_guiSelected;
    /**Players's current score*/
    uint32_t m_score[2];
    /**Playing players*/
    CPlayer * m_player[2];
    /**phase of the game -P1 move, P2 move, end etc*/
    GamePhase m_phase;

    /**Draw the gui around the table*/
    void DrawGUI();

    /**Draw buttons to the GUI*/
    void DrawGUIButtons();

    /** ncurses windows for GUI*/
    WINDOW *m_windowTop, *m_windowRight, *m_windowLeft, *m_windowBottom;


};

/*############################################################################################################################*/


#endif //GOPA2_GAME_HPP_B16B00B135A93A55555555M01189998819991197253
