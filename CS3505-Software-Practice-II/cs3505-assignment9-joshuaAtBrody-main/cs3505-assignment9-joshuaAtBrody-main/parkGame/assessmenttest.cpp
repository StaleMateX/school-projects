#include "assessmenttest.h"
#include "ui_assessmenttest.h"

AssessmentTest::AssessmentTest(Model* model, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AssessmentTest)
{
    ui->setupUi(this);

    connect(model->getAnimation(), &AnimationWidget::newFrame, ui->animationScreen, &AnimationScreen::newFrame);
}

AssessmentTest::~AssessmentTest()
{
    delete ui;
}

// Change text
void AssessmentTest::recieveScreen(QString option1Text, QString option2Text, QString contentText, QString title){
    ui->Option1Button->setText(option1Text);
    ui->Option2Button->setText(option2Text);
    ui->ContentLabel->setText(contentText);
    ui->ImageLabel->setText(title);
}


// Option 1 clicked!
void AssessmentTest::on_Option1Button_clicked()
{
    emit sendOptionClicked(0);
}


void AssessmentTest::on_Option2Button_clicked()
{
    emit sendOptionClicked(1);
}

// Goto Title Screen!
void AssessmentTest::on_GotoTitleScreenButton_clicked()
{
    emit gotoTitleScreenPlease();
}

