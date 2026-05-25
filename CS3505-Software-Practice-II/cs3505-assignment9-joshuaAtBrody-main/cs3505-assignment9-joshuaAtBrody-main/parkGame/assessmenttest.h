/**
 * Assessment test class, deals with assessing player input
 */
#ifndef ASSESSMENTTEST_H
#define ASSESSMENTTEST_H

#include <QWidget>
#include "model.h"

namespace Ui {
class AssessmentTest;
}

class AssessmentTest : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief AssessmentTest constructor
     * @param model
     * @param parent
     */
    explicit AssessmentTest(Model* model, QWidget *parent = nullptr);
    /**
     * @brief ~AssessmentTest destructor for AssessmentTest
     */
    ~AssessmentTest();

private:
    // storing a pointer to the ui
    Ui::AssessmentTest *ui;



public slots:
    // Model sent us a new screen to display! Change the text info!
    void recieveScreen(QString option1Text, QString option2Text, QString contentText, QString title);


private slots:
    // Option 1 and 2 will display the next screen!
    void on_Option1Button_clicked();
    void on_Option2Button_clicked();

    // Will display the first title/start screen!
    void on_GotoTitleScreenButton_clicked();

signals:
    // tell model about which option was clicked!
    void sendOptionClicked(int option);
    // Tell mainwindow to display the start screen!
    void gotoTitleScreenPlease();
    // Tell someone to end the program!
    void quitProgramPlease();
    // send out the test screen data
    void getTestScreenData();
};

#endif // ASSESSMENTTEST_H
