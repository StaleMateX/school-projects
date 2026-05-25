/**
 * The data for the tutorial window.
 *
 * @author Ethan Andrews
 */
#ifndef TUTORIALDATA_H
#define TUTORIALDATA_H

#include "screen.h"

class TutorialData : public Screen
{
public:
    /**
     * @brief TutorialData default constructor for tutorial data
     */
    TutorialData();
    /**
     * @brief TutorialData construct a tutorial data screen using anopther one
     * @param other
     */
    TutorialData(Screen*& other);
    /**
     * @brief TutorialData main constructor for tutorial data class
     * @param title
     * @param content
     * @param imagePath
     * @param buttonResult
     */
    TutorialData(QString title, QString content, QString imagePath, int buttonResult);
    /**
     * Destructor for tutorial data
     */
    ~TutorialData();

    /**
     * @brief getTitle
     * Returns the title
     * @return the title
     */
    QString getTitle();

    /**
     * @brief getContent
     * Returns the content
     * @return the content.
     */
    QString getContent();

    /**
     * @brief getImagePath
     * Returns the image path
     * @return the image path
     */
    QString getImagePath();

    /**
     * @brief getButtonResult
     * Returns the button result
     * @return the button result
     */
    int getButtonResult();

private:
    // the title
    QString title;
    // the content
    QString content;
    // the image path
    QString imagePath;
    // the button result
    int buttonResult;
};

#endif // TUTORIALDATA_H
