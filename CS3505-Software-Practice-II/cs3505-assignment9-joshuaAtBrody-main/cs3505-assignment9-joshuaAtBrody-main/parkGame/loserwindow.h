/**
 * Window responsible for telling the user they made the wrong choice
 * @date 04/20/2024
 */
#ifndef LOSERWINDOW_H
#define LOSERWINDOW_H

#include <QWidget>

namespace Ui {
class LoserWindow;
}

class LoserWindow : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief LoserWindow constructor for the losing window
     * @param parent
     */
    explicit LoserWindow(QWidget *parent = nullptr);
    /**
     * @brief destructor for the loser window
     */
    ~LoserWindow();

private slots:
    /**
     * @brief on_tryAgainButton_clicked handler for try again button
     */
    void on_tryAgainButton_clicked();

public slots:
    /**
     * @brief changeContent change the content of the losing screen to these QStrings
     * @param content
     * @param title
     */
    void changeContent(QString content, QString title);

private:
    // storing poiinter to ui
    Ui::LoserWindow *ui;

signals:
    /**
     * @brief gotoFromLoserButton go to the option (int) after the losing screen
     * @param option
     */
    void gotoFromLoserButton(int option);
};

#endif // LOSERWINDOW_H
