/**
 * Window to tell the player they have won
 * @date 04/23/2024
 */
#ifndef WINNERWINDOW_H
#define WINNERWINDOW_H

#include <QWidget>

namespace Ui {
class WinnerWindow;
}

class WinnerWindow : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief WinnerWindow constructor for the winner window
     * @param parent
     */
    explicit WinnerWindow(QWidget *parent = nullptr);
    /**
     * @brief destructor for the winner window
     */
    ~WinnerWindow();

private slots:
    /**
     * @brief on_goNextButton_clicked handler for the next button click event
     */
    void on_goNextButton_clicked();

public slots:
    /**
     * @brief changeContent change the content of the winner screen
     * @param title
     * @param content
     * @param buttonText
     */
    void changeContent(QString title, QString content, QString buttonText);

signals:
    /**
     * @brief gotoFromWinnerButton tells where to go next once the button on the winner screen is clicked
     * @param option
     */
    void gotoFromWinnerButton(int option);

private:
    // pointer to ui
    Ui::WinnerWindow *ui;
};

#endif // WINNERWINDOW_H
