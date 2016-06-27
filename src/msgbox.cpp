/*
 *    ________  ________  .____      __________.__
 *    \______ \ \______ \ |    |     \______   \  |  __ __   ____
 *     |    |  \ |    |  \|    |      |    |  _/  | |  |  \_/ __ \
 *     |    `   \|    `   \    |___   |    |   \  |_|  |  /\  ___/
 *    /_______  /_______  /_______ \  |______  /____/____/  \___  >
 *            \/        \/        \/         \/                 \/
 */

#include "msgbox.hpp"

CMsgBox::CMsgBox() : m_message("Void.") {
    m_window = newwin(MSGBOX_H, MSGBOX_W, (LINES - MSGBOX_H) / 2, (COLS - MSGBOX_W) / 2);
}

/*----------------------------------------------------------------------*/
CMsgBox::~CMsgBox() {

    delwin(m_window);
}

/*----------------------------------------------------------------------*/
void CMsgBox::DrawBox() {
    for (uint32_t i = 0; i < MSGBOX_H; ++i) {
        wmove(m_window, i, 0);
        wchgat(m_window, -1, A_BOLD, 10, NULL);
    }
    wattron (m_window, COLOR_PAIR(10));
    wborder(m_window, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER,
            ACS_LRCORNER);
    wattroff (m_window, COLOR_PAIR(10));
}
/*----------------------------------------------------------------------*/
void CMsgBox::DrawButton(int x, int y, const std::string &text, bool active) {
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


/*############################################################################################################################*/
CMsgBoxOk::CMsgBoxOk(const std::string &text) {
    m_message = text;
}

/*----------------------------------------------------------------------*/
void CMsgBoxOk::Popup() {

    while (getch() != K_SELECT) {

        wattron (m_window, COLOR_PAIR(10));
        mvwprintw(m_window, 2, 2, m_message.c_str());
        wattroff (m_window, COLOR_PAIR(10));

        DrawBox();

        DrawButton(MSGBOX_W / 2 - 2, MSGBOX_H - 3, " OK ", true);

        wrefresh(m_window);
    }
}

/*############################################################################################################################*/
CMsgBoxTextInput::CMsgBoxTextInput(const std::string &text) : m_input("") {
    m_message = text;
}

/*----------------------------------------------------------------------*/
void CMsgBoxTextInput::Popup() {

    int chr = -1;
    while (true) {

        noecho();

        DrawBox();
        chr = getch();

        wattron (m_window, COLOR_PAIR(10));
        mvwprintw(m_window, 2, 2, m_message.c_str());
        wattroff (m_window, COLOR_PAIR(10));

        wattron (m_window, COLOR_PAIR(11));
        mvwchgat (m_window, 7, 3, MSGBOX_W - 6, 0, 11, NULL);
        mvwprintw(m_window, 7, 3, "%s ", m_input.c_str());
        wattroff (m_window, COLOR_PAIR(11));

        DrawButton(MSGBOX_W / 2 - 2, MSGBOX_H - 3, " OK ", true);

        wrefresh(m_window);

        if (chr == K_SELECT || chr == '\n') { break; }
        if (chr < 0) { continue; }
        if ((chr == KEY_BACKSPACE || chr == 127 || chr == -17)) {
            if (m_input.size()) {
                m_input.pop_back();
            }
            continue;
        }
        if (m_input.size() >= MSGBOX_INPUT_MAX_LEN) { continue; }
        m_input += chr;


    }
}

/*----------------------------------------------------------------------*/
const std::string &CMsgBoxTextInput::getInput() {
    return m_input;
}

/*############################################################################################################################*/

CMsgBoxDblChoice::CMsgBoxDblChoice(const std::string &message, const std::string &ans0,
                                   const std::string &ans1) : m_ans0(ans0), m_ans1(ans1), m_answer(-1) {
    m_message = message;
}

/*----------------------------------------------------------------------*/

void CMsgBoxDblChoice::Popup() {
    m_answer = 0;
    int chr = -1;
    while (true) {
        chr = getch();

        if (chr == K_SELECT) { return; }
        if (chr == K_LEFT || chr == K_RIGHT) { m_answer = !m_answer; }

        DrawBox();

        wattron (m_window, COLOR_PAIR(10));
        mvwprintw(m_window, 2, 2, m_message.c_str());
        wattroff (m_window, COLOR_PAIR(10));

        DrawButton(MSGBOX_W / 4 - 2, MSGBOX_H - 3, m_ans0, m_answer == 0);

        DrawButton((MSGBOX_W / 4) * 3 - 2, MSGBOX_H - 3, m_ans1, m_answer == 1);

        wrefresh(m_window);

    }

}

/*----------------------------------------------------------------------*/
int CMsgBoxDblChoice::getAnswer() {
    return m_answer;
}






