/**
 * Stores the data for the title window.
 *
 * @author Ethan Andrews
 */

#ifndef TITLEWINDOWDATA_H
#define TITLEWINDOWDATA_H

#include "screen.h"

class TitleWindowData : public Screen
{
    Q_OBJECT
public:
    /**
     * @brief TitleWindowData
     * Default constructor for the title
     * window.
     */
    TitleWindowData();

    /**
     * @brief TitleWindowData
     * Constructor for title window
     * @param buttonResult
     */
    TitleWindowData(int buttonResult);
    /**
     * @brief destructor for the title window data class
     */
    ~TitleWindowData();

    /**
     * @brief getButtonResult
     * Returns the button result
     * @return the button result
     */
    int getButtonResult();

private:
    // The result of pressing the button
    int buttonResult;
};

#endif // TITLEWINDOWDATA_H
