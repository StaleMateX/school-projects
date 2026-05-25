/**
 * Model Serializer, Deserializer, cpp
 * @authors Joshua Brody and Jacob Xu
 * CS 3505 Assignment 7-8 Sprite Editor
 * @date 3/2024
 *
 * Reviewer: Ethan Andrews
 */

#include "Model.h"
#include <iostream>

Model::Model(QObject *parent)
    : QObject(parent)
{
    size = 0;
}

Model::Model()
{
    size = 0;
    addFrame();
}

Model::~Model() {

}

// The user wants to create a project from scratch. An empty project is created into a Json and aded to filePath.
Model::Model(QString name, int thisSize, QString filePath)
{
    // Populate the Model
    size = thisSize;
    isSaved = true;
    QImage starterImage(size, size, QImage::Format_ARGB32);
    starterImage.fill(Qt::transparent);
    frames.insert(0, starterImage);

    // Create JSON object
    QJsonObject jsonObject;
    jsonObject["size"] = size;

    // Write JSON to file
    saveFilePath = filePath + "/" + name + ".ssp";
    QFile file(filePath + "/" + name + ".ssp");

    if (!file.open(QIODevice::WriteOnly)) {
        // Handle error
        qDebug() << "Failed to open file for writing:" << file.errorString();
        return;
    }

    QJsonDocument jsonDoc(jsonObject);
    file.write(jsonDoc.toJson());
    file.close();

    saveModel();

    // TODO: send open empty frame to view
    sendImages();
}

// The user wants to load a previous project. The project is saved in a Json at that filePath.
// Deserialize the json and send it to view.
Model::Model(QString &filePath)
{
    saveFilePath = filePath;
    isSaved = true;

    // Read JSON file
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        // Handle error
        return;
    }

    // Parse JSON
    QByteArray jsonData = file.readAll();
    QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonData));
    QJsonObject jsonObject = jsonDoc.object();

    // Deserialize size
    size = jsonObject["size"].toInt();

    // Deserialize imageMap
    frames.clear();
    QJsonObject imageMapObject = jsonObject["imageMap"].toObject();
    QStringList keys = imageMapObject.keys();
    for (const QString &key : keys) {
        int mapKey = key.toInt();
        QImage image = base64ToImage(imageMapObject[key].toString());
        frames[mapKey] = image;
    }

    file.close();

    // TODO: Open frames to view
    sendImages();
}

// The user wants to save the current project state. Update the json.
void Model::saveModel()
{
    isSaved = true;

    // Create JSON object
    QJsonObject jsonObject;
    jsonObject["size"] = size;

    // Serialize QMap<int, QImage>
    QJsonObject imageMapObject;
    for (auto it = frames.begin(); it != frames.end(); ++it) {
        imageMapObject[QString::number(it.key())] = imageToBase64(it.value());
    }
    jsonObject["imageMap"] = imageMapObject;

    // Write JSON to file
    QFile file(saveFilePath);
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument jsonDoc(jsonObject);
        file.write(jsonDoc.toJson());
        file.close();
    } else {
        std::cout << "Couldn't open file" << std::endl;
    }
}

QString Model::imageToBase64(const QImage &image)
{
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG"); // Save QImage as PNG format, you can change this if needed
    return QString::fromLatin1(byteArray.toBase64().data());
}

QImage Model::base64ToImage(const QString &base64)
{
    QByteArray imageData = QByteArray::fromBase64(base64.toLatin1());
    QImage image;
    image.loadFromData(imageData);
    return image;
}

//  ------- Slots --------

void Model::addFrame()
{
    QImage newFrame(size, size, QImage::Format_ARGB32);
    newFrame.fill(Qt::transparent);
    // frames.count is one bigger than the index of the last frame because the first frame has id of 0
    frames.insert(frames.count(), newFrame);

    sendImages();
}

void Model::deleteFrame(int id)
{
    int framesSize = frames.size();

    // Make it impossible to delete all frames
    if (framesSize == 1) {
        return;
    }

    frames.remove(id);

    for (int i = id + 1; i < framesSize; i++) {
        QImage previousValue = frames[i];
        frames.remove(i);
        frames[i - 1] = previousValue;
    }

    sendImages();
}

void Model::changePixelData(int id, int x, int y, int r, int g, int b, int a)
{
    frames[id].setPixel(x, y, qRgba(r, g, b, a));
    sendImages();
}

void Model::returnFrames()
{
    sendImages();
}

void Model::switchFrames(int frameOneID, int frameTwoID)
{
    QImage saveSwitchingFrame = frames[frameOneID];
    frames[frameOneID] = frames[frameTwoID];
    frames[frameTwoID] = saveSwitchingFrame;

    sendImages();
}

void Model::sendImages()
{
    emit sendNumberOfFrames(frames.count());
    emit sendFrames(frames);

    isSaved = false;
}

void Model::changeFrame(Tool tool, int frameID, int x, int y, int r, int g, int b, int a)
{
    if (tool == Tool::pen) {
        changePixelData(frameID, x, y, r, g, b, a);
    }
    if (tool == Tool::eraser) {
        changePixelData(frameID, x, y, 255, 255, 255, 0);
    }
    if (tool == Tool::fillTool) {
        frames[frameID].fill(QColor::fromRgba(qRgba(r, g, b, a)));
        sendImages();
    }
}

void Model::transmitSize()
{
    emit sendSize(size);
    sendImages();
}

void Model::notifyView()
{
    sendImages();
}

void Model::swapFrames(int leftFrameID, int rightFrameID)
{
    if (leftFrameID != rightFrameID) {
        frames[leftFrameID].swap(frames[rightFrameID]);
    }
    sendImages();
}

bool Model::getIsSaved()
{
    return isSaved;
}

void Model::invertColors(int frameID)
{
    frames[frameID].invertPixels();
    sendImages();
}

void Model::mirrorHorizontal(int frameID)
{
    frames[frameID].mirror(true, false);
    sendImages();
}

void Model::mirrorVertical(int frameID)
{
    frames[frameID].mirror();
    sendImages();
}

void Model::setSaved(bool value) {
    isSaved = value;
}
