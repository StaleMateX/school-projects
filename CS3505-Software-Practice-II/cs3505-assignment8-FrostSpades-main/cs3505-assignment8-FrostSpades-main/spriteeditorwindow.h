/**
 * spriteeditorwindow.h
 *
 * Window representing the sprite editor.
 * Provides sprite editor functionality.
 *
 *  CS 3505 : Assignment 8 Sprite Editor
 *
 * @author Ethan Andrews
 * reviewed by Joshua Brody
*/

#ifndef SPRITEEDITORWINDOW_H
#define SPRITEEDITORWINDOW_H

#include <QWidget>
#include "drawwindow.h"
#include "spriteanimation.h"
#include "tools.h"

namespace Ui {
class SpriteEditorWindow;
}

class SpriteEditorWindow : public QWidget
{
    Q_OBJECT

public:
    ///
    /// \brief SpriteEditorWindow default constructor
    /// \param parent   inherits from QWidget
    ///
    explicit SpriteEditorWindow(QWidget *parent = nullptr);

    /* Deconstructor
     */
    ~SpriteEditorWindow();

    ///
    /// \brief getDrawWindow gets the drawWindow that is embedded within SpriteEditorWindow
    /// \return the drawWindow within SpriteEditorWindow
    ///
    DrawWindow *getDrawWindow();

    ///
    /// \brief getAnimationWindow gets the animationWindow that is embedded within SpriteEditorWindow
    /// \return the animationWindow within SpriteEditorWindow
    ///
    SpriteAnimation *getAnimationWindow();


    ///
    /// \brief reset set ALL objects in SpriteEditorWindow to their default values.
    ///
    void reset();

private:
    Ui::SpriteEditorWindow *ui;
    Tool currentTool;

public slots:
    ///
    /// \brief processClick listen to a click within draw window and send color, position, and frame to Model.
    /// \param frameID  the current frame that the draw window is on
    /// \param x    the x position of the mouse
    /// \param y    the y position of the mouse
    ///
    void processClick(int frameID, int x, int y);

    ///
    /// \brief setSpinbox construct the spinboxes for frame switching
    /// \param newMaxValue the value from the spinbox
    ///
    void setSpinbox(int newMaxValue);
signals:
    ///
    /// \brief frameSpinBoxChanged update methods about which frame user is on
    /// \param frameID  the frame the user is on
    ///
    void frameSpinBoxChanged(int frameID);

    ///
    /// \brief sendClick user clicked, send model relavent info
    /// \param tool the tool that the user is using,
    /// \param frameID  the current frame the user wants to edit
    /// \param x    the x position of the pixel at mouse position
    /// \param y    the y posiiton of the pixel at mouse position
    /// \param red  the current red rgba value
    /// \param green    the current green rgba value
    /// \param blue     the current blue rgba value
    /// \param alpha    the current alpha rgba value
    ///
    void sendClick(Tool tool, int frameID, int x, int y, int red, int green, int blue, int alpha);

    ///
    /// \brief saveButtonClicked tell model to save the sprite
    ///
    void saveButtonClicked();

    ///
    /// \brief updateDelayOfAnimation tell animationWindow to update the fps
    /// \param delay    the new fps value
    ///
    void updateDelayOfAnimation(int delay);

    ///
    /// \brief startMenuButtonClicked tell mainwindow to goto startMenuWindow
    ///
    void startMenuButtonClicked();

    ///
    /// \brief addFrame tell model to add a new frame at end of list
    ///
    void addFrame();

    ///
    /// \brief deleteCurrentFrame tell mode to delete this frame
    /// \param frameID  the current frame the user is on
    ///
    void deleteCurrentFrame(int frameID);

    ///
    /// \brief setCurrentFrame tell model to show this frame
    /// \param newFrame the frame the user wants to go to
    ///
    void setCurrentFrame(int newFrame);

    ///
    /// \brief swapFrames   tell model to switch leftFrameID with rightFrameID
    /// \param leftFrameID  the frame id set from spinbox
    /// \param rightFrameID the frame id set from another spinbox
    ///
    void swapFrames(int leftFrameID, int rightFrameID);

    ///
    /// \brief invertColors tell model to invert all colors in this frame
    /// \param frameID  the current frame displayed in draw window
    ///
    void invertColors(int frameID);

    ///
    /// \brief animationScaleChange tell animationWindow to change the scale of the animation
    /// \param newScale the value of the scale from spinbox
    ///
    void animationScaleChange(int newScale);

    ///
    /// \brief mirrorHorizontal tell model to flip the frame horizontally
    /// \param frameID  the current frame
    ///
    void mirrorHorizontal(int frameID);

    ///
    /// \brief mirrorVertical   tell model to flip the frame vertically
    /// \param frameID  the current frame
    ///
    void mirrorVertical(int frameID);
private slots:
    ///
    /// \brief on_alphaSpinbox_valueChanged update the color display
    ///
    void on_alphaSpinbox_valueChanged();

    ///
    /// \brief on_greenSpinbox_valueChanged update the color display
    ///
    void on_greenSpinbox_valueChanged();

    ///
    /// \brief on_blueSpinbox_valueChanged  update the color display
    ///
    void on_blueSpinbox_valueChanged();

    ///
    /// \brief on_redSpinbox_valueChanged   update the color display
    ///
    void on_redSpinbox_valueChanged();

    ///
    /// \brief updatePreviewColor   update the color display
    ///
    void updatePreviewColor();

    ///
    /// \brief on_PenTool_clicked   tool is changed
    ///
    void on_PenTool_clicked();

    ///
    /// \brief on_EraseTool_clicked tool is changed
    ///
    void on_EraseTool_clicked();

    ///
    /// \brief on_saveButton_clicked    tell model to save
    ///
    void on_saveButton_clicked();

    ///
    /// \brief on_startMenuButton_clicked   tell mainWindow to change windows
    ///
    void on_startMenuButton_clicked();

    ///
    /// \brief on_addFrameButton_clicked    emit signal
    ///
    void on_addFrameButton_clicked();

    ///
    /// \brief on_frameSwitcher_valueChanged    emit signal
    /// \param newFrame the current frame
    ///
    void on_frameSwitcher_valueChanged(int newFrame);

    ///
    /// \brief on_deleteCurrentFrame_clicked    emit signal
    ///
    void on_deleteCurrentFrame_clicked();

    ///
    /// \brief on_horizontalSlider_sliderMoved update delay of animation
    /// \param position the value of the new delay variable
    ///
    void on_horizontalSlider_sliderMoved(int position);

    ///
    /// \brief on_switchFramesButton_clicked    emit signal
    ///
    void on_switchFramesButton_clicked();

    ///
    /// \brief on_FillTool_clicked  tool is changed
    ///
    void on_FillTool_clicked();

    ///
    /// \brief on_invertCurrentFrameColors_clicked emit signal
    ///
    void on_invertCurrentFrameColors_clicked();

    ///
    /// \brief on_animationScale_sliderMoved    tell animation value of slider
    /// \param position the value of slider, new scale value
    ///
    void on_animationScale_sliderMoved(int position);

    ///
    /// \brief on_mirrorHorizontalButton_clicked    emit signal to animation
    ///
    void on_mirrorHorizontalButton_clicked();

    ///
    /// \brief on_mirrorVerticalButton_clicked  emit signal to animation
    ///
    void on_mirrorVerticalButton_clicked();
};

#endif // SPRITEEDITORWINDOW_H
