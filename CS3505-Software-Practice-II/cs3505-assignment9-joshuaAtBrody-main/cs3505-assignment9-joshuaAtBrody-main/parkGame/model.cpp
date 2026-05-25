#include "model.h"
#include "tutorialdata.h"
#include "assessmentdata.h"
#include "titlewindowdata.h"
#include "loserwindowdata.h"
#include "winnerwindowdata.h"

Model::Model()
{
    currentlyShown = 0;

    std::vector<QString> playerImage;
    playerImage.push_back("://PlayerEncounter.png");
    animation = AnimationWidget(playerImage);

    std::vector<QString> bearImage;
    bearImage.push_back("://BEAR.png");

    std::vector<QString> bisonImage;
    bisonImage.push_back("://Bison.png");

    std::vector<QString> trashBinImage;
    trashBinImage.push_back("://Garbage.png");

    std::vector<QString> bootsImage;
    bootsImage.push_back("://Boots.png");

    std::vector<QString> deerImage;
    deerImage.push_back("://deer1.png");
    deerImage.push_back("://deer2.png");
    deerImage.push_back("://deer3.png");
    deerImage.push_back("://deer4.png");

    std::vector<QString> lunchboxImage;
    lunchboxImage.push_back("://Lunchbox.png");



    screens = {
        new TitleWindowData(1),
        new TutorialData("Leave No Trace", "When visiting national parks, it's crucial to leave no trace behind to preserve the natural beauty and ecosystem. This means packing out all trash and human waste, leaving natural objects and wildlife undisturbed, and minimizing your impact on the environment.", "", 2),
        new TutorialData("Don’t Feed Wildlife", "Feeding wildlife in national parks can disrupt their natural behaviors, harm their health, and create dependency on human food. It's important to admire wildlife from a distance and refrain from offering them any food.", "", 3),
        new TutorialData("Don’t Disturb/Approach Wildlife", "Approaching or disturbing wildlife in national parks can stress them, disrupt their natural behaviors, and even provoke defensive reactions. It's important to observe wildlife from a safe distance and avoid disturbing them.", "", 4),

        new AssessmentData("Test One", "Pack it back into your \r\n bag to dispose of it properly later.", "Scatter it on the ground \r\n for wildlife to eat.", "You've finished a picnic in a national park and have some leftover food. What should you do with it?", trashBinImage, 7, 10),
        new AssessmentData("Test Two", "Keep a safe distance \r\n and avoid feeding them.", "Approach them and offer them \r\n some snacks from your backpack.", "You encounter a group of deer during your hike and notice they're approaching you. What should you do?", deerImage, 8, 11),
        new AssessmentData("Test Three", "Observe them quietly from \r\n a safe distance and avoid disturbing them.", "Walk towards them slowly to take \r\n a closer look and get a better picture.", "You spot a family of bison grazing in a meadow. What should you do?", bisonImage, 9, 12),

        new WinnerWindowData(5, "Correct!", "Always take and keep any times you brang on the trip with you so that it doesn't leave any impact on the park!", "Next"), // 7
        new WinnerWindowData(6, "Correct!", "Never feed any wildlife as it dangers both them and you.", "Next"), // 8
        new WinnerWindowData(13, "Correct!", "You avoided the wildlife to ensure that both them and you have a nice, relaxing day.", "Next"),// 9

        new LoserWindowData(1, "You left your trash in the park and disturbed the ecosystem. \r\n The trout population was distrubed", "You Lose!"), // 10
        new LoserWindowData(2, "You fed the deer and they are now dependent on travelers food and can't feed themselves. They die of starvation.", "You Lose!"), // 11
        new LoserWindowData(3, "The bison are alerted and charge at you. You get gravely injured.", "You Lose!"), // 12

        new TutorialData("Hike with a Buddy", "When exploring national parks, it's recommended to hike with a buddy for safety. Having a companion can provide support in case of emergencies, navigation challenges, or unexpected situations.", "", 14),
        new TutorialData("Bring Water/Food", "Bringing an adequate supply of water and food is essential when visiting national parks, especially for longer hikes or remote areas where amenities may not be available. Proper hydration and nutrition are crucial for staying healthy and energized during outdoor activities.", "", 15),
        new TutorialData("Wear Appropriate Footwear and Bring Clothes for Cold/Bad Weather", "Weather conditions in national parks can change rapidly, and terrain can be rugged and unpredictable. Wearing appropriate footwear and bringing layers of clothing for varying temperatures and weather conditions is essential for comfort, safety, and enjoyment during outdoor activities.", "", 16),

        new AssessmentData("Test Four", "Invite a friend or family member \r\n to join you for the hike and bring a \r\n GPS enabled rescue device (Garmin inReach).", "Proceed with the solo hike \r\n without informing anyone about your plans.", "You're planning a solo hike in a remote area of a national park. What should you do?", bearImage, 19, 22),
        new AssessmentData("Test Five", "Sufficient water and nutritious snacks \r\n to keep you energized throughout the hike.", "No need for water or food, \r\n as you can rely on natural sources \r\n and wildlife for sustenance.", "You're preparing for a day-long hike in a national park. What should you make sure to bring with you?", lunchboxImage, 20, 23),
        new AssessmentData("Test Six", "Pack layers of clothing and sturdy footwear \r\n suitable for various weather conditions.", "Pack only light clothing and sandals \r\n for a more enjoyable and freeing experience.", "You're planning a hike in a national park known for its unpredictable weather. What should you consider when packing your gear?", bootsImage, 21, 24),

        new WinnerWindowData(17, "Correct!", "Your friends and family helped \r\n protect you from potential dangers.", "Next"), // 19
        new WinnerWindowData(18, "Correct!", "You brought water and snacks. \r\n These helped sustain you during your hike", "Next"), // 20
        new WinnerWindowData(25, "Correct!", "You brought clothing and footwear. \r\n These helped protect you from the weather", "Next"), // 21

        new LoserWindowData(13, "Remember to bring your friends and family.", "You Lose!"), // 22
        new LoserWindowData(14, "Remember to bring water and snacks.", "You Lose!"), // 23
        new LoserWindowData(15, "Remember to bring clothing and footwear.", "You Lose!"), // 24
        new WinnerWindowData(0, "You Completed the Course!", "Congratulations on completing the course! \r\n Please remember to follow the rules! \r\n Stay safe during your trip!", "Return to Title") // 25
    };
}

Model::~Model() {
    for (Screen* screen : screens) {
        delete screen;
    }
}

void Model::prepareTutorialData()
{
    TutorialData* tutorialData = static_cast<TutorialData*>(screens[currentlyShown]);

    emit sendTutorialContent(std::tuple(tutorialData->getTitle(), tutorialData->getContent(), tutorialData->getImagePath()));
}

void Model::prepareTestScreenData()
{
    animation.start();

    AssessmentData* assessmentData = static_cast<AssessmentData*>(screens[currentlyShown]);


    emit sendTestScreenContent(std::tuple(assessmentData->getOption1Text(), assessmentData->getOption2Text(), assessmentData->getContentText(), assessmentData->getTitle()));
}

void Model::prepareLoserScreenData() {
    LoserWindowData* loserWindow = static_cast<LoserWindowData*>(screens[currentlyShown]);

    emit sendLoserWindowContent(loserWindow->getContent(), loserWindow->getTitle());
}

void Model::prepareTitleScreenData() {
    emit sendTitleContent();
}

void Model::prepareWinnerScreenData() {
    WinnerWindowData* winnerWindow = static_cast<WinnerWindowData*>(screens[currentlyShown]);

    emit sendWinnerWindowContent(winnerWindow->getTitle(), winnerWindow->getContent(), winnerWindow->getButtonText());
}

void Model::changeScreenData(int option)
{
    animation.stop();

    // First, simulate the button click
    if (TutorialData* tutorialData = dynamic_cast<TutorialData*>(screens[currentlyShown])) {
        currentlyShown = tutorialData->getButtonResult();
    }

    else if (AssessmentData* assessmentData = dynamic_cast<AssessmentData*>(screens[currentlyShown])) {
        if (option == 0) {
            currentlyShown = assessmentData->getOption1Result();
        }
        else {
            currentlyShown = assessmentData->getOption2Result();
        }
    }

    else if (TitleWindowData* titleWindow = dynamic_cast<TitleWindowData*>(screens[currentlyShown])) {
        currentlyShown = titleWindow->getButtonResult();
    }

    else if (LoserWindowData* loserWindow = dynamic_cast<LoserWindowData*>(screens[currentlyShown])) {
        currentlyShown = loserWindow->getButtonResult();
    }

    else if (WinnerWindowData* winnerWindow = dynamic_cast<WinnerWindowData*>(screens[currentlyShown])) {
        currentlyShown = winnerWindow->getButtonResult();
    }

    // Send the data to the model
    if (TutorialData* tutorialData = dynamic_cast<TutorialData*>(screens[currentlyShown])) {
        prepareTutorialData();
    }

    else if (AssessmentData* assessmentData = dynamic_cast<AssessmentData*>(screens[currentlyShown])) {
        animation.setSubject(assessmentData->getImages());
        prepareTestScreenData();
    }

    else if (TitleWindowData* titleWindow = dynamic_cast<TitleWindowData*>(screens[currentlyShown])) {
        prepareTitleScreenData();
    }

    else if (LoserWindowData* loserWindow = dynamic_cast<LoserWindowData*>(screens[currentlyShown])) {
        prepareLoserScreenData();
    }

    else if (WinnerWindowData* winnerWindow = dynamic_cast<WinnerWindowData*>(screens[currentlyShown])) {
        prepareWinnerScreenData();
    }
}

void Model::resetView(){
    currentlyShown = 0;
    prepareTitleScreenData();
}


AnimationWidget* Model::getAnimation(){
    return &animation;
}

void Model::forwardFrameToView(int x, int y){
    emit newFrame(x, y);
}

