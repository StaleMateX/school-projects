/**
 * Class representing a tutorial window, where the player is taught information
 * before being assessed
 * @date 04/23/2024
 */
#ifndef TUTORIALWINDOW_H
#define TUTORIALWINDOW_H

#include <QWidget>

namespace Ui {
class TutorialWindow;
}

class TutorialWindow : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief TutorialWindow constructor for the tutorial window
     * @param parent
     */
    explicit TutorialWindow(QWidget *parent = nullptr);
    /**
     * @brief destructor for the tutorial window
     */
    ~TutorialWindow();

public slots:
    /**
     * @brief changeContent changes the content of the tutorial window
     * @param title
     * @param content
     * @param imagePath
     */
    void changeContent(QString title, QString content, QString imagePath);

private slots:
    /**
     * @brief on_pushButton_clicked handler for the next button being clicked
     */
    void on_pushButton_clicked();

private:
    // pointer to the ui
    Ui::TutorialWindow *ui;

signals:
    /**
     * @brief sendOptionClicked send the option that was selected on the screen
     * @param option
     */
    void sendOptionClicked(int option);
};

#endif // TUTORIALWINDOW_H
