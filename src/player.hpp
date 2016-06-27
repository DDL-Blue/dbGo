/*
 *    ________  ________  .____      __________.__
 *    \______ \ \______ \ |    |     \______   \  |  __ __   ____
 *     |    |  \ |    |  \|    |      |    |  _/  | |  |  \_/ __ \
 *     |    `   \|    `   \    |___   |    |   \  |_|  |  /\  ___/
 *    /_______  /_______  /_______ \  |______  /____/____/  \___  >
 *            \/        \/        \/         \/                 \/
 */


#ifndef GOPA2_PLAYER_HPP_B16B00B135A93A55555555M01189998819991197253
#define GOPA2_PLAYER_HPP_B16B00B135A93A55555555M01189998819991197253

#include <cstdint>
#include <ncurses.h>
#include <set>
class CPlayer;
#include "table.hpp"
#include "game.hpp"
#include "constants.hpp"
#include "structures.hpp"
#include "connect.hpp"


/**
 * Abstract class used for polymorphism. Represents the player. It choose which moves the player will play.
 * */
class CPlayer{
public:

    CPlayer();
    virtual ~CPlayer();

    /**
     * Attach player to a game. This must be called before calling any other method.
     * @param [in] game Game that will stick to this player.
     * @param [in] player As which player will this player join the game (PLAYER1/PLAYER2)
     * */
    virtual void Attach( CGame &game, Player player);

    /** This player is now on move. On attached CPlayers only. */
    virtual void OnMove();

    /** This player is anymore on move. On attached CPlayers only. */
    virtual void NotOnMove();

    /** One tick of the game. Takes care of user keyboard input if focused (CGame.m_focus). This should be called frequently. On attached CPlayers only. */
    virtual void Tick() = 0;

    /**
     * Send an event to the player. On attached CPlayers only.
     * This is a dummy version - will just free the event.
     * @param event Pointer to the event to be sent. Method will free that pointer.
     * */
    virtual void SendEvent ( CEvent * event );

protected:
    /**Game this player is attached to*/
    CGame* m_game;
    /**As which player in game does it play*/
    Player m_player;
    /**Is this player on move right now?*/
    bool m_onMove;
};

/*############################################################################################################################*/
class CPlayerLocal : public CPlayer {
public:
    CPlayerLocal ();
    virtual ~CPlayerLocal();

    /** This player is now on move. On attached CPlayers only. */
    virtual void OnMove() override;

    /** One tick of the game. Takes care of user keyboard input if focused (CGame.m_focus). This should be called frequently. On attached CPlayers only. */
    virtual void Tick();

protected:
    /**current cursor location*/
    uint32_t m_curX, m_curY;
};


/*############################################################################################################################*/
class CPlayerNetwork : public CPlayer {
public:
    /**Constructor that accepts pointer to a CConnect object that will satisfy the connection.
     * @param [in] connect Pointer to CConnect object that will transfer all the data. Will be freed when CPlayerNetwork is destroyed.
     * */
    CPlayerNetwork(CConnect *connect);
    virtual ~CPlayerNetwork();

    /** This player is now on move. On attached CPlayers only. */
    virtual void OnMove() override;

    /** One tick of the game. Takes care of user keyboard input if focused (CGame.m_focus). This should be called frequently. On attached CPlayers only. */
    virtual void Tick();

    /**
     * Send an event to the player. On attached CPlayers only.
     * This version will send it to the oponent.
     * @param event Pointer to the event to be sent. Method will free that pointer.
     * */
    virtual void SendEvent ( CEvent * event ) override;
private:
    CConnect *m_connect;
};

/*############################################################################################################################*/
class CPlayerAI : public CPlayer {
public:
    CPlayerAI();
    virtual ~CPlayerAI();

    /** This player is now on move. On attached CPlayers only. */
    virtual void OnMove() override;

    /** One tick of the game. Takes care of user keyboard input if focused (CGame.m_focus). This should be called frequently. On attached CPlayers only. */
    virtual void Tick();

protected:
    /** AI recursive max depth*/
    uint32_t m_SEARCH_DEPTH = 4;
    /**
     * Picks some possible moves for comparison. Does not check the Ko rulle.
     * @param [in] player Player that should take this move.
     * @param [in] table that move should be performed on
     * @param [in] exact determines if the pick should be determined precisely. If set to false, there is a bigger chance for invalid move, but is much faster.
     * @return Return set of few suggested moves.
     * */
    virtual std::set<CMove> getPossibleMoves (Player player,const Table &table, bool exact = true) const;

    /**
     * Gives a score to a given move suggestion. Using recursive guessing moves.
     * @param [in] player Player that would play this move
     * @param [in] move Suggested move
     * @param [in] table actual situation on the table
     * @param [in] depthLeft depth of the search. The higher number it is, the slower the method is and better results it provides.
     * @param [in] opponentMove Is this a move of the oponent?
     * @return gives a score to the move. The higher result is, the better choice it is.
     * */
    int32_t RecursiveSearch(Player player, const CMove &move, const Table &table, uint32_t depthLeft,
                            bool opponentMove) const;

    /**
     * Finds a best move to play. Quite naive name, because the AI is as stupid as an old table with brain disorder...
     * Reflects perfectly playing skills of the author :).
     * @return The best possible move ever.
     * */
    CMove FindBestMove();

    /**Thread used for AI move calculations*/
    pthread_t m_thread;
    /**mutex that locks m_status variable that both the main and the AI threads uses*/
    pthread_mutex_t m_mutexStatus;
    /**CTable instance used for it's move analytics methods in AI heuristics.*/
    CTable * m_computer;
    /**current status of the AI*/
    AIStatus m_status;
    /**Answer found by AI thread. Not protected by mutex - access determined by m_status*/
    CMove m_result;

    /**
     * Function run in AI thread.
     * @param [in] playerAI Pointer to CPlayerAI that called this function
     * @return Undefined.
     * */
    friend void* AIComputeThread( void * playerAI);
};
/*############################################################################################################################*/
/**
 * Function run in AI thread.
 * @param [in] playerAI Pointer to CPlayerAI that called this function
 * @return Undefined.
 * */
void* AIComputeThread( void * playerAI);
/*############################################################################################################################*/

/**
 * The easiest version of AI
 * */
class CPlayerAI1 : public CPlayerAI{
public:
    CPlayerAI1();
};

/*############################################################################################################################*/
/**
 * The easier version of AI
 * */
class CPlayerAI2 : public CPlayerAI {
public:
    CPlayerAI2();
};

/*############################################################################################################################*/
/**
 * The easy version of AI
 * */
class CPlayerAI3 : public CPlayerAI {
public:
    CPlayerAI3();
};



#endif //GOPA2_PLAYER_HPP_B16B00B135A93A55555555M01189998819991197253
