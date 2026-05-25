/**
 * Data for the loser window.
 *
 * @author Ethan Andrews
 */
#ifndef LOSERWINDOWDATA_H
#define LOSERWINDOWDATA_H

#include "screen.h"

class LoserWindowData : public Screen
{
    Q_OBJECT
public:
    /**
     * @brief LoserWindowData default constructor
     */
    LoserWindowData();
    /**
     * @brief LoserWindowData constructor
     * @param buttonResult where the failiure screen leads to
     * @param content content for the loser window
     * @param title title of the loser window
     */
    LoserWindowData(int buttonResult, QString content, QString title);
    /**
     * @brief destructor for loser window
     */
    ~LoserWindowData();

    /**
     * @brief getButtonResult
     * @return the button result
     */
    int getButtonResult();

    /**
     * @brief getContent
     * @return the content
     */
    QString getContent();

    /**
     * @brief getTitle
     * @return the title
     */
    QString getTitle();

private:
    // The button result
    int buttonResult;
    // The content
    QString content;
    // The title
    QString title;
};

#endif // LOSERWINDOWDATA_H
