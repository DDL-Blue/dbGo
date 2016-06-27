/*
 *    ________  ________  .____      __________.__
 *    \______ \ \______ \ |    |     \______   \  |  __ __   ____
 *     |    |  \ |    |  \|    |      |    |  _/  | |  |  \_/ __ \
 *     |    `   \|    `   \    |___   |    |   \  |_|  |  /\  ___/
 *    /_______  /_______  /_______ \  |______  /____/____/  \___  >
 *            \/        \/        \/         \/                 \/
 */


#ifndef GOPA2_TABLE_HPP_B16B00B135A93A55555555M01189998819991197253
#define GOPA2_TABLE_HPP_B16B00B135A93A55555555M01189998819991197253

#include<cstdint>
#include <ncurses.h>
#include <list>
#include <array>
class CTable;
#include "constants.hpp"
#include "structures.hpp"
#include "functions.hpp"
#include "game.hpp"

/**
 * Represents the game table. Class is storing current state of the board, performing moves and checking the rules.
 * Can be asked to answer various questions about hypothetical states of the board - which is used in AI decisions.
 * */
class CTable {
public:
    CTable();
    ~CTable();

    /**Draw the table. DrawRebuild MUST BE CALLED AT LEAST ONCE BEFORE THIS.*/
    void Draw();

    /** Sets/changes the table drawing position. Should be called at start before first Draw() and each time the terminal size is changed.
    * @param[in] x The X-coordinate of top left corner
    * @param[in] y The Y-coordinate of top left corner
    * */
    void DrawRebuild( uint32_t x, uint32_t y);

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

    /**
     * Set the cursor color according to player.
     * @param [in] player Player that is on move - will change cursor color to his color.
     * */
    void CursorSetPlayer (Player player);

    /**
     * Performs a move on a table.
     * @param [in] player Player that does this move
     * @param [in] move Structure that holds the move coordinates
     * @param [out] score Score the player gained by this move. Undefined if move was not performed.
     * @return Return true if move was performed. Returns false if move was invalid.
     * */
    bool Move ( Player player,const CMove &move, uint32_t &score);

    /**
     * Decides if this move would be valid or not.
     * @param [in] player Player that does this move
     * @param [in] move Structure that holds the move coordinates
     * @return Returns true if move is valid, false if invalid.
     * */
    bool IsValidMove ( Player player, const CMove &move ) const;


    /**
     * Determine the owner of the territory and counts number of free places within.
     * Will leave processed points as marked so it could not be counted again.
     * @param [in] move Position of free space to check.
     * @param [out] number of places in territory. 0 if not a territory.
     * @return Player the territory belongs to. Return PLAYER_NONE if not a territory, or place not empty.
     * */
    Player DetectTerrytoryDestructive(const CMove &move, uint32_t &number);

    /**
     * Determines if move is a suicide.
     * @param [in] player Player that does this move
     * @param [in] move Structure that holds the move coordinates
     * @return Returns true if move is suicide, false if it is not.
     * */
    bool IsSuicide (Player player, const CMove &move) const;

    /**
     * Get the constant reference to table array which contains all the positions on table with information about stones on them.
     * Useful for AIs, sending via network or saving game.
     * @return The table array.
     * */
    const Table& getTable() const;

    /**
     * Set table to given state. Useful for loading game etd.
     * @param [in] input The array with table.
     * */
    void setTable(const Table &input);

    /**
     * Tells if the stone is surrounded. Behavior with marked stones is undefined. Does not change the table.
     * @param [in] move Position you want to check. Is not a move in the true sense - the table will not be modified.
     * @param [in] table Table state you are calculating with.
     * @return Return true, if point is surrounded. Returns false, if point is not surrounded or empty.
     * */
    bool IsSurrounded ( const CMove &move, const Table &table ) const;

    /**
     * Takes out surrounded group of stones. Behavior if group is not surrounded or pointing to an empty place is undefined.
     * @param [in] move Position of a member of a group to be taken.
     * @param [in,out] table Table to be modified.
     * @return Number of stones taken.
     * */
    uint32_t TakeoutSurrounded ( const CMove &move, Table &table  ) const;

    /**
     * Determines whether or not would the move in given table lead to taking the stones. CMove not used because of optimisations.
     * @param [in] player Player that would perform the move.
     * @param [in] x X-coordinate of the move
     * @param [in] y Y-coordinate of the move
     * @param [in] table Situation on the board to check.
     * @return If te move takes stones, return true, otherwise return false.
     * */
    bool WouldTakeTable (Player player, uint32_t x, uint32_t y, const Table &table) const;

    /**
     * Determines if the group of stones is surrounded or not. Leaves processed stones as marked. Modifies the table.
     * Speed optimization for AI heuristics
     * @param [in] move Position to be checked.
     * @param [in,out] table Table to be processed.
     * @return Return true if position is surrounded, false if it is not or position empty.
     * */
    bool IsSurroundedDestructive (const CMove &move, Table &table) const;

    /**
     * Performes a given move on a table. Playing invalid move can lead into undefined results.
     * @param [in] player Player that plays the move.
     * @param [in] move Move to be played
     * @param [in,out] table to be processed.
     * @return Return number of stones took by the move.
     * */
    uint32_t PerformMove ( Player player, const CMove &move, Table &table  ) const;

    /**
     * Same as IsSuicide, but for any given table, not the internal one.
     * @param [in] player Player that does this move
     * @param [in] move Structure that holds the move coordinates
     * @param [in] table Table to be processed.
     * @return Returns true if move is a suicide, false if it is not.
     * */
    bool IsSuicideTable (Player player, const CMove &move, const Table &table) const;

private:
    /** Draw table grid - background gfx*/
    void DrawGrid ();

    /** Draw the cursor*/
    void DrawCursor ();

    /** Draw stones at their positions on the table*/
    void DrawStones ();

    /** Cursor X and Y coordinates, current cursor animation stadium*/
    uint32_t m_curX, m_curY, m_curAnim;
    /** Show cursor, or hide it*/
    bool m_curShow;
    /** Which player's color should cursor animation take*/
    Player m_curCol;

    /**Data 2D array that holds stone information about table positions*/
    Table m_table;

    /**History of tables*/
    std::list<Table> m_tableHistory;

    /**ncurses window for table drawing*/
    WINDOW * m_window;
};


#endif //GOPA2_TABLE_HPP_B16B00B135A93A55555555M01189998819991197253
