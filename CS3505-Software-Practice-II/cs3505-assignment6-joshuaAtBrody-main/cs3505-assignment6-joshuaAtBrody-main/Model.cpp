
#include "Model.h"
#include <QMediaPlayer>
#include <QSoundEffect>

Model::Model(QObject *parent) : QObject(parent){

    currentPlayerIndexInSequence = 0;
    currentComputerIndexInSequence = 0;
    flashDelay = 1000; // Starting delay, will lower as game increases
    waitDelay = 500;
    // Create starter sequence
    sequence.push_back(rand()%2);
}

// Slot
void Model::startGame(){
    QSoundEffect * sound = new QSoundEffect();
    sound->setSource(QUrl("qrc:/sounds/gameStart.wav"));
    sound->setVolume(1);
    sound->play();

    emit turnStartButton(false);
    timer.singleShot(500, this, &Model::pauseToComputerTurn);

}


void Model::computerTurn(){
    // COMPUTER TURN
    currentPlayerIndexInSequence = 0;
    currentComputerIndexInSequence = 0;
    // Add a part to the sequence
    sequence.push_back(rand()%2);
    // Lower flash delay and respective timing variables (make game faster)
    flashDelay -= 50;
    waitDelay -= 20;

    // Disable buttons
    emit switchAbilityOfColorButtons(false);

    // For each part in the sequence:
    flashButtonsInSequence(currentComputerIndexInSequence);

}


void Model::flashButtonsInSequence(int indexInSequence){
    if(sequence[indexInSequence] == 0){
        // Do red button
        timer.singleShot(0, this, &Model::flashRedButton);
        timer.singleShot(flashDelay, this, &Model::endFlashRedButton);
    } else {
        // Do blue button
        timer.singleShot(0, this, &Model::flashBlueButton);
        timer.singleShot(flashDelay, this, &Model::endFlashBlueButton);
    }

    // Pause between computer's flashing
    timer.singleShot(flashDelay + waitDelay, this, &Model::pauseToFlashNextSequence);
}

//Slot
void Model::pauseToFlashNextSequence(){
    if(currentComputerIndexInSequence < (int) sequence.size() - 1){
        // Recursive
        currentComputerIndexInSequence += 1;
        flashButtonsInSequence(currentComputerIndexInSequence);
    } else {
        // Enable buttons when computer has finsihed flashing sequence.
        emit switchAbilityOfColorButtons(true);
    }
}

//Slot
void Model::flashRedButton(){
    QSoundEffect * sound = new QSoundEffect();
    sound->setSource(QUrl("qrc:/sounds/drum.wav"));
    sound->setVolume(1);
    sound->play();

    emit viewRedFlash(true);
}

//Slot
void Model::flashBlueButton(){
    QSoundEffect * sound = new QSoundEffect();
    sound->setSource(QUrl("qrc:/sounds/hihat.wav"));
    sound->setVolume(1);
    sound->play();

    emit viewBlueFlash(true);
}

//Slot
void Model::endFlashRedButton(){
    emit viewRedFlash(false);
}

//Slot
void Model::endFlashBlueButton(){
    emit viewBlueFlash(false);
}


// Slot
void Model::buttonRedClicked(){
    QSoundEffect * sound = new QSoundEffect();
    sound->setSource(QUrl("qrc:/sounds/drum.wav"));
    sound->setVolume(1);
    sound->play();

    if(sequence[currentPlayerIndexInSequence] != 0){
        // If player failed
        emit playerFailed(true);
        reset();
        return;
    } else {
        // Player didn't fail display progress and continue
        currentPlayerIndexInSequence += 1;
        emit updateProgress((100*currentPlayerIndexInSequence)/sequence.size());

        if(currentPlayerIndexInSequence == (int) sequence.size()){
            currentPlayerIndexInSequence = 0;
            timer.singleShot(700, this, &Model::pauseToLookAt100Percent);
        }
    }
}

// Slot
void Model::buttonBlueClicked(){
    QSoundEffect * sound = new QSoundEffect();
    sound->setSource(QUrl("qrc:/sounds/hihat.wav"));
    sound->setVolume(1);
    sound->play();

    if(sequence[currentPlayerIndexInSequence] != 1){
        // If player failed
        emit playerFailed(true);
        reset();
        return;
    } else {
        // Player didn't fail display progress and continue
        currentPlayerIndexInSequence += 1;
        emit updateProgress((100*currentPlayerIndexInSequence)/sequence.size());

        if(currentPlayerIndexInSequence == (int) sequence.size()){
            currentPlayerIndexInSequence = 0;
            timer.singleShot(700, this, &Model::pauseToLookAt100Percent);
        }
    }
}
//Slot
void Model::pauseToLookAt100Percent(){
    emit updateProgress(0);
    timer.singleShot(500, this, &Model::pauseToComputerTurn);
}


//Slot
void Model::pauseToComputerTurn(){
    computerTurn();
}

void Model::reset(){
    QSoundEffect * sound = new QSoundEffect();
    sound->setSource(QUrl("qrc:/sounds/GameLost.wav"));
    sound->setVolume(1);
    sound->play();

    emit switchAbilityOfColorButtons(false);
    sequence.clear();
    currentPlayerIndexInSequence = 0;
    flashDelay = 1000;
    waitDelay = 500;
    sequence.push_back(rand()%2);
    timer.singleShot(5000, this, &Model::restartGame);

}

void Model::restartGame(){
    emit playerFailed(false);
    emit updateProgress(0);
    startGame();
}


