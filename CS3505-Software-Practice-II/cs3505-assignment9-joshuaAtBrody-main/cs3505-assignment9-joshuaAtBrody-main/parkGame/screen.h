/**
 * Interface for a screen object.
 *
 * @author Ethan Andrews
 */
#ifndef SCREEN_H
#define SCREEN_H

#include <QObject>

class Screen : public QObject
{
    Q_OBJECT
public:
    Screen();
    Screen(Screen&& other);
    Screen(const Screen& other);
    Screen& operator=(const Screen& other);
};

#endif // SCREEN_H
