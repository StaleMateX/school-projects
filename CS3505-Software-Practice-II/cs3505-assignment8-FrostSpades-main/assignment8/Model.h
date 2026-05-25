/// Model Serializer, Deserializer, header
/// By Joshua Brody, Jacob Xu
/// CS 3505 Assignment 7-8 Sprite Editor
/// 3/2024
///


#ifndef MODEL_H
#define MODEL_H

#include <QBuffer>
#include <QDebug>
#include <QFile>
#include <QImage>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QVector>


class Model : public QObject
{
    Q_OBJECT

private:
    ///
    /// \brief size it stores the width/height of the squared canvas
    /// When the user creates a project they are offered a choice on resolution. That choice is stored here for QImage settings
    ///
    int size;

    ///
    /// \brief frames is a map from the key (which is the id of the QImage) to a QImage.
    ///
    QMap<int, QImage> frames;

    ///
    /// \brief saveFilePath just stores where the user would to save.
    ///
    QString saveFilePath;

    ///
    /// \brief imageToBase64 helper method for loading images to json
    /// \param image    the image that wants to be saved
    /// \return     json serialized version of that image.
    ///
    QString imageToBase64(const QImage &image);

    ///
    /// \brief base64ToImage helper method for loading QImages from json.
    /// \param base64   the json serialization part of an image
    /// \return     The QImage of the serialization.
    ///
    QImage base64ToImage(const QString &base64);

    void prepareImagesToSend();

public:
    explicit Model(QObject *parent = nullptr);

    /*
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
    void changePixelData(int id, int x, int y, int a, int r, int g, int b);

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

signals:
    /**
     * @brief sendFrames
     * Send array of frames to view
     *
     * @param frames the array of frames to send
     */
    void sendFrames(QImage frames[]);
};

#endif // MODEL_H
