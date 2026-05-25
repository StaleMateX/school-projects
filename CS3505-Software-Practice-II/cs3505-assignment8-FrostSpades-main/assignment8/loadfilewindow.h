/**
 * Dialog box representing a load file window.
 * Has user input a path to load a file.
 *
 * @author Ethan Andrews
*/
#ifndef LOADFILEWINDOW_H
#define LOADFILEWINDOW_H

#include <QDialog>

namespace Ui {
class LoadFileWindow;
}

class LoadFileWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoadFileWindow(QWidget *parent = nullptr);
    ~LoadFileWindow();

    void reset();

private slots:
    /**
     * @brief on_sourceButton_clicked
     * Opens file selector to select
     * a path for the file.
     */
    void on_sourceButton_clicked();

    /**
     * @brief on_submitButton_clicked
     * Submits and checks the fields.
     */
    void on_submitButton_clicked();

    /**
     * @brief on_cancelButton_clicked
     * Cancels the dialog box
     */
    void on_cancelButton_clicked();

private:
    Ui::LoadFileWindow *ui;

signals:
    /**
     * @brief cancelButtonClicked
     * Sends signal that cancel button
     * was clicked.
     */
    void cancelButtonClicked();

    /**
     * @brief submitButtonClicked
     * Sends signal that submit
     * button was clicked with
     * required fields.
     * @param path path to file
     */
    void submitButtonClicked(QString path);

    /**
     * @brief error
     * Sends signal that error has occurred.
     * @param errorMessage the error message.
     */
    void error(QString errorMessage);
};

#endif // LOADFILEWINDOW_H
