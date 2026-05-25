/*  spriteeditorwindow.cpp
 *
 *  slots and signals for ui
 *
 */


#include "spriteeditorwindow.h"
#include "ui_spriteeditorwindow.h"
#include <iostream>

SpriteEditorWindow::SpriteEditorWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SpriteEditorWindow)
{
    ui->setupUi(this);

    currentTool = pen;
}

SpriteEditorWindow::~SpriteEditorWindow()
{
    delete ui;
}

void SpriteEditorWindow::reset() {
    ui->PenTool->setChecked(true);
    ui->EraseTool->setChecked(false);
    ui->FillTool->setChecked(false);
    ui->redSlider->setValue(255);
    ui->greenSlider->setValue(255);
    ui->blueSlider->setValue(255);
    ui->alphaSlider->setValue(255);
    ui->redSpinbox->setValue(255);
    ui->blueSpinbox->setValue(255);
    ui->greenSpinbox->setValue(255);
    ui->alphaSpinbox->setValue(255);
    updatePreviewColor();
    ui->animationScale->setValue(1);
    emit animationScaleChange(1);
    ui->horizontalSlider->setValue(2000);
    emit updateDelayOfAnimation(2000);
    ui->frameSwitcher->setValue(1);
    emit setCurrentFrame(1);
    ui->frameSwitcher->setMaximum(1);
    ui->leftSwitchSpinbox->setMaximum(1);
    ui->leftSwitchSpinbox->setValue(1);
    ui->rightSwitchSpinbox->setMaximum(1);
    ui->rightSwitchSpinbox->setValue(1);
}


DrawWindow *SpriteEditorWindow::getDrawWindow()
{
    return ui->DrawWidget;
}

SpriteAnimation *SpriteEditorWindow::getAnimationWindow()
{
    return ui->spriteAnimationWidg;
}

void SpriteEditorWindow::processClick(int frameID, int x, int y)
{
    int red = ui->redSpinbox->value();
    int green = ui->greenSpinbox->value();
    int blue = ui->blueSpinbox->value();
    int alpha = ui->alphaSpinbox->value();

    emit sendClick(currentTool, frameID, x, y, red, green, blue, alpha);
}

void SpriteEditorWindow::on_alphaSpinbox_valueChanged()
{
    updatePreviewColor();
}

void SpriteEditorWindow::on_greenSpinbox_valueChanged()
{
    updatePreviewColor();
}

void SpriteEditorWindow::on_blueSpinbox_valueChanged()
{
    updatePreviewColor();
}

void SpriteEditorWindow::on_redSpinbox_valueChanged()
{
    updatePreviewColor();
}

void SpriteEditorWindow::updatePreviewColor()
{
    QString colorChange = QString("background: rgba(%1, %2, %3, %4)")
                              .arg(ui->redSpinbox->value())
                              .arg(ui->greenSpinbox->value())
                              .arg(ui->blueSpinbox->value())
                              .arg(ui->alphaSpinbox->value());

    ui->colorPreview->setStyleSheet(colorChange);
}

void SpriteEditorWindow::on_PenTool_clicked()
{
    currentTool = pen;
}

void SpriteEditorWindow::on_EraseTool_clicked()
{
    currentTool = eraser;
}

void SpriteEditorWindow::on_FillTool_clicked()
{
    currentTool = fillTool;
}

void SpriteEditorWindow::on_saveButton_clicked()
{
    emit saveButtonClicked();
}

void SpriteEditorWindow::on_startMenuButton_clicked()
{
    emit startMenuButtonClicked();
}

void SpriteEditorWindow::on_addFrameButton_clicked()
{
    ui->frameSwitcher->setMaximum(ui->frameSwitcher->maximum() + 1);

    emit addFrame();
}

void SpriteEditorWindow::on_frameSwitcher_valueChanged(int newFrame)
{
    emit setCurrentFrame(newFrame);
}

void SpriteEditorWindow::on_deleteCurrentFrame_clicked()
{
    int currentFrame = ui->frameSwitcher->value() - 1;

    if (ui->frameSwitcher->maximum() != 1) {
        ui->frameSwitcher->setMaximum(ui->frameSwitcher->maximum() - 1);
    }

    if (currentFrame == 0) {
        emit setCurrentFrame(1);
    } else {
        ui->frameSwitcher->setValue(currentFrame);
        emit setCurrentFrame(currentFrame);
    }

    emit deleteCurrentFrame(currentFrame);
}

void SpriteEditorWindow::setSpinbox(int newMaxValue)
{
    ui->frameSwitcher->setMaximum(newMaxValue);
    ui->leftSwitchSpinbox->setMaximum(newMaxValue);
    ui->rightSwitchSpinbox->setMaximum(newMaxValue);
}

void SpriteEditorWindow::on_horizontalSlider_sliderMoved(int position)
{
    emit updateDelayOfAnimation(position);
}

void SpriteEditorWindow::on_switchFramesButton_clicked()
{
    emit swapFrames(ui->leftSwitchSpinbox->value() - 1, ui->rightSwitchSpinbox->value() - 1);
}

void SpriteEditorWindow::on_invertCurrentFrameColors_clicked()
{
    std::cout << "emmiting" << std::endl;
    emit invertColors(ui->frameSwitcher->value() - 1);
}

void SpriteEditorWindow::on_animationScale_sliderMoved(int position)
{
    emit animationScaleChange(position);
}

void SpriteEditorWindow::on_mirrorHorizontalButton_clicked()
{
    int currentFrame = ui->frameSwitcher->value() - 1;
    emit mirrorHorizontal(currentFrame);
}

void SpriteEditorWindow::on_mirrorVerticalButton_clicked()
{
    int currentFrame = ui->frameSwitcher->value() - 1;
    emit mirrorVertical(currentFrame);
}

