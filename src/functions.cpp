/*
 *    ________  ________  .____      __________.__
 *    \______ \ \______ \ |    |     \______   \  |  __ __   ____
 *     |    |  \ |    |  \|    |      |    |  _/  | |  |  \_/ __ \
 *     |    `   \|    `   \    |___   |    |   \  |_|  |  /\  ___/
 *    /_______  /_______  /_______ \  |______  /____/____/  \___  >
 *            \/        \/        \/         \/                 \/
 */

#include "functions.hpp"

Player getOtherPlayer (Player player){
    if (player == PLAYER_1) {return PLAYER_2;}
    if (player == PLAYER_NONE) {return PLAYER_NONE;}
    return PLAYER_1;
}

/*----------------------------------------------------------------------*/
StoneType getOtherStone (StoneType stone){
    if (stone == STONE_1) {return STONE_2;}
    if (stone == STONE_NONE) {return STONE_NONE;}
    return STONE_1;
}

/*----------------------------------------------------------------------*/
void CopyTable ( Table &dst, const Table src ){
    for (uint32_t i = 0; i < TABLE_W; ++i) {
        for (uint32_t j = 0; j < TABLE_H; ++j) {
            dst[i][j] = src[i][j];
        }
    }
}

/*----------------------------------------------------------------------*/
StoneType PlayerToStone (Player player){
    if (player == PLAYER_1){
        return STONE_1;
    }else if (player == PLAYER_2){
        return STONE_2;
    }else{
        return STONE_NONE;
    }
}

/*----------------------------------------------------------------------*/
Player StoneToPlayer (StoneType stone){
    if (stone == STONE_1){
        return PLAYER_1;
    }else if (stone == STONE_2){
        return PLAYER_2;
    }else{
        return PLAYER_NONE;
    }
}

/*----------------------------------------------------------------------*/
bool TablesEqual (const Table &table1, const Table &table2){
    for (uint32_t i = 0; i < TABLE_W; ++i) {
        for (uint32_t j = 0; j < TABLE_H; ++j) {
            if (table1[i][j] != table2[i][j]) {
                return false;
            }
        }
    }
    return true;
}

/*----------------------------------------------------------------------*/
Player PhaseToPlayer(GamePhase phase) {
    if (phase == PHASE_P1_MOVE) { return PLAYER_1; }
    if (phase == PHASE_P2_MOVE) { return PLAYER_2; }
    return PLAYER_NONE;

}















