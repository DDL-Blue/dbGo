/*
 *    ________  ________  .____      __________.__
 *    \______ \ \______ \ |    |     \______   \  |  __ __   ____
 *     |    |  \ |    |  \|    |      |    |  _/  | |  |  \_/ __ \
 *     |    `   \|    `   \    |___   |    |   \  |_|  |  /\  ___/
 *    /_______  /_______  /_______ \  |______  /____/____/  \___  >
 *            \/        \/        \/         \/                 \/
 */


#include "menu.hpp"
#include "msgbox.hpp"

CMenu::CMenu() : m_phase(MENU_HOME), m_select(0), m_address("localhost"), m_loadName("") {
    m_window = newwin(1, 1, 1, 1);
}

/*----------------------------------------------------------------------*/
CMenu::~CMenu() {
    delwin(m_window);
}

/*----------------------------------------------------------------------*/
bool CMenu::Tick() {

    CMsgBoxDblChoice msgLoadQ("Do you want to load a saved game?", " no ", " yes ");
    CMsgBoxTextInput msgLoadName("Enter save name.");
    CMsgBoxTextInput msgAddrName("Enter the host's address.");
    CMsgBoxOk msgConnErr("Connection error.");
    std::string saveFolder = SAVE_FOLDER_NAME;

    int chr = '@';
    if (m_phase != MENU_INGAME) {
        chr = getch();
        switch (chr) {
            case 'q':
                m_phase = MENU_END;
                break;
            case K_DOWN: {
                ++m_select;
            }
                break;
            case K_UP:
                if (m_select) { --m_select; }
                break;
            case K_SELECT:
                wclear(m_window);
                break;
            default:
                break;
        }
    }

    //what to do here
    switch (m_phase) {
        case MENU_INGAME:
            if (!m_game->Tick()) {
                delete m_game;
                m_phase = MENU_HOME;
                wclear(m_window);
            }
            break;

        case MENU_HOME:
            if (m_select > 1) { m_select = 1; }
            if (chr == K_SELECT) {
                switch (m_select) {
                    case 0:
                        m_phase = MENU_NEWGAME;
                        break;
                    case 1:
                        m_phase = MENU_END;
                        break;
                    default:
                        m_select = 0;
                        break;

                }
                m_select = 0;
            }
            break;

        case MENU_NEWGAME:
            if (m_select > 3) { m_select = 3; }
            if (chr == K_SELECT) {
                switch (m_select) {
                    case 0:
                        m_game = new CGame(new CPlayerLocal(), new CPlayerLocal());
                        m_game->DrawRebuild(m_winX, m_winY);

                        msgLoadQ.Popup();
                        if (msgLoadQ.getAnswer() == 1) {
                            msgLoadName.Popup();
                            m_game->GameLoad(saveFolder + msgLoadName.getInput());
                        }

                        m_phase = MENU_INGAME;
                        break;
                    case 1:
                        m_phase = MENU_NETPICK;
                        m_select = 0;
                        break;
                    case 2:
                        m_phase = MENU_AIPICK;
                        m_select = 0;
                        break;
                    case 3:
                        m_phase = MENU_HOME;
                        m_select = 0;
                    default:
                        m_select = 0;
                        break;
                }

            }
            break;

        case MENU_NETPICK:
            if (m_select > 3) { m_select = 3; }
            CConnectClient *cli;
            CConnectServer *srv;
            if (chr == K_SELECT) {
                switch (m_select) {
                    case 0:
                        srv = new CConnectServer();

                        wclear(m_window);
                        mvwprintw(m_window, GAME_GFX_H / 2, GAME_GFX_W / 2 - 3, "WAITING");
                        DrawBox();
                        wrefresh(m_window);

                        srv->CreateSocket("0.0.0.0", 2666);

                        if (srv->getStatus() != CONN_OK) {
                            msgConnErr.Popup();
                            delete srv;
                        } else {
                            CPlayerNetwork *pl = new CPlayerNetwork(srv);
                            m_game = new CGame(pl, new CPlayerLocal());
                            m_game->DrawRebuild(m_winX, m_winY);
                            m_phase = MENU_INGAME;
                        }
                        break;
                    case 1:
                        msgAddrName.Popup();
                        cli = new CConnectClient();
                        cli->CreateSocket(msgAddrName.getInput().c_str(), 2666);
                        if (cli->getStatus() != CONN_OK) {
                            msgConnErr.Popup();
                            delete cli;
                        } else {
                            CPlayerNetwork *pl = new CPlayerNetwork(cli);
                            m_game = new CGame(new CPlayerLocal(), pl);
                            m_game->DrawRebuild(m_winX, m_winY);
                            m_phase = MENU_INGAME;
                        }
                        break;

                    case 2:
                        m_select = 0;
                        m_phase = MENU_NEWGAME;
                        break;

                    default:
                        m_select = 0;
                        break;
                }

            }
            break;

        case MENU_AIPICK:
            if (m_select > 4) { m_select = 4; }
            CPlayer *pl;
            if (chr == K_SELECT) {
                switch (m_select) {
                    case 0:
                        pl = new CPlayerAI3();
                        m_game = new CGame(new CPlayerLocal(), pl);
                        m_game->DrawRebuild(m_winX, m_winY);
                        m_phase = MENU_INGAME;
                        msgLoadQ.Popup();
                        if (msgLoadQ.getAnswer() == 1) {
                            msgLoadName.Popup();
                            m_game->GameLoad(saveFolder + msgLoadName.getInput());
                        }
                        break;
                    case 1:
                        pl = new CPlayerAI2();
                        m_game = new CGame(new CPlayerLocal(), pl);
                        m_game->DrawRebuild(m_winX, m_winY);
                        m_phase = MENU_INGAME;
                        msgLoadQ.Popup();
                        if (msgLoadQ.getAnswer() == 1) {
                            msgLoadName.Popup();
                            m_game->GameLoad(saveFolder + msgLoadName.getInput());
                        }
                        break;
                    case 2:
                        pl = new CPlayerAI1();
                        m_game = new CGame(new CPlayerLocal(), pl);
                        m_game->DrawRebuild(m_winX, m_winY);
                        m_phase = MENU_INGAME;
                        msgLoadQ.Popup();
                        if (msgLoadQ.getAnswer() == 1) {
                            msgLoadName.Popup();
                            m_game->GameLoad(saveFolder + msgLoadName.getInput());
                        }
                        break;
                    case 3:
                        m_select = 0;
                        m_phase = MENU_NEWGAME;
                        break;
                    default:
                        m_select = 0;
                        break;
                }
            }

            break;

        case MENU_END:
            return false;
        default:
            throw "CMenu::Tict::unknown phase";
    }

    return true;
}

/*----------------------------------------------------------------------*/
void CMenu::DrawRebuild(uint32_t x, uint32_t y) {
    delwin(m_window);
    m_window = newwin(GAME_GFX_H, GAME_GFX_W, y, x);

    m_winX = x;
    m_winY = y;

    if (m_phase == MENU_INGAME) {
        m_game->DrawRebuild(x, y);
    }

    return;
}

/*----------------------------------------------------------------------*/
void CMenu::Draw() {

    switch (m_phase){
        case MENU_HOME:
            DrawGameName();
            DrawBox();

            DrawButton(GAME_GFX_W / 2 - 4, GAME_GFX_H / 3 + 3, " NEW GAME ", m_select == 0);
            DrawButton(GAME_GFX_W / 2 - 2, GAME_GFX_H / 3 * 2 - 3, " END ", m_select == 1);
            break;

        case MENU_NEWGAME:
            DrawGameName();
            DrawBox();

            DrawButton(GAME_GFX_W / 2 - 5, GAME_GFX_H / 3, " PvP local ", m_select == 0);
            DrawButton(GAME_GFX_W / 2 - 6, GAME_GFX_H / 3 + 3, " PvP network ", m_select == 1);
            DrawButton(GAME_GFX_W / 2 - 2, GAME_GFX_H / 3 + 6, " PvE ", m_select == 2);
            DrawButton(GAME_GFX_W / 2 - 2, GAME_GFX_H / 3 + 9, " Back ", m_select == 3);
            break;

        case MENU_NETPICK:
            DrawGameName();
            DrawBox();

            DrawButton(GAME_GFX_W / 2 - 2, GAME_GFX_H / 3, " Host ", m_select == 0);
            DrawButton(GAME_GFX_W / 2 - 3, GAME_GFX_H / 3 + 3, " Client ", m_select == 1);
            DrawButton(GAME_GFX_W / 2 - 2, GAME_GFX_H / 3 + 9, " Back ", m_select == 2);
            break;

        case MENU_AIPICK:
            DrawGameName();
            DrawBox();

            DrawButton(GAME_GFX_W / 2 - 6, GAME_GFX_H / 3, " The easiest ", m_select == 0);
            DrawButton(GAME_GFX_W / 2 - 3, GAME_GFX_H / 3 + 3, " Easier ", m_select == 1);
            DrawButton(GAME_GFX_W / 2 - 2, GAME_GFX_H / 3 + 6, " Easy ", m_select == 2);

            DrawButton(GAME_GFX_W / 2 - 2, GAME_GFX_H / 3 + 12, " Back ", m_select == 3);
            break;
        case MENU_INGAME:
            m_game->Draw();
            break;

        default: throw "CMenu::Draw::unknown phase";
    }

    wrefresh(m_window);
}

/*----------------------------------------------------------------------*/
void CMenu::DrawBox() {
    for (uint32_t i = 0; i < MSGBOX_H; ++i) {
        wmove(m_window, i, 0);
        wchgat(m_window, -1, A_BOLD, 5, NULL);
    }
    wattron (m_window, COLOR_PAIR(5));
    wborder(m_window, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER,
            ACS_LRCORNER);
    wattroff (m_window, COLOR_PAIR(5));
}

/*----------------------------------------------------------------------*/
void CMenu::DrawButton(int x, int y, const std::string &text, bool active) {
    if (active) {
        wattron (m_window, COLOR_PAIR(9));
    } else {
        wattron (m_window, COLOR_PAIR(8));
    }

    mvwprintw(m_window, y, x, text.c_str());

    if (active) {
        wattroff (m_window, COLOR_PAIR(9));
    } else {
        wattroff (m_window, COLOR_PAIR(8));
    }
}

/*----------------------------------------------------------------------*/

void CMenu::DrawGameName() {
    mvwprintw(m_window, 2, GAME_GFX_W / 2 - 1, "GO");
}