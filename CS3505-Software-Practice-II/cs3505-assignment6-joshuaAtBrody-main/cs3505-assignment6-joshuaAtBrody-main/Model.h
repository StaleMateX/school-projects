#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QVector>
#include <QTimer>

class Model : public QObject
{
    Q_OBJECT

public:
    explicit Model(QObject *parent = nullptr);

public slots:
    ///
    /// \brief startGame    when the start button is clicked, start the game.
    ///
    void startGame();

    ///
    /// \brief buttonRedClicked    When button is clicked, compare with sequence if player hit correct button in order.
    /// Otherwise restart the game
    ///
    void buttonRedClicked();

    ///
    /// \brief buttonBlueClicked    When button is clicked, compare with sequence if player hit correct button in order.
    /// Otherwise restart the game
    ///
    void buttonBlueClicked();

    ///
    /// \brief flashRedButton tell view to highlight the red button
    ///
    void flashRedButton();

    ///
    /// \brief flashBlueButton signal view to highlight the blue button
    ///
    void flashBlueButton();

    ///
    /// \brief endFlashRedButton signal view to stop highlighting the red button
    ///
    void endFlashRedButton();

    ///
    /// \brief endFlashBlueButton signal view to stop highlight the blue button
    ///
    void endFlashBlueButton();

    ///
    /// \brief pauseToLookAt100Percent a slot for a pause timer before reseting the progress bar so the player can look at it
    ///
    void pauseToLookAt100Percent();

    ///
    /// \brief pauseToComputerTurn a slot for a pause timer before calling computer's turn to give time for player to relax (get ready)
    ///
    void pauseToComputerTurn();

    ///
    /// \brief pauseToFlashNextSequence a slot for a pause timer to delay computer's next flash sequence
    ///
    void pauseToFlashNextSequence();

    ///
    /// \brief restartGame a helper slot for restarting the game after player loses
    ///
    void restartGame();


signals:
    ///
    /// \brief flashButton  flash the given button in the view.
    /// \param button   which button to flash
    ///
    void flashButton(int button);

    ///
    /// \brief playerFailed game end view, reset sequence and variables.
    ///
    void playerFailed(bool failed);

    ///
    /// \brief updateProgress   calculate the progress the player has made and display it in view.
    /// \param progress the new progress value
    ///
    void updateProgress(double progress);

    ///
    /// \brief turnStartButton  disable start button once start button has been clicked
    /// \param abled    enable/disable true/false
    ///
    void turnStartButton(bool abled);



    ///
    /// \brief switchAblityOfColorButtons   turn off or on the buttons when it is the computers turn teehee
    /// \param abled    enable/disable true/false
    ///
    void switchAbilityOfColorButtons(bool abled);

    ///
    /// \brief viewRedFlash tell view to highlight (true) and unhighlight (false) the red button
    /// \param flashing true when we want to highlight, false when we want to unhighlight
    ///
    void viewRedFlash(bool flashing);

    ///
    /// \brief viewBlueFlash tell view to highlight (true) and unhighlight (false) the blue button
    /// \param flashing true when we want to highlight, false when we want to unhighlight
    ///
    void viewBlueFlash(bool flashing);


private:
    ///
    /// \brief sequence     a vector list to hold either 0s (red) and 1s (blue)
    ///
    std::vector<int> sequence;

    ///
    /// \brief timer    useful for singleShot(...) for pausing and schedualling events
    ///
    QTimer timer;

    ///
    /// \brief currentPlayerIndexInSequence holds an int on the players correct progress (index) of the sequence
    ///
    int currentPlayerIndexInSequence;

    ///
    /// \brief currentComputerIndexInSequence holds an int of the computer's turn's progress (index) through the sequence
    ///
    int currentComputerIndexInSequence;

    ///
    /// \brief flashDelay int in milliseconds of how long the button will flash for
    ///
    int flashDelay;

    ///
    /// \brief waitDelay    int in milliseconds of how long the computer will wait until flashing the next button sequence
    ///
    int waitDelay;

    ///
    /// \brief computerTurn a method that will run computer's turn recursively
    ///
    void computerTurn();

    ///
    /// \brief flashButtonsInSequence recursive helper method for computerTurn that will flash the buttons from the sequence
    /// \param index    which button in that part of the sequence to flash
    ///
    void flashButtonsInSequence(int index);

    ///
    /// \brief reset the game, progress, timers, etc.
    ///
    void reset();



};


#endif // MODEL_H
