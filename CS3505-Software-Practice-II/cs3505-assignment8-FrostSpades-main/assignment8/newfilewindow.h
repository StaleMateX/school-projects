/**
 * Represents the new file dialog box.
 * Presents users with fields to create
 * a new project.
 *
 * @author Ethan Andrews
*/

#ifndef NEWFILEWINDOW_H
#define NEWFILEWINDOW_H

#include <QDialog>

namespace Ui {
class NewFileWindow;
}

class NewFileWindow : public QDialog
{
    Q_OBJECT

public:
    explicit NewFileWindow(QWidget *parent = nullptr);
    ~NewFileWindow();

    /**
     * @brief reset
     *
     * Resets the internal fields.
     */
    void reset();

private slots:
    /**
     * @brief on_cancelButton_clicked
     * Handles the cancel button clicks.
     */
    void on_cancelButton_clicked();

    /**
     * @brief on_pushButton_clicked
     * Handles the source button clicks.
     */
    void on_pushButton_clicked();

    /**
     * @brief on_pushButton_2_clicked
     * Handles the push submit clicks.
     */
    void on_pushButton_2_clicked();

private:
    Ui::NewFileWindow *ui;

signals:
    /**
     * @brief cancelButtonClicked
     * Sends signal that cancel
     * button was clicked.
     */
    void cancelButtonClicked();

    /**
     * @brief submitButtonClicked
     * Sends signal that submit
     * button was clicked with
     * required fields.
     *
     * @param name The name of the project
     * @param size The size of the canvas
     * @param path The file path
     */
    void submitButtonClicked(QString name, int size, QString path);

    /**
     * @brief error
     * Sends signal that error has
     * occurred.
     *
     * @param errorMessage
     */
    void error(QString errorMessage);
};

#endif // NEWFILEWINDOW_H
