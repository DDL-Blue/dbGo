/*
 *    ________  ________  .____      __________.__
 *    \______ \ \______ \ |    |     \______   \  |  __ __   ____
 *     |    |  \ |    |  \|    |      |    |  _/  | |  |  \_/ __ \
 *     |    `   \|    `   \    |___   |    |   \  |_|  |  /\  ___/
 *    /_______  /_______  /_______ \  |______  /____/____/  \___  >
 *            \/        \/        \/         \/                 \/
 */

#include "functions.hpp"
#include "table.hpp"

CTable::CTable() : m_curX(0), m_curY(0), m_curAnim(0), m_curShow(true), m_curCol(PLAYER_1) {
    //clear table
    for (uint32_t i = 0; i < TABLE_W; ++i) {
        for (uint32_t j = 0; j < TABLE_H; ++j) {
            m_table[i][j] = STONE_NONE;
        }
    }

    m_window = newwin(0, 0, 0, 0);
}

/*----------------------------------------------------------------------*/
CTable::~CTable() {
    delwin(m_window);
}

/*----------------------------------------------------------------------*/
bool CTable::IsSuicideTable(Player player, const CMove &move, const Table &table) const {
    Table tmpTable = table;
    tmpTable[move.m_x][move.m_y] = PlayerToStone(player); //simulate the move on tmp table

    if (!IsSurrounded(CMove(move.m_x, move.m_y), tmpTable)) { return false; } //does not kill you

    if (move.m_x) { //left
        if (tmpTable[move.m_x - 1][move.m_y] == getOtherStone(tmpTable[move.m_x][move.m_y])
            && IsSurrounded(CMove(move.m_x - 1, move.m_y), tmpTable)) {
            return false;
        }
    }

    if (move.m_y) { //up
        if (tmpTable[move.m_x][move.m_y - 1] == getOtherStone(tmpTable[move.m_x][move.m_y])
            && IsSurrounded(CMove(move.m_x, move.m_y - 1), tmpTable)) {
            return false;
        }
    }

    if (move.m_x < TABLE_W - 1) { //right
        if (tmpTable[move.m_x + 1][move.m_y] == getOtherStone(tmpTable[move.m_x][move.m_y])
            && IsSurrounded(CMove(move.m_x + 1, move.m_y), tmpTable)) {
            return false;
        }
    }

    if (move.m_y < TABLE_H - 1) { //down
        if (tmpTable[move.m_x][move.m_y + 1] == getOtherStone(tmpTable[move.m_x][move.m_y])
            && IsSurrounded(CMove(move.m_x, move.m_y + 1), tmpTable)) {
            return false;
        }
    }

    return true;
}

/*----------------------------------------------------------------------*/
bool CTable::IsSuicide(Player player, const CMove &move) const {
    return IsSuicideTable(player, move, m_table);
}

/*----------------------------------------------------------------------*/
bool CTable::IsSurrounded(const CMove &move, const Table &table) const {
    Table tmpTable = table;

    if (tmpTable[move.m_x][move.m_y] == STONE_NONE) { return false; } //point is empty

    StoneType stoneType = table[move.m_x][move.m_y];

    std::list<CMove> todoList;

    todoList.push_back(move);

    while (!todoList.empty()) {
        if (todoList.front().m_x) { //left
            if (tmpTable[todoList.front().m_x - 1][todoList.front().m_y] == stoneType) {
                todoList.push_back(CMove(todoList.front().m_x - 1, todoList.front().m_y));
                tmpTable[todoList.front().m_x - 1][todoList.front().m_y] = STONE_MARKED;
            } else {
                if (tmpTable[todoList.front().m_x - 1][todoList.front().m_y] == STONE_NONE) {
                    return false;
                }
            }
        }

        if (todoList.front().m_y) { //up
            if (tmpTable[todoList.front().m_x][todoList.front().m_y - 1] == stoneType) {
                todoList.push_back(CMove(todoList.front().m_x, todoList.front().m_y - 1));
                tmpTable[todoList.front().m_x][todoList.front().m_y - 1] = STONE_MARKED;
            } else {
                if (tmpTable[todoList.front().m_x][todoList.front().m_y - 1] == STONE_NONE) {
                    return false;
                }
            }
        }

        if (todoList.front().m_x < TABLE_W - 1) { //right
            if (tmpTable[todoList.front().m_x + 1][todoList.front().m_y] == stoneType) {
                todoList.push_back(CMove(todoList.front().m_x + 1, todoList.front().m_y));
                tmpTable[todoList.front().m_x + 1][todoList.front().m_y] = STONE_MARKED;
            } else {
                if (tmpTable[todoList.front().m_x + 1][todoList.front().m_y] == STONE_NONE) {
                    return false;
                }
            }
        }

        if (todoList.front().m_y < TABLE_H - 1) { //down
            if (tmpTable[todoList.front().m_x][todoList.front().m_y + 1] == stoneType) {
                todoList.push_back(CMove(todoList.front().m_x, todoList.front().m_y + 1));
                tmpTable[todoList.front().m_x][todoList.front().m_y + 1] = STONE_MARKED;
            } else {
                if (tmpTable[todoList.front().m_x][todoList.front().m_y + 1] == STONE_NONE) {
                    return false;
                }
            }
        }

        todoList.pop_front();
    }
    return true;
}

/*----------------------------------------------------------------------*/
uint32_t CTable::TakeoutSurrounded(const CMove &move, Table &table) const {
    if (!IsSurrounded(move, table)) { return 0; }

    uint32_t cnt = 0;

    StoneType stoneType = table[move.m_x][move.m_y];

    std::list<CMove> todoList;

    todoList.push_back(move);
    table[move.m_x][move.m_y] = STONE_NONE;

    while (!todoList.empty()) {
        ++cnt;

        if (todoList.front().m_x) { //left
            if (table[todoList.front().m_x - 1][todoList.front().m_y] == stoneType) {
                todoList.push_back(CMove(todoList.front().m_x - 1, todoList.front().m_y));
                table[todoList.front().m_x - 1][todoList.front().m_y] = STONE_NONE;
            }
        }

        if (todoList.front().m_y) { //up
            if (table[todoList.front().m_x][todoList.front().m_y - 1] == stoneType) {
                todoList.push_back(CMove(todoList.front().m_x, todoList.front().m_y - 1));
                table[todoList.front().m_x][todoList.front().m_y - 1] = STONE_NONE;
            }
        }

        if (todoList.front().m_x < TABLE_W - 1) { //right
            if (table[todoList.front().m_x + 1][todoList.front().m_y] == stoneType) {
                todoList.push_back(CMove(todoList.front().m_x + 1, todoList.front().m_y));
                table[todoList.front().m_x + 1][todoList.front().m_y] = STONE_NONE;
            }
        }

        if (todoList.front().m_y < TABLE_H - 1) { //down
            if (table[todoList.front().m_x][todoList.front().m_y + 1] == stoneType) {
                todoList.push_back(CMove(todoList.front().m_x, todoList.front().m_y + 1));
                table[todoList.front().m_x][todoList.front().m_y + 1] = STONE_NONE;
            }
        }
        todoList.pop_front();
    }

    return cnt;
}

/*----------------------------------------------------------------------*/
Player CTable::DetectTerrytoryDestructive(const CMove &move, uint32_t &number) {
    number = 0;

    if (m_table[move.m_x][move.m_y] != STONE_NONE) {
        return PLAYER_NONE;
    }

    StoneType territory = STONE_NONE;

    bool isTerritory = true;

    std::list<CMove> todoList;

    uint32_t x, y;

    todoList.push_back(move);
    m_table[move.m_x][move.m_y] = STONE_MARKED;

    while (!todoList.empty()) {
        ++number;

        x = todoList.front().m_x;
        y = todoList.front().m_y;

        if (x) { //left
            if (m_table[x - 1][y] == STONE_NONE) {
                todoList.push_back(CMove(x - 1, y));
                m_table[x - 1][y] = STONE_MARKED;
            } else {
                if (m_table[x - 1][y] == STONE_1) {
                    if (territory == STONE_NONE) { territory = STONE_1; } else {
                        isTerritory *= (territory == STONE_1);
                    }
                } else if (m_table[x - 1][y] == STONE_2) {
                    if (territory == STONE_NONE) { territory = STONE_2; } else {
                        isTerritory *= (territory == STONE_2);
                    }
                }
            }
        }

        if (y) { //up
            if (m_table[x][y - 1] == STONE_NONE) {
                todoList.push_back(CMove(x, y - 1));
                m_table[x][y - 1] = STONE_MARKED;
            } else {
                if (m_table[x][y - 1] == STONE_1) {
                    if (territory == STONE_NONE) { territory = STONE_1; } else {
                        isTerritory *= (territory == STONE_1);
                    }
                } else if (m_table[x][y - 1] == STONE_2) {
                    if (territory == STONE_NONE) { territory = STONE_2; } else {
                        isTerritory *= (territory == STONE_2);
                    }
                }
            }
        }

        if (x < TABLE_W - 1) { //right
            if (m_table[x + 1][y] == STONE_NONE) {
                todoList.push_back(CMove(x + 1, y));
                m_table[x + 1][y] = STONE_MARKED;
            } else {
                if (m_table[x + 1][y] == STONE_1) {
                    if (territory == STONE_NONE) { territory = STONE_1; } else {
                        isTerritory *= (territory == STONE_1);
                    }
                } else if (m_table[x + 1][y] == STONE_2) {
                    if (territory == STONE_NONE) { territory = STONE_2; } else {
                        isTerritory *= (territory == STONE_2);
                    }
                }
            }
        }

        if (y < TABLE_H - 1) { //down
            if (m_table[x][y + 1] == STONE_NONE) {
                todoList.push_back(CMove(x, y + 1));
                m_table[x][y + 1] = STONE_MARKED;
            } else {
                if (m_table[x][y + 1] == STONE_1) {
                    if (territory == STONE_NONE) { territory = STONE_1; } else {
                        isTerritory *= (territory == STONE_1);
                    }
                } else if (m_table[x][y + 1] == STONE_2) {
                    if (territory == STONE_NONE) { territory = STONE_2; } else {
                        isTerritory *= (territory == STONE_2);
                    }
                }
            }
        }

        todoList.pop_front();
    }

    if (!isTerritory || territory == STONE_NONE) {
        number = 0;
        return PLAYER_NONE;
    } else {

        return StoneToPlayer(territory);
    }
}

/*----------------------------------------------------------------------*/
bool CTable::IsValidMove(Player player, const CMove &move) const {

    if (m_table[move.m_x][move.m_y] != STONE_NONE) { return false; } //empty places only

    if (IsSuicide(player, move)) { return false; } //forbid the suicide

    if (m_tableHistory.size() < 2) { return true; } //the KO rule
    Table tmpTable;
    CopyTable(tmpTable, m_table);
    PerformMove(player, move, tmpTable);
    auto it = m_tableHistory.begin();
    ++it; //++it;
    return !TablesEqual(*it, tmpTable);
}

/*----------------------------------------------------------------------*/
uint32_t CTable::PerformMove(Player player, const CMove &move, Table &table) const {
    uint32_t cnt = 0;

    table[move.m_x][move.m_y] = PlayerToStone(player);

    if (move.m_x && table[move.m_x - 1][move.m_y] == getOtherStone(PlayerToStone(player))
        && IsSurrounded(CMove(move.m_x - 1, move.m_y), table)) { //left
        cnt += TakeoutSurrounded(CMove(move.m_x - 1, move.m_y), table);
    }

    if (move.m_y && table[move.m_x][move.m_y - 1] == getOtherStone(PlayerToStone(player))
        && IsSurrounded(CMove(move.m_x, move.m_y - 1), table)) { //up
        cnt += TakeoutSurrounded(CMove(move.m_x, move.m_y - 1), table);
    }

    if (move.m_x < TABLE_W - 1 && table[move.m_x + 1][move.m_y] == getOtherStone(PlayerToStone(player))
        && IsSurrounded(CMove(move.m_x + 1, move.m_y), table)) { //left
        cnt += TakeoutSurrounded(CMove(move.m_x + 1, move.m_y), table);
    }

    if (move.m_y < TABLE_H - 1 && table[move.m_x][move.m_y + 1] == getOtherStone(PlayerToStone(player))
        && IsSurrounded(CMove(move.m_x, move.m_y + 1), table)) { //up
        cnt += TakeoutSurrounded(CMove(move.m_x, move.m_y + 1), table);
    }

    return cnt;
}

/*----------------------------------------------------------------------*/
bool CTable::IsSurroundedDestructive(const CMove &move, Table &table) const {

    if (table[move.m_x][move.m_y] == STONE_NONE ||
        table[move.m_x][move.m_y] == STONE_MARKED) { return false; } //point is empty or already processed

    StoneType stoneType = table[move.m_x][move.m_y];

    std::list<CMove> todoList;

    todoList.push_back(move);

    while (!todoList.empty()) {
        if (todoList.front().m_x) { //left
            if (table[todoList.front().m_x - 1][todoList.front().m_y] == stoneType) {
                todoList.push_back(CMove(todoList.front().m_x - 1, todoList.front().m_y));
                table[todoList.front().m_x - 1][todoList.front().m_y] = STONE_MARKED;
            } else {
                if (table[todoList.front().m_x - 1][todoList.front().m_y] == STONE_NONE) {
                    return false;
                }
            }
        }

        if (todoList.front().m_y) { //up
            if (table[todoList.front().m_x][todoList.front().m_y - 1] == stoneType) {
                todoList.push_back(CMove(todoList.front().m_x, todoList.front().m_y - 1));
                table[todoList.front().m_x][todoList.front().m_y - 1] = STONE_MARKED;
            } else {
                if (table[todoList.front().m_x][todoList.front().m_y - 1] == STONE_NONE) {
                    return false;
                }
            }
        }

        if (todoList.front().m_x < TABLE_W - 1) { //right
            if (table[todoList.front().m_x + 1][todoList.front().m_y] == stoneType) {
                todoList.push_back(CMove(todoList.front().m_x + 1, todoList.front().m_y));
                table[todoList.front().m_x + 1][todoList.front().m_y] = STONE_MARKED;
            } else {
                if (table[todoList.front().m_x + 1][todoList.front().m_y] == STONE_NONE) {
                    return false;
                }
            }
        }

        if (todoList.front().m_y < TABLE_H - 1) { //down
            if (table[todoList.front().m_x][todoList.front().m_y + 1] == stoneType) {
                todoList.push_back(CMove(todoList.front().m_x, todoList.front().m_y + 1));
                table[todoList.front().m_x][todoList.front().m_y + 1] = STONE_MARKED;
            } else {
                if (table[todoList.front().m_x][todoList.front().m_y + 1] == STONE_NONE) {
                    return false;
                }
            }
        }

        todoList.pop_front();
    }
    return true;
}

/*----------------------------------------------------------------------*/
bool CTable::WouldTakeTable(Player player, uint32_t x, uint32_t y, const Table &table) const {

    Table tmpTable = table;
    tmpTable[x][y] = PlayerToStone(player);
    StoneType stone = getOtherStone(PlayerToStone(player));

    if (x && table[x - 1][y] == stone
        && IsSurroundedDestructive(CMove(x - 1, y), tmpTable)) { //left
        return true;
    }

    if (y && table[x][y - 1] == stone
        && IsSurroundedDestructive(CMove(x, y - 1), tmpTable)) { //up
        return true;
    }

    if (x < TABLE_W - 1 && table[x + 1][y] == stone
        && IsSurroundedDestructive(CMove(x + 1, y), tmpTable)) { //left
        return true;
    }

    if (y < TABLE_H - 1 && table[x][y + 1] == stone
        && IsSurroundedDestructive(CMove(x, y + 1), tmpTable)) { //up
        return true;
    }
    return false;
}

/*----------------------------------------------------------------------*/
bool CTable::Move(Player player, const CMove &mov, uint32_t &score) {

    if (!IsValidMove(player, mov)) { return false; }

    score = PerformMove(player, mov, m_table);
    m_tableHistory.push_front(m_table);

    return true;
}

/*----------------------------------------------------------------------*/
void CTable::DrawRebuild(uint32_t x, uint32_t y) {
    delwin(m_window);
    m_window = newwin(19, 57, y, x);
    return;
}

/*----------------------------------------------------------------------*/
void CTable::Draw() {

    DrawGrid();
    DrawCursor();
    DrawStones();

    wrefresh(m_window);
}

/*----------------------------------------------------------------------*/
void CTable::DrawStones() {
    for (uint32_t i = 0; i < TABLE_W; ++i) {
        for (uint32_t j = 0; j < TABLE_H; ++j) {
            if (m_table[i][j] != STONE_NONE) {
                mvwaddch (m_window, j, i * 3 + 1, ACS_CKBOARD | STONE_CHAR[m_table[i][j]]);
            }
        }
    }
}

/*----------------------------------------------------------------------*/
void CTable::DrawCursor() {

    m_curAnim = (m_curAnim + 1) % 24;
    if (m_curCol == PLAYER_1) {
        init_pair(4, CURSOR_COLOR_ANIMATION_1[m_curAnim / 2], COLOR_WHITE);
    } else {
        init_pair(4, CURSOR_COLOR_ANIMATION_2[m_curAnim / 2], COLOR_WHITE);
    }


    if (m_curShow) {
        wmove(m_window, m_curY, m_curX * 3);
        wattron (m_window, COLOR_PAIR(4));
        waddch(m_window, '[' | A_BOLD);
        wmove(m_window, m_curY, m_curX * 3 + 2);
        waddch(m_window, ']' | A_BOLD);
        wattroff (m_window, COLOR_PAIR(4));
    }
}

/*----------------------------------------------------------------------*/
void CTable::SetCursor(uint32_t x, uint32_t y) {
    m_curX = x;
    m_curY = y;
}

/*----------------------------------------------------------------------*/
void CTable::CursorSetPlayer(Player player) {
    m_curCol = player;
}

/*----------------------------------------------------------------------*/
void CTable::ShowCursor() {
    m_curShow = true;
}

/*----------------------------------------------------------------------*/
void CTable::HideCursor() {
    m_curShow = false;
}

/*----------------------------------------------------------------------*/
const Table &CTable::getTable() const {
    return m_table;
}

/*----------------------------------------------------------------------*/
void CTable::setTable(const Table &input) {
    m_table = input;
}

/*----------------------------------------------------------------------*/
void CTable::DrawGrid() {
    wattron(m_window, COLOR_PAIR(1));
    //top
    wmove(m_window, 0, 0);
    waddch(m_window, ' ');
    waddch(m_window, ACS_ULCORNER);
    for (uint32_t i = 1; i < TABLE_W - 1; ++i) {
        waddch(m_window, ACS_HLINE);
        waddch(m_window, ACS_HLINE);
        waddch(m_window, ACS_TTEE);
    }
    waddch(m_window, ACS_HLINE);
    waddch(m_window, ACS_HLINE);
    waddch(m_window, ACS_URCORNER);
    waddch(m_window, ' ');

    wmove(m_window, 1, 0);
    waddch(m_window, ' ');

    //middle
    for (uint32_t i = 0; i < TABLE_H - 1; ++i) {
        wmove(m_window, 1 * i + 1, 0);
        waddch(m_window, ' ');
        waddch(m_window, ACS_LTEE);
        waddch(m_window, ACS_HLINE);
        waddch(m_window, ACS_HLINE);
        for (uint32_t j = 0; j < TABLE_W - 2; ++j) {
            waddch(m_window, ACS_PLUS);
            waddch(m_window, ACS_HLINE);
            waddch(m_window, ACS_HLINE);
        }
        waddch(m_window, ACS_RTEE);
        waddch(m_window, ' ');

    }

    //bottom
    wmove(m_window, 1 * TABLE_H - 1, 0);
    waddch(m_window, ' ');
    waddch(m_window, ACS_LLCORNER);
    for (uint32_t i = 1; i < TABLE_W - 1; ++i) {
        waddch(m_window, ACS_HLINE);
        waddch(m_window, ACS_HLINE);
        waddch(m_window, ACS_BTEE);
    }
    waddch(m_window, ACS_HLINE);
    waddch(m_window, ACS_HLINE);
    waddch(m_window, ACS_LRCORNER);
    waddch(m_window, ' ');

    wattroff(m_window, COLOR_PAIR(1));
}