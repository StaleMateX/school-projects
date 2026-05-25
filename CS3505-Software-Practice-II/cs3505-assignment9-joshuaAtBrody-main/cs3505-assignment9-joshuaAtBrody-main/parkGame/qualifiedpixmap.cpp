#include "qualifiedpixmap.h"
#include <QPixmap>

QualifiedPixmap::QualifiedPixmap() {}

QualifiedPixmap::QualifiedPixmap(QPixmap pixmap, int x, int y) : pixmap(pixmap), x(x), y(y){}
