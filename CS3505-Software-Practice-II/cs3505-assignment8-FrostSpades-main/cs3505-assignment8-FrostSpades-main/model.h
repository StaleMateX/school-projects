/**
 * Model Serializer, Deserializer, header
 * @authors Joshua Brody, Jacob Xu
 * CS 3505 Assignment 7-8 Sprite Editor
 * @date 3/2024
 *
 * Reviewer: Ethan Andrews
 **/

#ifndef MODEL_H
#define MODEL_H

#include <QBuffer>
#include <QDebug>
#include <QFile>
#include <QFrame>
#include <QImage>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QVector>
#include "tools.h"

class Model : public QObject
{
    Q_OBJECT

private:
    int size; // the width/height of the squared canvas

    bool isSaved; // bool representing if the newest changes have been saved

    QMap<int, QImage> frames; // map from the key (which is the id of the QImage) to a QImage.

    QString saveFilePath; // stores the save path

    /**
     * @brief imageToBase64 helper method for loading images to json
     * @param image the image that wants to be saved
     * @return json serialized version of that image.
     */
    QString imageToBase64(const QImage &image);

    /**
     * @brief base64ToImage helper method for loading QImages from json.
     * @param base64 the json serialization part of an image
     * @return The QImage of the serialization.
     */
    QImage base64ToImage(const QString &base64);

    /**
     * @brief sendImages starts the process of sending the images
     */
    void sendImages();

public:
    /**
     * @brief Model
     * Qt constructor
     * @param parent
     */
    explicit Model(QObject *parent = nullptr);

    /**
     * @brief Model
     * Default Constructor
     */
    Model();

    /**
     * @brief Model
     * Creates a new model object with
     * name, size, and path variables set.
     *
     * @param name the name of the project
     * @param size the size of the project
     * @param path the path of the project
     */
    Model(QString name, int size, QString path);

    /**
     * @brief Model
     * Creates a new model object based
     * on a given json .ssp file.
     *
     * @param filePath
     */
    Model(QString &filePath);

    /**
     * Default Deconstructor
     */
    ~Model();

    /**
     * @brief transmitSize
     * Sends the size to connected
     * components.
     */
    void transmitSize();

    /**
     * @brief notifyView
     * Notifies the view of the current
     * frames.
     */
    void notifyView();

    /**
     * @brief getIsSaved
     * Returns if there are unsaved changes
     * @return true if saved all changes
     */
    bool getIsSaved();

    /**
     * @brief setSaved
     * Sets the isSaved value
     *
     * @param value the new isSaved value
     */
    void setSaved(bool value);

public slots:
    /**
     * @brief addFrame
     * Inserts a frame at the end
     * of the map.
     */
    void addFrame();

    /**
     * @brief deleteFrame
     * Deletes a frame at the given id
     * and resets all the ids to their
     * position.
     *
     * @param id the id of the frame to delete
     */
    void deleteFrame(int id);

    /**
     * @brief changePixelData
     * Changes the pixel of the frame at the
     * given id, at the given location (x, y)
     * to the given argb value.
     *
     * @param id the id of the frame
     * @param x the x location
     * @param y the y location
     * @param a alpha value
     * @param r red value
     * @param g green value
     * @param b blue value
     */
    void changePixelData(int id, int x, int y, int r, int g, int b, int a);

    /**
     * @brief returnFrames
     * Trigger the sending of the array of frames
     * to the view in the correct ID order.
     */
    void returnFrames();

    /**
     * @brief switchFrames
     * Switches the frames at the given
     * id's.
     *
     * @param frameOneID
     * @param frameTwoID
     */
    void switchFrames(int frameOneID, int frameTwoID);

    /**
     * @brief saveModel
     * Saves the model to the given file path.
     */
    void saveModel();

    /**
     * @brief changeFrame
     * Changes the frame corresponding to the data
     * in the parameters.
     * @param tool the tool that was used to change the frame
     * @param frameID the ID of the frame to be changed
     * @param x the x position of the change
     * @param y the y position of the change
     * @param r the red value of the tool
     * @param g the green value of the tool
     * @param b the blue value of the tool
     * @param a the alpha value of the tool
     */
    void changeFrame(Tool tool, int frameID, int x, int y, int r, int g, int b, int a);

    /**
     * @brief swapFrames
     * Swaps the frames at leftFrameID
     * and rightFrameID
     * @param leftFrameID the first frame to swap
     * @param rightFrameID the second frame to swap
     */
    void swapFrames(int leftFrameID, int rightFrameID);

    /**
     * @brief invertColors
     * Inverts the pixel colors of the frame
     * at frameID
     *
     * @param frameID the ID of the frame
     */
    void invertColors(int frameID);

    /**
     * @brief mirrorHorizontal
     * Mirrors the frame in the horizontal
     * direction.
     *
     * @param frameID the id of the frame
     * to be mirrored
     */
    void mirrorHorizontal(int frameID);

    /**
     * @brief mirrorVertical
     * Mirrors the frame in the vertical
     * direction.
     *
     * @param frameID the id of the frame
     * to be mirrored
     */
    void mirrorVertical(int frameID);

signals:
    /**
     * @brief sendFrames
     * Send frames to view
     *
     * @param frames the frames to send
     */
    void sendFrames(QMap<int, QImage> frames);

    /**
     * @brief sendSize
     * Sends size to view
     *
     * @param size
     */
    void sendSize(int size);

    /**
     * @brief sendNumberOfFrames
     * Sends the number of frames to
     * view.
     *
     * @param numberOfFrames
     */
    void sendNumberOfFrames(int numberOfFrames);
};

#endif // MODEL_H
