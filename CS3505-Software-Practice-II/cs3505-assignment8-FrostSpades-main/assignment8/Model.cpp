/// Model Serializer, Deserializer, cpp
/// By Joshua Brody, Jacob Xu
/// CS 3505 Assignment 7-8 Sprite Editor
/// 3/2024
///

#include "Model.h"

// Constructor
Model::Model(QObject *parent)
    : QObject(parent)
{
    size = 0;
}


Model::Model(){
    size = 0;
}

Model::~Model() {

}

// The user wants to create a project from scratch. An empty project is created into a Json and aded to filePath.
Model::Model(QString name, int thisSize, QString filePath)
{
    // Populate the Model
    size = thisSize;
    QImage starterImage(size, size, QImage::Format_RGBA16FPx4);
    frames.insert(0, starterImage);

    // Create JSON object
    QJsonObject jsonObject;
    jsonObject["size"] = size;

    // Write JSON to file
    QFile file(filePath + "/" + name + ".json");
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

}

// The user wants to load a previous project. The project is saved in a Json at that filePath.
// Deserialize the json and send it to view.
Model::Model(QString &filePath)
{
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
}

// The user wants to save the current project state. Update the json.
void Model::saveModel()
{
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
        // Handle error
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

void Model::addFrame(){
    int currentNumberOfFrames = frames.count();
    if(!frames.contains(currentNumberOfFrames)){
        QImage newFrame(size, size, QImage::Format_RGBA16FPx4);
        frames[currentNumberOfFrames] = newFrame;
    }

    prepareImagesToSend();
}


void Model::deleteFrame(int id){
    frames.remove(id);
}

void Model::changePixelData(int id, int x, int y, int a, int r, int g, int b){
    // VASKO HOW DID YOU USE QIMAGE
    QImage daFrameImage = frames[id];

    daFrameImage.setPixel(x, y, qRgba(r, g, b, a));
}

void Model::returnFrames(){
    prepareImagesToSend();
}

void Model::switchFrames(int frameOneID, int frameTwoID){
    QImage saveSwitchingFrame = frames[frameOneID];
    frames[frameOneID] = frames[frameTwoID];
    frames[frameTwoID] = saveSwitchingFrame;
}

void Model::prepareImagesToSend(){
    QImage* frameArray = new QImage[frames.count()];
    for (auto it = frames.begin(); it != frames.end(); ++it) {
        frameArray[it.key()] = it.value().copy();
    }
    emit sendFrames(frameArray);

}






