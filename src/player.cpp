/*
 *    ________  ________  .____      __________.__
 *    \______ \ \______ \ |    |     \______   \  |  __ __   ____
 *     |    |  \ |    |  \|    |      |    |  _/  | |  |  \_/ __ \
 *     |    `   \|    `   \    |___   |    |   \  |_|  |  /\  ___/
 *    /_______  /_______  /_______ \  |______  /____/____/  \___  >
 *            \/        \/        \/         \/                 \/
 */

#include <cstdlib>
#include <utility>
#include <unistd.h>
#include "player.hpp"

CPlayer::CPlayer() : m_game ( nullptr ), m_onMove(false) {

}

CPlayer::~CPlayer() {

}

/*----------------------------------------------------------------------*/
void CPlayer::Attach( CGame &game, Player player) {
    m_game = &game;
    m_player = player;
}

/*----------------------------------------------------------------------*/
void CPlayer::OnMove() {
    m_onMove = true;
}

/*----------------------------------------------------------------------*/
void CPlayer::NotOnMove() {
    m_onMove = false;
}

/*----------------------------------------------------------------------*/
void CPlayer::SendEvent(CEvent *event) {
    delete event;
    return;
}
/*############################################################################################################################*/
CPlayerLocal::CPlayerLocal() : m_curX (0), m_curY (0) {

}

/*----------------------------------------------------------------------*/
CPlayerLocal::~CPlayerLocal() {

}

/*----------------------------------------------------------------------*/
void CPlayerLocal::Tick() {
    if (m_onMove) {

        if (m_game->m_focus==GAME_FOCUS_GAME) {
            int chr = getch();
            switch (chr) {
                case K_LEFT:
                    if (m_curX) {
                        --m_curX;
                    }
                    break;
                case K_RIGHT:
                    if (m_curX < TABLE_W - 1) {
                        ++m_curX;
                    }
                    break;
                case K_DOWN:
                    if (m_curY < TABLE_H - 1) {
                        ++m_curY;
                    }
                    break;
                case K_UP:
                    if (m_curY) {
                        --m_curY;
                    }
                    break;
                case K_SELECT:
                    m_game->Move(m_player, CMove(m_curX, m_curY));
                    break;
                case 'q':
                    m_game->Surrender(m_player);
                case K_SWITCH:
                    m_game->m_focus = GAME_FOCUS_GUI;
                    break;

                default:
                    break;
            }
        }
        m_game->SetCursor(m_curX, m_curY);
    }
}

/*----------------------------------------------------------------------*/
void CPlayerLocal::OnMove() {
    m_onMove = true;
    m_game->ShowCursor();
}


/*############################################################################################################################*/
CPlayerNetwork::CPlayerNetwork(CConnect *connect) : m_connect (connect) {
}

/*----------------------------------------------------------------------*/
CPlayerNetwork::~CPlayerNetwork() {
    delete m_connect;
}

/*----------------------------------------------------------------------*/

void CPlayerNetwork::OnMove() {
    m_onMove = true;
    m_game->HideCursor();
}

/*----------------------------------------------------------------------*/
void CPlayerNetwork::Tick() {

    if (m_connect->getStatus() != CONN_OK){
        m_game->Surrender (m_player);
    }
    if (m_game->m_focus==GAME_FOCUS_GAME) {
        if (m_onMove) {
            int chr = getch();
            switch (chr) {
                case 'q':
                    m_game->Surrender(m_player);
                case K_SWITCH:
                    m_game->m_focus = GAME_FOCUS_GUI;
                    break;
                default:
                    break;
            }
        }
    }

    CEvent * ev;
    ev = m_connect->getEvent();
    if (ev){
        m_game->SendEvent (ev);
    }


}

    /*----------------------------------------------------------------------*/
void CPlayerNetwork::SendEvent ( CEvent * event ) {
    m_connect->SendEvent(event);
}
/*############################################################################################################################*/
CPlayerAI::CPlayerAI() : m_status (AI_IDLE) {
    m_mutexStatus = PTHREAD_MUTEX_INITIALIZER;
    m_computer = new CTable();
}

/*----------------------------------------------------------------------*/
CPlayerAI::~CPlayerAI() {

    pthread_mutex_lock(&m_mutexStatus);
    if (m_status == AI_COMPUTING) {
        pthread_mutex_unlock(&m_mutexStatus);
        pthread_join(m_thread, NULL);
    } else {
        pthread_mutex_unlock(&m_mutexStatus);
    }

    sleep(1);
    delete m_computer;
}

/*----------------------------------------------------------------------*/
std::set<CMove> CPlayerAI::getPossibleMoves(Player player, const Table &table, bool exact) const{
    std::set<CMove> rtrn;

//Add scoring and moves potentionally dangerous places
    for (uint32_t i = 0; i < TABLE_W; ++i) {
        for (uint32_t j = 0; j < TABLE_H; ++j) {
            if (table[i][j]==STONE_NONE){ //for each free position
                if (!exact || !m_computer->IsSuicideTable ( player, CMove(i,j), table) ) { //each possible move
                    if (m_computer->WouldTakeTable(player, i, j, table)){ //scoring
                        rtrn.insert( CMove(i, j));
                        continue;
                    }
                    if (m_computer->WouldTakeTable(getOtherPlayer(player), i, j, table)){ //dangerous
                        rtrn.insert( CMove(i, j));
                        continue;
                    }
                }
            }
        }
    }

    //insert some genius fast neuron-network-based heuristic algorithm here :)

    //if too few moves, add some random
    uint32_t triesToGiveUp = 666;
    while (rtrn.size()<AI_POINT_MIN_NUMBER){
        if (! --triesToGiveUp ) {break;}
        CMove move( std::rand()%TABLE_W, std::rand()%TABLE_H);
        if (table[move.m_x][move.m_y] != STONE_NONE){ continue; }
        if (! m_computer->IsSuicideTable(player, move, table )){
            rtrn.insert (move);
        }
    }

return rtrn;
}

/*----------------------------------------------------------------------*/
int32_t CPlayerAI::RecursiveSearch(Player player, const CMove &move, const Table &table, uint32_t depthLeft,
                                   bool opponentMove) const {

    Table tmpTable, processedTable;
    processedTable = table;

    int32_t moveValue = m_computer->PerformMove(player, move, processedTable)*AI_POINT_VALUE;
    if (opponentMove) {
        moveValue *= -1; //such a evil move :!
    }

    if (moveValue<0) { depthLeft = 0; } //negative move, stop recursion at this point

    std::set<CMove>moveSet = getPossibleMoves(player, table, false);

    int32_t sum = 0;
    int32_t num = moveSet.size();

        for (auto &s : moveSet) {
            if (depthLeft) {
                sum += RecursiveSearch(getOtherPlayer(player), s, processedTable, depthLeft - 1, !opponentMove);
            }else{
                tmpTable = table;

                if (opponentMove) {
                    sum -= m_computer->PerformMove(player, s, tmpTable)*AI_POINT_VALUE;
                }else{
                    sum += m_computer->PerformMove(player, s, tmpTable)*AI_POINT_VALUE;
                }
            }
        }

    return moveValue+(sum/num);
}

/*----------------------------------------------------------------------*/
CMove CPlayerAI::FindBestMove() {
    Table table = m_game->getTable().getTable();

    std::set<CMove> moves = getPossibleMoves(m_player, table);

    std::list<std::pair<CMove, int32_t>> answers;

    for (auto &s : moves){
        answers.push_back ( std::pair<CMove, int32_t>( s, RecursiveSearch(m_player, s, table, m_SEARCH_DEPTH, false) ) );
    }

    CMove winner;
    int32_t winnerScore = -9999999;

    for (auto &s : answers){
        if (s.second>winnerScore || m_game->getTable().IsValidMove(m_player,s.first)){
            winner = s.first;
            winnerScore = s.second;
        }
    }
    return winner;
}

/*----------------------------------------------------------------------*/
void CPlayerAI::OnMove() {
    m_onMove = true;
    m_game->HideCursor();
}

/*----------------------------------------------------------------------*/
void CPlayerAI::Tick() {
    if (m_onMove) {
        if (m_game->m_focus==GAME_FOCUS_GAME) {
            int chr = getch();
            switch (chr) {
                case 'q':
                    m_game->Surrender(m_player);
                case K_SWITCH:
                    m_game->m_focus = GAME_FOCUS_GUI;
                    break;
                default:
                    break;
            }
        }

        pthread_mutex_lock(&m_mutexStatus);
        if (m_status == AI_IDLE){
            int ret = pthread_create( &m_thread, NULL, AIComputeThread, (void*) this);
            if (ret){throw "CPlayerAI::Tick::pthread_create failed";}

        }else if (m_status == AI_DONE){
            pthread_join (m_thread, NULL);
            m_game->Move (m_player, m_result);
            m_status = AI_IDLE;
        }
        pthread_mutex_unlock(&m_mutexStatus);



    }
}
/*############################################################################################################################*/
CPlayerAI1::CPlayerAI1() {
    m_SEARCH_DEPTH = 3;
}

/*----------------------------------------------------------------------*/

CPlayerAI2::CPlayerAI2() {
    m_SEARCH_DEPTH = 6;
}

/*----------------------------------------------------------------------*/

CPlayerAI3::CPlayerAI3() {
    m_SEARCH_DEPTH = 10;
}

/*----------------------------------------------------------------------*/

/*############################################################################################################################*/
void* AIComputeThread( void * playerAI){
    CPlayerAI * player = (CPlayerAI*) playerAI;

    CMove ret;

    pthread_mutex_lock(&(player->m_mutexStatus));
    player->m_status = AI_COMPUTING;
    pthread_mutex_unlock(&(player->m_mutexStatus));

    ret = player->FindBestMove();

    pthread_mutex_lock(&(player->m_mutexStatus));

    player->m_result = ret;
    player->m_status = AI_DONE;


    pthread_mutex_unlock(&(player->m_mutexStatus));

    return nullptr;
}

