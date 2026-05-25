#include "animationwidget.h"
#include <QTimer>
#include <iostream>

AnimationWidget::AnimationWidget(std::vector<QString> playerSpritePath) :  playerSpritePath(playerSpritePath), world(new b2World(b2Vec2(0.0f, -10.0f))){
    currentSpritePlayer = 0;
    currentSpriteOther = 0;
    frameCounter = 0;
    maxFrames = 50;
}

AnimationWidget::AnimationWidget() : world(new b2World(b2Vec2(0.0f, -10.0f))) {
    currentSpritePlayer = 0;
    currentSpriteOther = 0;
    frameCounter = 0;
    maxFrames = 50;
}

AnimationWidget::~AnimationWidget() {
    delete world;
}


AnimationWidget& AnimationWidget::operator=(AnimationWidget&& other) {
    std::swap(playerSpritePath, other.playerSpritePath);
    std::swap(otherSpritePath, other.otherSpritePath);

    return *this;
}

void AnimationWidget::start() {
    continueAnimation = true;

    delete world;
    world = new b2World(b2Vec2(0.0f, -10.0f));

    // Initialize box2D animation
    //  Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);

    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    b2Body* groundBody = world->CreateBody(&groundBodyDef);

    // Define the ground box shape.
    b2PolygonShape groundBox;

    // The extents are the half-widths of the box.
    groundBox.SetAsBox(50.0f, 10.0f);

    // Add the ground fixture to the ground body.
    groundBody->CreateFixture(&groundBox, 0.0f);

    // Define the dynamic body. We set its position and call the body factory.
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 4.0f);

    body = world->CreateBody(&bodyDef);

    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);

    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;

    // Set the box density to be non-zero, so it will be dynamic.
    fixtureDef.density = 1.0f;

    // Override the default friction.
    fixtureDef.friction = 0.3f;

    fixtureDef.restitution = 0.5f;

    // Add the shape to the body.
    body->CreateFixture(&fixtureDef);

    generateNextFrame();
}

void AnimationWidget::stop() {
    continueAnimation = false;
}

void AnimationWidget::setSubject(std::vector<QString> pathToEncounteredSprite) {
    otherSpritePath = pathToEncounteredSprite;
}

void AnimationWidget::generateNextFrame() {
    // CALL NEXT BOX2D FRAME
    // Instruct the world to perform a single step of simulation.
    // It is generally best to keep the time step and iterations fixed.
    world->Step(1.0f / 60.0f, 6, 2);

    frameCounter += 1;
    frameCounter %= maxFrames;

    currentSpritePlayer = frameCounter / (maxFrames / playerSpritePath.size());
    currentSpritePlayer %= playerSpritePath.size();

    currentSpriteOther = frameCounter / (maxFrames / otherSpritePath.size());
    currentSpriteOther %= otherSpritePath.size();

    // Get the position of the body
    b2Vec2 position = body->GetPosition();

    // Debug output
    //std::cout << position.y << std::endl;
    //std::cout << playerSpritePath.at(currentSpritePlayer).toStdString() << std::endl;
    //std::cout << otherSpritePath.at(currentSpriteOther).toStdString() << std::endl;

    // EMIT BOX2D FRAME TO MODEL
    // Position is now a value between [0.0, 1.0] and comes to rest at 1.0.
    emit newFrame(300, 120-(position.y*30), playerSpritePath.at(currentSpritePlayer), otherSpritePath.at(currentSpriteOther));

    if (continueAnimation) {
        QTimer::singleShot(16, this, [this]() { this->generateNextFrame(); });
    }
}
