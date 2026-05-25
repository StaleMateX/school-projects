#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QImage>
#include <QMainWindow>
#include "Model.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT


private slots:
    ///
    /// \brief flashRedButton set blue button's color to pure red or dull red
    /// \param flashing true for pure red, false for dull red
    ///
    void flashRedButton(bool flashing);

    ///
    /// \brief flashBlueButton set blue button's color to pure blue or dull blue
    /// \param flashing true for pure blue, false for dull blue
    ///
    void flashBlueButton(bool flashing);


public:
    MainWindow(Model& model, QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QImage image1;

};
#endif // MAINWINDOW_H
