/*
 *    ________  ________  .____      __________.__
 *    \______ \ \______ \ |    |     \______   \  |  __ __   ____
 *     |    |  \ |    |  \|    |      |    |  _/  | |  |  \_/ __ \
 *     |    `   \|    `   \    |___   |    |   \  |_|  |  /\  ___/
 *    /_______  /_______  /_______ \  |______  /____/____/  \___  >
 *            \/        \/        \/         \/                 \/
 */

#ifndef GOPA2_FUNCTIONS_HPP_B16B00B135A93A55555555M01189998819991197253
#define GOPA2_FUNCTIONS_HPP_B16B00B135A93A55555555M01189998819991197253

#include <array>
#include "constants.hpp"

/**
 * Get the oponent.
 * @param [in] player One player.
 * @return The other player.
 * */
Player getOtherPlayer (Player player);
/*----------------------------------------------------------------------*/
/**
 * Get the oponents's stone.
 * @param [in] stone One stone.
 * @return The other stone.
 * */
StoneType getOtherStone (StoneType stone);
/*----------------------------------------------------------------------*/
/**
 * get the stone of a given player
 * @param [in] player The player.
 * @return The player's stone.
 * */
StoneType PlayerToStone (Player player);
/*----------------------------------------------------------------------*/
/**
 * get the player that uses this stone type.
 * @param [in] stone The stone type.
 * @return The user of these stones.
 * */
Player StoneToPlayer (StoneType stone);
/*----------------------------------------------------------------------*/
bool TablesEqual (const Table &table1, const Table &table2);
/*----------------------------------------------------------------------*/
void CopyTable ( Table &dst, const Table src );

/*----------------------------------------------------------------------*/
/**
 * Tells which player is on move at the given phase.
 * @param [in] phase The phase you ask about.
 * @return The player that is on move in this phase.
 * */
Player PhaseToPlayer(GamePhase phase);

#endif //GOPA2_FUNCTIONS_HPP_B16B00B135A93A55555555M01189998819991197253
