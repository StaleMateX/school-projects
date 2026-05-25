/**
 * Central view and control class that
 * routes each signal from the view.
 *
 * @author Ethan Andrews
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "loadfilewindow.h"
#include "model.h"
#include "newfilewindow.h"
#include "spriteeditorwindow.h"
#include "startupwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    /**
     * @brief openNewFileWindow
     *
     * Opens the new file dialog box.
     */
    void openNewFileWindow();

    /**
     * @brief openLoadFileWindow
     *
     * Opens the load file dialog box.
     */
    void openLoadFileWindow();

    /**
     * @brief onNewFileCancel
     *
     * Closes and resets the new
     * file dialog box.
     */
    void onNewFileCancel();

    /**
     * @brief showError
     * Shows an error.
     * @param errorMessage error message to show
     */
    void showError(QString errorMessage);

    /**
     * @brief onNewFileSubmit
     *
     * Handles the creation of a new file.
     *
     * @param name the name of the project
     * @param size the size of the canvas
     * @param path the file path
     */
    void onNewFileSubmit(QString name, int size, QString path);

    /**
     * @brief onLoadFileCancel
     *
     * Closes and resets the load file
     * dialog box.
     */
    void onLoadFileCancel();

    /**
     * @brief onLoadFileSubmit
     *
     * Loads a project from the
     * provided file path.
     *
     * @param path
     */
    void onLoadFileSubmit(QString path);

    // tells the view to return to start page
    void returnToStartPage();

private:
    Ui::MainWindow *ui;
    QStackedWidget *allPages;
    LoadFileWindow *loadFileWindow;
    NewFileWindow *newFileWindow;
    SpriteEditorWindow *spriteEditorWindow;
    StartupWindow *startupWindow;
    Model *model;

    /**
     * @brief openStartupWindow
     * Opens the startup window.
     */
    void openStartupWindow();

    /**
     * @brief openSpriteEditorWindow
     * Opens the sprite editor window.
     */
    void openSpriteEditorWindow();

    /**
     * @brief setConnections
     * Sets all the connections.
     */
    void setConnections();

    /**
     * @brief setGlobalPalette
     * Sets the color palette
     * for each of the elements.
     */
    void setGlobalPalette();

    void setModelConnections();

    void setSpriteEditorConnections();

signals:
    void addFrame();
    void deleteFrame(int id);
    void drawOnFrame(int id, int x, int y, int a, int r, int g, int b);
    void getData();
    void reorderFrames(int frameOneID, int frameTwoID);
    void saveData();

protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
