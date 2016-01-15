#ifndef CREATEPATH
#define CREATEPATH

#include <QPainterPath>

#include "joint.h"

//Two function to draw compound path by either vector of QPointF or pointers to Joint
QPainterPath createPath(QVector<QPointF> points);
QPainterPath createPath(QVector<Joint*> joints);

#endif // CREATEPATH

