/**
 * Widget that represents the drawable
 * canvas of the editor.
 *
 * Reviewer: Ethan Andrews
 */

#ifndef DRAWWINDOW_H
#define DRAWWINDOW_H

#include <QMouseEvent>
#include <QPainter>
#include <QVector>
#include <QWidget>

class DrawWindow : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief DrawWindow
     * Qt constructor
     * @param parent
     */
    explicit DrawWindow(QWidget *parent = nullptr);

    /**
     * @brief setPixel
     * Sets the pixel color specified by the
     * parameters.
     * @param x the x coordinate of the pixel
     * @param y the y coordinate of the pixel
     * @param r the red value
     * @param g the green value
     * @param b the blue value
     * @param alpha the alpha value
     */
    void setPixel(int x, int y, int r, int g, int b, int alpha);

    /**
     * @brief deletePixel
     * Deletes the pixel at the specified
     * coordinates.
     *
     * @param x the x location
     * @param y the y location
     */
    void deletePixel(int x, int y);

    /**
     * @brief getPixel
     * Returns the pixel color of the pixel
     * at the parameter coordinates.
     *
     * @param x the x coordinate of the pixel
     * @param y the y coordinate of the pixel
     * @return the color of the pixel
     */
    QColor getPixel(int x, int y);

    /**
     * @brief getImageData
     * Returns the current frame
     *
     * @return
     */
    QImage getImageData();

protected:
    /**
     * @brief mousePressEvent
     * Handles clicks.
     * @param event
     */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * @brief mouseMoveEvent
     * Handles mouse moves.
     * @param event
     */
    void mouseMoveEvent(QMouseEvent *event) override;

    /**
     * @brief paintEvent
     * Defines how the canvas looks
     * @param event
     */
    void paintEvent(QPaintEvent *event) override;

private:
    QImage image; // Current frame
    QColor currentColor; // Current color
    int scale;                // Scale factor for pixels
    int size;                 // size of image
    int currentFrame;         // current frame being looked at/edited
    QMap<int, QImage> frames; // temporary storage of frames for drawing
public slots:
    /**
     * @brief changeFrame
     * Changes the current frame
     * @param ID
     */
    void changeFrame(int ID);

    /**
     * @brief updateFrames
     * Updates to the current frames
     * @param frames
     */
    void updateFrames(QMap<int, QImage> frames);

    /**
     * @brief changePixelSize
     * Changes the pixel size
     * @param size
     */
    void changePixelSize(int size);
signals:
    /**
     * @brief click
     * Emits the click data
     * @param frameID
     * @param x
     * @param y
     */
    void click(int frameID, int x, int y);
};

#endif // PIXELEDITORWIDGET_H
