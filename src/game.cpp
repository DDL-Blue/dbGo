/*
 *    ________  ________  .____      __________.__
 *    \______ \ \______ \ |    |     \______   \  |  __ __   ____
 *     |    |  \ |    |  \|    |      |    |  _/  | |  |  \_/ __ \
 *     |    `   \|    `   \    |___   |    |   \  |_|  |  /\  ___/
 *    /_______  /_______  /_______ \  |______  /____/____/  \___  >
 *            \/        \/        \/         \/                 \/
 */

#include <fstream>
#include <sstream>
#include "game.hpp"
#include "msgbox.hpp"


/*############################################################################################################################*/
CGame::CGame(CPlayer *player1, CPlayer *player2) : m_focus(GAME_FOCUS_GAME), m_passed(false),
                                                   m_guiSelected(GAME_GUI_SEL_PASS), m_phase(PHASE_START) {
    m_windowTop = newwin (0, 0, 0, 0);
    m_windowLeft = newwin (0, 0, 0, 0);
    m_windowRight = newwin (0, 0, 0, 0);
    m_windowBottom = newwin (0, 0, 0, 0);

    m_score [PLAYER_1] = m_score [PLAYER_2] = 0;

    player1->Attach (*this, PLAYER_1);
    player2->Attach (*this, PLAYER_2);

    m_table = new CTable();
    m_player [0] = player1;
    m_player [1] = player2;

    m_player [0] ->OnMove();
    m_player [1] ->NotOnMove();

}

/*----------------------------------------------------------------------*/
CGame::~CGame() {
    delete m_player[0];
    delete m_player[1];

    delete m_table;

    delwin (m_windowTop);
    delwin (m_windowLeft);
    delwin (m_windowRight);
    delwin (m_windowBottom);
}

/*----------------------------------------------------------------------*/
void CGame::DrawRebuild(uint32_t x, uint32_t y) {
    m_table->DrawRebuild (x+1, y+3);

    delwin (m_windowTop);
    delwin (m_windowLeft);
    delwin (m_windowRight);
    delwin (m_windowBottom);

    m_windowTop = newwin  ( 3, GAME_GFX_W, y, x );
    m_windowLeft = newwin ( TABLE_H, 1, y+3, x );
    m_windowRight = newwin ( TABLE_H, 1, y+3, x+GAME_GFX_W-1 );
    m_windowBottom = newwin ( 4, GAME_GFX_W, y+GAME_GFX_H-4, x );
}

/*----------------------------------------------------------------------*/
void CGame::Draw() {
    m_table->Draw();
    DrawGUI();
}

/*----------------------------------------------------------------------*/
void CGame::DrawGUI() {
    //sides
    wattron (m_windowLeft,COLOR_PAIR(5));
    wattron (m_windowRight,COLOR_PAIR(5));
    for (uint32_t i = 0; i < TABLE_H; ++i) {

        wmove (m_windowLeft, i, 0);
        waddch (m_windowLeft, ACS_VLINE | A_BOLD);

        wmove (m_windowRight, i, 0);
        waddch (m_windowRight, ACS_VLINE);

    }
    wattroff (m_windowLeft,COLOR_PAIR(5) | A_BOLD);
    wattroff (m_windowRight,COLOR_PAIR(5) | A_BOLD);


    //bottom
    wattron (m_windowBottom,COLOR_PAIR(5));
    for (int j = 0; j < 3; ++j) {
        wmove (m_windowBottom, j, 0);
        waddch (m_windowBottom, ACS_VLINE | A_BOLD);

        wmove (m_windowBottom, j, GAME_GFX_W-1);
        waddch (m_windowBottom, ACS_VLINE);
    }
    wmove (m_windowBottom, 3, 0);
    waddch (m_windowBottom, ACS_LLCORNER | A_BOLD);
    wmove (m_windowBottom, 3,GAME_GFX_W-1);
    waddch (m_windowBottom, ACS_LRCORNER);


    for (uint32_t k = 1; k < GAME_GFX_W - 1; ++k) {
        wmove (m_windowBottom, 3, k);
        waddch (m_windowBottom, ACS_HLINE);
    }

    wattroff (m_windowBottom,COLOR_PAIR(5) | A_BOLD);


    //top
    wattron (m_windowTop,COLOR_PAIR(5));
    mvwaddch(m_windowTop, 0, 0, ACS_ULCORNER | A_BOLD );
    mvwaddch(m_windowTop, 0, GAME_GFX_W-1, ACS_URCORNER );

    for (uint32_t l = 1; l < GAME_GFX_W - 1; ++l) {
        mvwaddch(m_windowTop, 0, l, ACS_HLINE | A_BOLD );
    }

    for (uint32_t m = 1; m < 3; ++m) {
        mvwaddch(m_windowTop, m, 0, ACS_VLINE | A_BOLD );
        mvwaddch(m_windowTop, m, GAME_GFX_W-1, ACS_VLINE );
    }

    wattroff (m_windowTop,COLOR_PAIR(5));

    //score
    wattron (m_windowTop,COLOR_PAIR(5));
    mvwprintw (m_windowTop, 1, (GAME_GFX_W-5)/2, "SCORE" );
    wattroff (m_windowTop,COLOR_PAIR(5));

    wattron (m_windowTop,COLOR_PAIR(6));
    mvwprintw (m_windowTop, 1, (GAME_GFX_W-5)/2-6, "%5d", m_score[PLAYER_1] );
    wattroff (m_windowTop,COLOR_PAIR(6));

    wattron (m_windowTop,COLOR_PAIR(7));
    mvwprintw (m_windowTop, 1, (GAME_GFX_W-5)/2+6, "%d", m_score[PLAYER_2] );
    wattroff (m_windowTop,COLOR_PAIR(7));

    //Move
    wmove(m_windowTop, 2, (GAME_GFX_W-8)/2);
    waddch(m_windowTop, ACS_CKBOARD | A_BOLD | COLOR_PAIR (6 + (m_phase == PHASE_P2_MOVE)));
    wattron (m_windowTop,COLOR_PAIR(5));
    mvwprintw (m_windowTop, 2, (GAME_GFX_W-8)/2+1, "'s MOVE");
    wattroff (m_windowTop,COLOR_PAIR(5));

    DrawGUIButtons();

    wrefresh (m_windowTop);
    wrefresh (m_windowBottom);
    wrefresh (m_windowLeft);
    wrefresh (m_windowRight);
}

/*----------------------------------------------------------------------*/
void CGame::DrawGUIButtons() {

    //PASS
    wattron ( m_windowBottom, COLOR_PAIR ( 8+ (m_guiSelected==GAME_GUI_SEL_PASS && m_focus==GAME_FOCUS_GUI) ) );
    mvwprintw( m_windowBottom, 1, 16 ,"PASS");
    wattroff( m_windowBottom, COLOR_PAIR ( 8+ (m_guiSelected==GAME_GUI_SEL_PASS && m_focus==GAME_FOCUS_GUI) ) );

    //SAVE
    wattron ( m_windowBottom, COLOR_PAIR ( 8+ (m_guiSelected==GAME_GUI_SEL_SAVE && m_focus==GAME_FOCUS_GUI) ) );
    mvwprintw( m_windowBottom, 1, 26 ,"SAVE");
    wattroff( m_windowBottom, COLOR_PAIR ( 8+ (m_guiSelected==GAME_GUI_SEL_SAVE && m_focus==GAME_FOCUS_GUI) ) );

    //SURRENDER
    wattron ( m_windowBottom, COLOR_PAIR ( 8+ (m_guiSelected==GAME_GUI_SEL_SURRENDER && m_focus==GAME_FOCUS_GUI) ) );
    mvwprintw( m_windowBottom, 1, 36 ,"SURRENDER");
    wattroff( m_windowBottom, COLOR_PAIR ( 8+ (m_guiSelected==GAME_GUI_SEL_SURRENDER && m_focus==GAME_FOCUS_GUI) ) );

}

/*----------------------------------------------------------------------*/
void CGame::SendEventToPlayers(CEvent *event) {
    m_player[0]->SendEvent ( event->Clone() );
    m_player[1]->SendEvent ( event );
}
/*----------------------------------------------------------------------*/
bool CGame::Move(Player player, const CMove &move) {
    uint32_t score = 0;
    if (m_table->Move (player, move, score)){
        if ((m_phase == PHASE_P1_MOVE && player != PLAYER_1) ||
            (m_phase == PHASE_P2_MOVE && player != PLAYER_2)) { return false; } //not your move, moron!

        m_score [player] += score;

        m_passed = false;
        TogglePlayer();

        CEvent *ev = new CEvent();
        ev->WriteData (EV_PLAYER_MOVE);
        ev->WriteData (player);
        ev->WriteData (move);
        SendEventToPlayers (ev);

        return true;
    }else{

        return false;
    }
}
/*----------------------------------------------------------------------*/
void CGame::Pass(Player player) {
    if ( (m_phase==PHASE_P1_MOVE && player!=PLAYER_1) || (m_phase==PHASE_P2_MOVE && player!=PLAYER_2) ) {return;}

    if (m_passed) {
        m_phase = PHASE_END;
        CEvent *ev = new CEvent();
        ev->WriteData(EV_GAME_END);
        SendEventToPlayers (ev);
    }else{
        CEvent *ev = new CEvent();
        ev->WriteData (EV_PLAYER_PASS);
        ev->WriteData (player);

        SendEventToPlayers (ev);
        m_passed = true;
        TogglePlayer();
    }


}

/*----------------------------------------------------------------------*/
void CGame::GameEnd() {
    std::stringstream resultText;

    uint32_t tmp;
    Player player;

    for (uint32_t i = 0; i < TABLE_W; ++i) {
        for (uint32_t j = 0; j < TABLE_H; ++j) {
            player = m_table->DetectTerrytoryDestructive(CMove(i, j), tmp);
            if (player != PLAYER_NONE) {
                m_score[player] += tmp;
            }
        }
    }

    resultText << "GAME END\n\n    Results:\n      Player 1: " << m_score[PLAYER_1] << "b\n      Player 2: " <<
    m_score[PLAYER_2] << "b + 6.5b" << std::endl;

    CMsgBoxOk box(resultText.str().c_str());

    box.Popup();

    return;
}

/*----------------------------------------------------------------------*/
void CGame::TogglePlayer() {
    if (m_phase==PHASE_P1_MOVE) {
        m_phase = PHASE_P2_MOVE;
        m_player[0]->NotOnMove();
        m_player[1]->OnMove();
        m_table->CursorSetPlayer (PLAYER_2);
    }else{
        if (m_phase==PHASE_P2_MOVE) {
            m_phase = PHASE_P1_MOVE;
            m_player[0]->OnMove();
            m_player[1]->NotOnMove();
            m_table->CursorSetPlayer (PLAYER_1);
        }
    }
}

/*----------------------------------------------------------------------*/
bool CGame::Tick() {
    m_player[0]->Tick();
    m_player[1]->Tick();


    CMsgBoxDblChoice boxPass("Do you really want to pass?", " no ", " yes ");
    CMsgBoxTextInput boxSave("Type the name of new save:");
    CMsgBoxDblChoice boxSurrender("Do you really want to surrender?", " no ", " yes ");
    std::string saveFolder = SAVE_FOLDER_NAME;

    if (m_focus==GAME_FOCUS_GUI) {
        int chr = getch();
        switch (chr){

            case K_LEFT:
                switch (m_guiSelected){ //stupid enums -_-
                    case GAME_GUI_SEL_PASS: m_guiSelected= GAME_GUI_SEL_SURRENDER; break;
                    case GAME_GUI_SEL_SAVE: m_guiSelected= GAME_GUI_SEL_PASS; break;
                    case GAME_GUI_SEL_SURRENDER: m_guiSelected= GAME_GUI_SEL_SAVE; break;
                    default: break;
                }
                break;

            case K_RIGHT:
                switch (m_guiSelected) {
                    case GAME_GUI_SEL_PASS:
                        m_guiSelected = GAME_GUI_SEL_SAVE;
                        break;
                    case GAME_GUI_SEL_SAVE:
                        m_guiSelected = GAME_GUI_SEL_SURRENDER;
                        break;
                    case GAME_GUI_SEL_SURRENDER:
                        m_guiSelected = GAME_GUI_SEL_PASS;
                        break;
                    default:
                        break;
                }
                break;

            case K_SWITCH:
                m_focus = GAME_FOCUS_GAME;
                break;

            case K_SELECT:
                switch (m_guiSelected) {
                    case GAME_GUI_SEL_PASS:
                        if (dynamic_cast<CPlayerLocal *>(m_player[PhaseToPlayer(m_phase)] )) {
                            boxPass.Popup();
                            if (boxPass.getAnswer() == 1) {
                                Pass(PhaseToPlayer(m_phase));
                            }
                        }
                        break;

                    case GAME_GUI_SEL_SAVE:
                        boxSave.Popup();
                        GameSave(saveFolder + boxSave.getInput());
                        getchar();
                        break;

                    case GAME_GUI_SEL_SURRENDER:
                        boxSurrender.Popup();
                        if (boxSurrender.getAnswer() == 0) { break; }
                        if (dynamic_cast<CPlayerLocal *>(m_player[PhaseToPlayer(m_phase)] )) {
                            Surrender(PhaseToPlayer(m_phase));
                        } else {
                            Surrender(getOtherPlayer(PhaseToPlayer(m_phase)));
                        }
                        break;
                    default:
                        break;
                }
                break;
            default: break;
        }
    }

    switch (m_phase){
        case PHASE_START:
            m_phase = PHASE_P1_MOVE;
            break;

        case PHASE_P1_MOVE:
            break;

        case PHASE_P2_MOVE:

            break;

        case PHASE_END:
            GameEnd();
            return false;

        default: throw ("CGame::Tick::unknown phase.");
    }
    return true;
}

/*----------------------------------------------------------------------*/
void CGame::SetCursor(uint32_t x, uint32_t y) {
    m_table->SetCursor (x, y);
}

/*----------------------------------------------------------------------*/
void CGame::ShowCursor() {
    m_table->ShowCursor();
}

/*----------------------------------------------------------------------*/
void CGame::HideCursor() {
    m_table->HideCursor();
}

/*----------------------------------------------------------------------*/
void CGame::Surrender(Player player) {

    CEvent *ev = new CEvent();
    ev->WriteData(EV_PLAYER_SURRENDER);
    ev->WriteData(player);
    m_player[0]->SendEvent ( ev->Clone() );
    m_player[1]->SendEvent ( ev );

    m_phase = PHASE_END;
}

/*----------------------------------------------------------------------*/
void CGame::SendEvent(CEvent *event) {
    event->ResetCursor();
    EventType type=EV_NONE;
    event->ReadData(type);
    Player player = PLAYER_NONE;
    CMove mov;

    switch (type){
        case EV_PLAYER_MOVE:

            event->ReadData (player);
            event->ReadData (mov);
            Move (player, mov);

            break;

        case EV_NONE:;
            break;

        case EV_PLAYER_PASS:
            event->ReadData (player);
            Pass (player);

            break;

        case EV_PLAYER_SURRENDER:
            event->ReadData (player);
            Surrender (player);
            break;

        case EV_GAME_END:
            m_phase = PHASE_END;
            break;

        default:
            throw ("CGame::SendEvent::UnknownEvent");
    }

    delete event;
    return;
}

/*----------------------------------------------------------------------*/
bool CGame::GameSave(const std::string &name) {

    if (name.size() == 0) {
        CMsgBoxOk box("ERROR: Empty name.");
        box.Popup();
        return false;
    }


    std::ofstream file;

    file.open(name.c_str(), std::ostream::out);

    if (!file.is_open()) {
        CMsgBoxOk box("ERROR: Could not save the game.\n Failed to open the file.");
        box.Popup();
        return false;
    }

    CEvent save(true); //bend CEvent for this use

    char verify[] = "Pikachu";
    for (uint32_t l = 0; l < sizeof(verify); ++l) {
        save.WriteData(verify[l]);
    }

    save.WriteData(m_phase);
    save.WriteData(m_score[PLAYER_1]);
    save.WriteData(m_score[PLAYER_2]);
    save.WriteData(m_passed);

    Table tbl = m_table->getTable();

    for (uint32_t i = 0; i < TABLE_W; ++i) {
        for (uint32_t j = 0; j < TABLE_H; ++j) {
            save.WriteData(tbl[i][j]);
        }
    }

    int len = save.m_data.size();

    save.ResetCursor();
    char buffer[] = {0};
    for (int k = 0; k < len; ++k) {
        if (!save.ReadData(buffer[0])) { throw "Game Load data missing"; };
        file.write(buffer, 1);
    }

    file.close();

    return true;


}

/*----------------------------------------------------------------------*/

bool CGame::GameLoad(const std::string &name) {

    if (name.size() == 0) {
        CMsgBoxOk box("ERROR: Empty name.");
        box.Popup();
        return false;
    }

    std::ifstream file;

    file.open(name.c_str(), std::ostream::in);

    if (!file.is_open()) {
        CMsgBoxOk box("ERROR: Can not open the file.");
        box.Popup();
        return false;
    }


    CEvent save(true); //bend CEvent for this use


    char buffer;
    while (file.good()) {
        file.read(&buffer, 1);
        if (!file.good()) { break; }
        save.WriteData(buffer);
    }


    save.ResetCursor();
    char verify[] = "Pikachu";
    //verify the file
    char tmp=0;
    for (uint32_t i = 0; i < sizeof(verify); ++i) {
        save.ReadData(tmp);

        if (tmp != verify[i]) {
            CMsgBoxOk box("ERROR: This is not a valid\ngame save file.");
            box.Popup();
            return false;
        }
    }


    save.ReadData(m_phase);
    save.ReadData(m_score[PLAYER_1]);
    save.ReadData(m_score[PLAYER_2]);
    save.ReadData(m_passed);

    Table tbl = m_table->getTable();

    for (uint32_t i = 0; i < TABLE_W; ++i) {
        for (uint32_t j = 0; j < TABLE_H; ++j) {
            save.ReadData(tbl[i][j]);
        }
    }

    m_table->setTable(tbl);

    file.close();

    return true;
}

/*----------------------------------------------------------------------*/
const CTable& CGame::getTable() const {
    return *m_table;
}

/*############################################################################################################################*/
CEvent::CEvent(bool unlimited) : m_readCursor(0), m_unlimited(unlimited) {
}

/*----------------------------------------------------------------------*/
CEvent* CEvent::Clone() {
    CEvent * rtrn = new CEvent();
    rtrn->m_data = m_data;
    return rtrn;
}

/*----------------------------------------------------------------------*/
void CEvent::ResetCursor() { m_readCursor = 0; }
