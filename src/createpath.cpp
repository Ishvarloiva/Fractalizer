#include "createpath.h"

//Create QPainterPath by given vector of points
QPainterPath createPath(QVector<QPointF> points)
{
    QPainterPath tmp;
    tmp.moveTo(points.first());
    for(int i = 1; i < points.size(); i++)
    {
        tmp.lineTo(points.at(i));
    }
    return tmp;
}

QPainterPath createPath(QVector<Joint*> joints)
{
    QPainterPath tmp;
    tmp.moveTo(150, 500);
    for(int i = 0; i < joints.size(); i++)
    {
        tmp.lineTo(joints.at(i)->pos());
    }
    tmp.lineTo(850,500);
    return tmp;
}
