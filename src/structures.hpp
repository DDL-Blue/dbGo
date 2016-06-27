/*
 *    ________  ________  .____      __________.__
 *    \______ \ \______ \ |    |     \______   \  |  __ __   ____
 *     |    |  \ |    |  \|    |      |    |  _/  | |  |  \_/ __ \
 *     |    `   \|    `   \    |___   |    |   \  |_|  |  /\  ___/
 *    /_______  /_______  /_______ \  |______  /____/____/  \___  >
 *            \/        \/        \/         \/                 \/
 */


#ifndef GOPA2_STRUCTURES_HPP_B16B00B135A93A55555555M01189998819991197253
#define GOPA2_STRUCTURES_HPP_B16B00B135A93A55555555M01189998819991197253

#include <cstdint>
#include "constants.hpp"

struct CMove{
    CMove () = default;
    CMove(uint32_t x, uint32_t y) : m_x (x), m_y (y) {}
    uint32_t m_x, m_y;

    bool operator<(const CMove &other) const{if (m_x<other.m_x){return true;}if (m_x>other.m_x){return false;} return m_y < other.m_y;}
};
/*############################################################################################################################*/


#endif //GOPA2_STRUCTURES_HPP_B16B00B135A93A55555555M01189998819991197253