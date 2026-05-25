/**
 * Implementation of draw window class.
 *
 * Reviewer: Ethan Andrews
 */

#include "drawwindow.h"
#include <iostream>

DrawWindow::DrawWindow(QWidget *parent)
    : QWidget(parent)
{
    setMouseTracking(true);
    resize(320, 320); // Set initial size to size X size
    size = 8;
    image = QImage(size, size, QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    scale = 320 / size;
    currentFrame = 0;
    frames = QMap<int, QImage>();
}

void DrawWindow::changePixelSize(int newSize)
{
    size = newSize;
    image = QImage(size, size, QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    scale = 320 / size;
}

void DrawWindow::setPixel(int x, int y, int r, int g, int b, int alpha)
{
    if (x >= 0 && x < size && y >= 0 && y < size) {
        QRgb color = qRgba(r, g, b, alpha);
        image.setPixel(x, y, color);
        update(); // Update widget to reflect changes
    }
}

void DrawWindow::deletePixel(int x, int y)
{
    if (x >= 0 && x < size && y >= 0 && y < size) {
        image.setPixel(x, y, qRgba(255, 255, 255, 0)); // Set pixel to transparent
        update();                                      // Update widget to reflect changes
    }
}

QColor DrawWindow::getPixel(int x, int y)
{
    if (x >= 0 && x < size && y >= 0 && y < size) {
        return QColor(image.pixel(x, y));
    }
    return QColor(); // Return null color if out of bounds
}

QImage DrawWindow::getImageData()
{
    return image.copy(); // Return a copy to avoid direct manipulation of internal data
}

void DrawWindow::mousePressEvent(QMouseEvent *event)
{
    int x = event->position().x() / scale; // Adjust coordinates for scaled image
    int y = event->position().y() / scale;

    emit click(currentFrame, x, y);
}

void DrawWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        int x = event->position().x() / scale; // Adjust coordinates for scaled image
        int y = event->position().y() / scale;
        emit click(currentFrame, x, y);
    }
}

void DrawWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawImage(0, 0, image.scaled(QSize(320, 320))); // Scale image to fit widget size
}

void DrawWindow::changeFrame(int ID)
{
    currentFrame = ID - 1;
    image = frames[currentFrame];

    update();
}

void DrawWindow::updateFrames(QMap<int, QImage> incomingFrames)
{
    frames = incomingFrames;
    image = incomingFrames[currentFrame];

    update();
}
