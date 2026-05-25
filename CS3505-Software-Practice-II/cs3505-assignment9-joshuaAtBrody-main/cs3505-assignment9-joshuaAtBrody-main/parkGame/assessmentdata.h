/**
 * Data for the assessment screen.
 *
 * @author Ethan Andrews
 */
#ifndef ASSESSMENTDATA_H
#define ASSESSMENTDATA_H

#include "screen.h"

class AssessmentData : public Screen
{
    Q_OBJECT
public:
    /**
     * @brief AssessmentData default constructor for the assessment screen
     */
    AssessmentData();
    /**
     * @brief AssessmentData contrsutor that takes all neccesary data to make an assessment screen
     * @param option1Text
     * @param option2Text
     * @param contentText
     * @param imagePath
     * @param option1Result
     * @param option2Result
     * @param title
     */
    AssessmentData(QString title, QString option1Text, QString option2Text, QString contentText, std::vector<QString> imagePath, int option1Result, int option2Result);

    /**
     * @brief ~AssessmentData destructor for assessment data
     */
    ~AssessmentData();

    /**
     * @brief getOption1Text
     * @return the option 1 text
     */
    QString getOption1Text();

    /**
     * @brief getOption2Text
     * @return the option 2 text
     */
    QString getOption2Text();

    /**
     * @brief getContentText
     * @return the content text
     */
    QString getContentText();

    /**
     * @brief getOption1Result
     * @return the option 1 result
     */
    int getOption1Result();

    /**
     * @brief getOption2Result
     * @return the option 2 result
     */
    int getOption2Result();

    /**
     * @brief getImages
     * Returns the images.
     *
     * @return list of images
     */
    std::vector<QString> getImages();

    /**
     * @brief getTitle
     * Returns the title
     * @return the title
     */
    QString getTitle();

private:
    // the title
    QString title;
    // the option 1 button text
    QString option1Text;
    // the option 2 button text
    QString option2Text;
    // the content text
    QString contentText;
    // the image path
    QString imagePath;
    // the option 1 button result
    int option1Result;
    // the option 2 button result
    int option2Result;
    // list of all images
    std::vector<QString> imageList;
};

#endif // ASSESSMENTDATA_H
