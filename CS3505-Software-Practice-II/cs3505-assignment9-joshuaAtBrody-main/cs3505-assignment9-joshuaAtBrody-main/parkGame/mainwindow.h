/**
 * Mainwindow class reponsible for managing
 * @date 04/23/2024
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QStackedWidget>

#include "model.h"
#include "assessmenttest.h"
#include "tutorialwindow.h"
#include "titlewindow.h"
#include "loserwindow.h"
#include "winnerwindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief MainWindow constructor for the MianWindow class
     * @param parent
     */
    MainWindow(QWidget *parent = nullptr);
    /**
     * @brief destructor for MinaWindow
     */
    ~MainWindow();

public slots:
    /**
     * @brief changeTestScreen change the contents of the test screen
     * @param content
     */
    void changeTestScreen(std::tuple<QString, QString, QString, QString> content);
    /**
     * @brief changeTutorialScreen change contents of the tutorial screen
     * @param content
     */
    void changeTutorialScreen(std::tuple<QString, QString, QString> content);
    /**
     * @brief changeTitleScreen change the title screen
     */
    void changeTitleScreen();
    /**
     * @brief changeLoserScreen change the contents of the loser screen
     * @param content
     * @param title
     */
    void changeLoserScreen(QString content, QString title);
    /**
     * @brief changeWinnerScreen change the content of the winnner screen
     * @param title
     * @param content
     * @param buttonText
     */
    void changeWinnerScreen(QString title, QString content, QString buttonText);

signals:
    /**
     * @brief optionClicked User clicked an option button in a test screen
     * @param option
     */
    void optionClicked(int option);
    /**
     * @brief sendTestScreenContent send the contents of the test screen
     * @param option1Text
     * @param option2Text
     * @param contentText
     */
    void sendTestScreenContent(QString option1Text, QString option2Text, QString contentText, QString title);
    /**
     * @brief sendTutorialScreenContent send the contents of the tutorial screen
     * @param title
     * @param content
     * @param imagePath
     */
    void sendTutorialScreenContent(QString title, QString content, QString imagePath);

private:
    // pointer to the model
    Model *model;
    // pointer to the assessment test
    AssessmentTest *assessmentTest;
    // pointer to the tutorial window
    TutorialWindow *tutorialWindow;
    // pointer to the stacked widget
    QStackedWidget *switchScreenType;
    //pointer to the title window
    TitleWindow *titleWindow;
    // pointer to the loserWindow
    LoserWindow *loserWindow;
    //pointer to the winner window
    WinnerWindow *winnerWindow;
    /**
     * @brief performSetup set up all connections once constructor is done running
     */
    void performSetup();
    /**
     * @brief setGlobalPalette setting the global pallette of the view
     */
    void setGlobalPalette();
};

#endif
