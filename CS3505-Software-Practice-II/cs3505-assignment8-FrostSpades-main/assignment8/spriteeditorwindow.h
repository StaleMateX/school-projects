/**
 * Window representing the sprite editor.
 * Provides sprite editor functionality.
 *
 * @author Ethan Andrews
*/

#ifndef SPRITEEDITORWINDOW_H
#define SPRITEEDITORWINDOW_H

#include <QWidget>

namespace Ui {
class SpriteEditorWindow;
}

class SpriteEditorWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SpriteEditorWindow(QWidget *parent = nullptr);
    ~SpriteEditorWindow();

private:
    Ui::SpriteEditorWindow *ui;
};

#endif // SPRITEEDITORWINDOW_H
