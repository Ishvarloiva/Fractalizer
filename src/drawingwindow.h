#ifndef DRAWINGWINDOW_H
#define DRAWINGWINDOW_H

#include <QGraphicsView>
#include <QPushButton>
#include <QVBoxLayout>

#include "createpath.h"
#include "joint.h"

class DrawingWindow : public QGraphicsView
{
    Q_OBJECT
public:
    explicit DrawingWindow(QVector<QPointF> *customRulePoints, QWidget *parent = 0);

    QVector<Joint*> joints;
    QVector<QPointF> *customRulePoints;
    QGraphicsPathItem *customRule;

    QGraphicsScene *drawingScene;

    QWidget *drawingSceneWidget;
    QVBoxLayout drawingSceneLayout;
    QPushButton *createJointButton;
    QPushButton *deleteJointButton;
    QPushButton *acceptCustomRule;
    QPushButton *cancelCustomRule;
    ~DrawingWindow();
public slots:
    void createJoint();
    void deleteJoint();
    void acceptRule();
    void cancelRule();
    void redraw();
};

#endif // DRAWINGWINDOW_H
