/**
 * Model class for the game. Stores screen information
 * and handles logic with changing between screens.
 *
 * @authors Ethan Andrews, Joshua Brody, Vasil Vassilev
 */
#ifndef MODEL_H
#define MODEL_H

#include "animationwidget.h"
#include "screen.h"
#include <Box2D/Box2D.h>
#include <QObject>
#include <QTimer>
#include <tuple>
#include <vector>

class Model : public QObject {
        Q_OBJECT

    public:
        /**
         * @brief Model default constructor for model
         */
        Model();
        /**
         * @brief Model destructor
         */
        ~Model();

        /**
         * @brief getAnimation Getter for the animation
         */
        AnimationWidget* getAnimation();

    signals:
        /**
         * @brief sendTutorialContent
         * Sends the tutorial screen content.
         * @param content
         */
        void sendTutorialContent(std::tuple<QString, QString, QString> content);

        /**
         * @brief sendTestScreenContent
         * Sends the test screen content.
         * @param content
         */
        void sendTestScreenContent(std::tuple<QString, QString, QString, QString> content);

        /**
         * @brief sendTitleContent
         * Sends the title content.
         */
        void sendTitleContent();

        /**
         * @brief sendLoserWindowContent
         * Sends the loser window content.
         *
         * @param content- new content
         * @param title- new title
         */
        void sendLoserWindowContent(QString content, QString title);

        /**
         * @brief sendWinnerWindowContent
         * Sends the winner window content.
         *
         * @param title the new title
         * @param content the new content
         * @param buttonText the new button text
         */
        void sendWinnerWindowContent(QString title, QString content, QString buttonText);
        /**
         * @brief newFrame creating a new frame
         */
        void newFrame(int, int);

    private:
        // Stores the screens
        std::vector<Screen*> screens;
        // Stores animation data
        AnimationWidget animation;
        // Stores the index of the current screen
        int currentlyShown;
        /**
         * @brief prepareTutorialData
         * Prepares the tutorial data to send.
         */
        void prepareTutorialData();
        /**
         * @brief prepareTestScreenData
         * Prepares the test screen data to send.
         */
        void prepareTestScreenData();
        /**
         * @brief prepareTitleScreenData
         * Prepares the title screen data to send.
         */
        void prepareTitleScreenData();
        /**
         * @brief prepareLoserScreenData
         * Prepares the loser screen data to send.
         */
        void prepareLoserScreenData();
        /**
         * @brief prepareWinnerScreenData
         * Prepares the winner screen data to send.
         */
        void prepareWinnerScreenData();

    public slots:
        /**
         * @brief changeScreenData
         * Changes the screen data.
         * @param option the option of the screen
         * to change it to
         */
        void changeScreenData(int option);
        /**
         * @brief forwardFrameToView Simple helper method that forwards newFrame
         * events from animation to view.
         */
        void forwardFrameToView(int, int);
        /**
         * @brief resetView reset the view
         */
        void resetView();
};

#endif // MODEL_H
