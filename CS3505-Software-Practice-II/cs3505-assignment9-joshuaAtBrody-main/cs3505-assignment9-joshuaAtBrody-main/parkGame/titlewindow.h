/**
 * Representation of a tutorial screen
 */
#ifndef TITLEWINDOW_H
#define TITLEWINDOW_H

#include <QWidget>

namespace Ui {
class TitleWindow;
}

class TitleWindow : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief TitleWindow construtor for a title window
     * @param parent
     */
    explicit TitleWindow(QWidget *parent = nullptr);
    /**
     * @brief destructor for the title window
     */
    ~TitleWindow();

private slots:
    /**
     * @brief on_pushButton_clicked handler for the start button on the title window being clicked
     */
    void on_pushButton_clicked();

private:
    // pointer to the ui
    Ui::TitleWindow *ui;

signals:
    /**
     * @brief sendStartClicked signal which signifies that the start button has been clicked
     * @param option
     */
    void sendStartClicked(int option);
};

#endif // TITLEWINDOW_H
