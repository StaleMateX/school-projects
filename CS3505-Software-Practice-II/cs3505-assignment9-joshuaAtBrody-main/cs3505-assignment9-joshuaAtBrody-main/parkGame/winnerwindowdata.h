/**
 * Stores the winner window data.
 *
 * @author Ethan Andrews
 */
#ifndef WINNERWINDOWDATA_H
#define WINNERWINDOWDATA_H

#include "screen.h"

class WinnerWindowData : public Screen
{
    Q_OBJECT
public:
    /**
     * @brief WinnerWindowData default constructor for the data storage for winner window
     */
    WinnerWindowData();
    /**
     * @brief WinnerWindowData main constructor for the winner window data
     * @param buttonResult
     * @param title
     * @param content
     * @param buttonText
     */
    WinnerWindowData(int buttonResult, QString title, QString content, QString buttonText);
    /**
     * destructor for winner window data
     */
    ~WinnerWindowData();

    /**
     * @brief getTitle
     * Returns the title
     * @return the title
     */
    QString getTitle();

    /**
     * @brief getContent
     * Returns the content
     * @return the content
     */
    QString getContent();

    /**
     * @brief getButtonText
     * Returns the button text
     * @return the button text
     */
    QString getButtonText();

    /**
     * @brief getButtonResult
     * Returns the button result
     * @return the button result
     */
    int getButtonResult();

private:
    // the result of pressing the button
    int buttonResult;
    // the title
    QString title;
    // the content
    QString content;
    // the button text
    QString buttonText;
};

#endif // WINNERWINDOWDATA_H
