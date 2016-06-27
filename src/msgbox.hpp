/*
 *    ________  ________  .____      __________.__
 *    \______ \ \______ \ |    |     \______   \  |  __ __   ____
 *     |    |  \ |    |  \|    |      |    |  _/  | |  |  \_/ __ \
 *     |    `   \|    `   \    |___   |    |   \  |_|  |  /\  ___/
 *    /_______  /_______  /_______ \  |______  /____/____/  \___  >
 *            \/        \/        \/         \/                 \/
 */


#ifndef GOPA2_MSGBOX_HPP_B16B00B135A93A55555555M01189998819991197253
#define GOPA2_MSGBOX_HPP_B16B00B135A93A55555555M01189998819991197253

#include <ncurses.h>
#include <string>
#include "constants.hpp"


/**
 * Abstract class that represents popup box.
 * Every popup create an infinite loop!
 * */
class CMsgBox {
public:
    CMsgBox();
    virtual ~CMsgBox();

    /** The window pops up. Infinite loop starts. */
    virtual void Popup() = 0;


protected:
    /**Draws a box - a background of the popup*/
    void DrawBox();

    /**
     * Draws a button at the coordinates x,y (relatively from left top corner of the popup
     * @param [in] x X-coordinate of the button. (left top corner)
     * @param [in] y Y-coordinate of the button. (left top corner)
     * @param [in] text Button text. I recommend to add a space at the end and the beginning of the text.
     * @param [in] active Changes the colors of the button - whether it is selected or not.
     * */
    void DrawButton(int x, int y, const std::string &text, bool active);

    /**nurses window*/
    WINDOW *m_window;
    /**Popup message*/
    std::string m_message;

};

/*############################################################################################################################*/
/**Simple popup with mesage and OK button*/
class CMsgBoxOk : public CMsgBox {
public:
    /**
     * Constructor of the popup object
     * @param [in] text The message of the popup.
     * */
    CMsgBoxOk(const std::string &text);

    /** The window pops up. Infinite loop starts. */
    virtual void Popup() override;

protected:

};

/*############################################################################################################################*/
/**Popup with message and text input limited by MSGBOX_INPUT_MAX_LEN.*/
class CMsgBoxTextInput : public CMsgBox {
public:
    /**
     * Constructor of the popup object
     * @param [in] text The message of the popup.
     * */
    CMsgBoxTextInput(const std::string &text);

    /** The window pops up. Infinite loop starts. */
    void Popup() override;

    /**
     * Get the input user wrote in.
     * @return Returns a text that user typed in, or an empty string if Popup was not called.
     * */
    const std::string &getInput();

protected:
    /**input typed in by the user*/
    std::string m_input;
};

/*############################################################################################################################*/
/**Popup with two choices*/
class CMsgBoxDblChoice : public CMsgBox {
public:
    /**
     * Constructor of the popup object
     * @param [in] message The message of the popup.
     * @param [in] ans0 First button's text
     * @param [in] ans1 Second button's text
     * */
    CMsgBoxDblChoice(const std::string &message, const std::string &ans0, const std::string &ans1);

    /** The window pops up. Infinite loop starts. */
    virtual void Popup() override;

    /**
     * Get the user's answer.
     * @return Returns 0 if first button was selected or Popup was not called, 1 if the second button was called.
     * */
    int getAnswer();

protected:
    /**Button captions*/
    std::string m_ans0, m_ans1;
    /**user's answer*/
    int m_answer;
};


#endif //GOPA2_MSGBOX_HPP_B16B00B135A93A55555555M01189998819991197253
