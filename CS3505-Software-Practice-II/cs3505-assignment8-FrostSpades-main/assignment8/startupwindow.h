/**
 * Window representing the startup window.
 * Allows users to create a new project
 * or load a previous project.
 *
 * @author Ethan Andrews
*/

#ifndef STARTUPWINDOW_H
#define STARTUPWINDOW_H

#include <QWidget>

namespace Ui {
class StartupWindow;
}

class StartupWindow : public QWidget
{
    Q_OBJECT

public:
    explicit StartupWindow(QWidget *parent = nullptr);
    ~StartupWindow();

private slots:
    /**
     * @brief on_newButton_clicked
     * Handles new button clicks.
     */
    void on_newButton_clicked();

    /**
     * @brief on_loadButton_clicked
     * Handles load button clicks.
     */
    void on_loadButton_clicked();

private:
    Ui::StartupWindow *ui;

signals:
    /**
     * @brief newButtonClicked
     * Sends signal that new
     * button was clicked.
     */
    void newButtonClicked();

    /**
     * @brief loadButtonClicked
     * Sends signal that load
     * button was clicked.
     */
    void loadButtonClicked();
};

#endif // STARTUPWINDOW_H
